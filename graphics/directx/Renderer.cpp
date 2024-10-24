#include  "Renderer.h"

#include <string>
#include <memory>
#include <ppltasks.h>

#include <fstream>

Renderer::Renderer(std::shared_ptr<DeviceResources> DeviceResources) : FrameCount(0), Resx(DeviceResources)
{
	IndexCount = 0;
}
Renderer::~Renderer()
{

}

HRESULT Renderer::CreateShaders()
{
	HRESULT hr = S_OK;
	ID3D11Device* Device = Resx->GetDevice();

	{
		ID3DBlob* VertexBlob = nullptr;
		hr = CompileShader(L"CubeVertexShader.hlsl", "main", "vs_4_0_level_9_1", &VertexBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr, L"Failed to compile shader.", L"error", MB_ICONERROR | MB_OK);
			return hr;
		}
		hr = Device->CreateVertexShader(VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), nullptr, &VertexShader);

		D3D11_INPUT_ELEMENT_DESC Desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		hr = Device->CreateInputLayout(Desc, ARRAYSIZE(Desc), VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), &InputLayout);
		VertexBlob->Release();
	}

	{
		ID3DBlob* PixelBlob = nullptr;
		hr = CompileShader(L"CubePixelShader.hlsl", "main", "ps_4_0_level_9_1", &PixelBlob);
		if (FAILED(hr))
		{
			MessageBox(nullptr, L"Failed to compile pixel shader.", L"Error", MB_ICONERROR | MB_OK);
			PostQuitMessage(0);
			return hr;
		}
		hr = Device->CreatePixelShader(PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), nullptr, PixelShader.GetAddressOf());
		PixelBlob->Release();

		CD3D11_BUFFER_DESC cbDesc(sizeof(ConstantBufferStruct), D3D11_BIND_CONSTANT_BUFFER);
		hr = Device->CreateBuffer(&cbDesc, nullptr, ConstantBuffer.GetAddressOf());
	}

	return hr;
}
HRESULT Renderer::CreateCube()
{
	HRESULT hr = S_OK;
	ID3D11Device* Device = Resx->GetDevice();

	VertexPositionColor CubeVertices[] =
	{
		{DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0,   0,   0),},
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0,   0,   1),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0,   1,   0),},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0,   1,   1),},

		{DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(1,   0,   0),},
		{DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(1,   0,   1),},
		{DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(1,   1,   0),},
		{DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1,   1,   1),},
	};

	CD3D11_BUFFER_DESC vDesc(sizeof(CubeVertices), D3D11_BIND_VERTEX_BUFFER);
	D3D11_SUBRESOURCE_DATA vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = CubeVertices;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	hr = Device->CreateBuffer(&vDesc, &vData, &VertexBuffer);

	// Create index buffer:
	unsigned short CubeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	IndexCount = ARRAYSIZE(CubeIndices);
	CD3D11_BUFFER_DESC iDesc(sizeof(CubeIndices), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = CubeIndices;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	hr = Device->CreateBuffer(&iDesc, &iData, &IndexBuffer);
	return hr;
}

void Renderer::CreateViewAndPerspective()
{
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 0.7f, 1.5f, 0.f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.f, 0.0f, 0.f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.f);

	DirectX::XMStoreFloat4x4(&ConstantBufferData.View, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(Eye, At, Up)));

	float AspectRatioX = Resx->GetAspectRatio(), AspectRatioY = AspectRatioX < (16.0f / 9.0f) ? AspectRatioX / (16.0f / 9.0f) : 1.0f;

	DirectX:XMStoreFloat4x4(&ConstantBufferData.Projection,
		DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovRH(2.0f * std::atan(std::tan(DirectX::XMConvertToRadians(70) * 0.5f) / AspectRatioY), AspectRatioX, 0.01f, 100.0f)
		));
}

void Renderer::CreateDeviceDependentResources()
{
	CreateShaders(); 

	CreateCube();
}
void Renderer::CreateWindowSizeDependentResources()
{
	CreateViewAndPerspective();
}

void Renderer::Update()
{
	float Angle = DirectX::XMConvertToRadians((float)(FrameCount++));
	DirectX::XMMATRIX RotMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationY(Angle*2), DirectX::XMMatrixRotationZ(sin(Angle)));

	RotMatrix = DirectX::XMMatrixMultiply(RotMatrix, DirectX::XMMatrixScaling(0.2+0.5*sin(Angle*2), 1.5+0.5*cos(Angle*3), 1));

	DirectX::XMStoreFloat4x4(&ConstantBufferData.World, DirectX::XMMatrixTranspose(RotMatrix));

	if (FrameCount == MAXUINT) 
		FrameCount = 0;
}
void Renderer::Render()
{
	ID3D11DeviceContext* Context = Resx->GetDeviceContext();
	ID3D11RenderTargetView* RenderTarget = Resx->GetRenderTarget();
	ID3D11DepthStencilView* DepthStencil = Resx->GetDepthStencil();

	Context->UpdateSubresource(ConstantBuffer.Get(), 0, nullptr, &ConstantBufferData, 0, 0);

	const float Teal[] = { 0.098f, 0.439f, 0.439f, 1.000f };
	Context->ClearRenderTargetView(RenderTarget, Teal);
	Context->ClearDepthStencilView(DepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Context->OMSetRenderTargets(1, &RenderTarget, DepthStencil);

	UINT Stride = sizeof(VertexPositionColor), Offset = 0;

	Context->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);
	Context->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Context->IASetInputLayout(InputLayout.Get());

	Context->VSSetShader(VertexShader.Get(), nullptr, 0);
	Context->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
	Context->PSSetShader(PixelShader.Get(), nullptr, 0);
	
	Context->DrawIndexed(IndexCount, 0, 0);
}

HRESULT CompileShader(LPCWSTR Source, LPCSTR EntryPoint, LPCSTR Profile, ID3DBlob** Blob)
{
	if (!Source || !EntryPoint || !Profile || !Blob)
		return E_INVALIDARG;

	*Blob = nullptr;

	UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	Flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO Defines[] = { "EXAMPLE_DEFINE", "1", NULL, NULL };

	ID3DBlob* ShaderBlob = nullptr, * ErrorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(Source, Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint, Profile, Flags, 0, &ShaderBlob, &ErrorBlob);

	if (FAILED(hr))
	{
		if (ErrorBlob)
		{
			OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
			ErrorBlob->Release();
		}

		if (ShaderBlob)
			ShaderBlob->Release();

		return hr;
	}

	*Blob = ShaderBlob;

	return hr;
}