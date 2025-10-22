#pragma once

#include "DeviceResources.h"
#include <memory>
#include <DirectXMath.h>

class Renderer
{
public:
	Renderer(std::shared_ptr<DeviceResources> DeviceResources);
	~Renderer();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void Update();
	void Render();

private:
	HRESULT CreateShaders();
	HRESULT CreateCube();
	void CreateViewAndPerspective();

	std::shared_ptr<DeviceResources> Resx;

	struct ConstantBufferStruct
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Projection;
	};

	static_assert((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Color;
	};
	struct VertexPositionColorTangent
	{
		DirectX::XMFLOAT3 Pos;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 Tangent;
	};

	ConstantBufferStruct ConstantBufferData;
	unsigned int IndexCount;
	unsigned int FrameCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayoutExtended;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
};

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

HRESULT CompileShader(LPCWSTR Source, LPCSTR EntryPoint, LPCSTR Profile, ID3DBlob** Blob);