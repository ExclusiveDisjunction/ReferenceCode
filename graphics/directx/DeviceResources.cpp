#include "DeviceResources.h"
#include <d3d11_3.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <string>
#include <memory>

DeviceResources::DeviceResources()
{

}
DeviceResources::~DeviceResources()
{

}

HRESULT DeviceResources::CreateDeviceResources(HWND Window)
{
	HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL Levels[] = {
       D3D_FEATURE_LEVEL_9_1,
       D3D_FEATURE_LEVEL_9_2,
       D3D_FEATURE_LEVEL_9_3,
       D3D_FEATURE_LEVEL_10_0,
       D3D_FEATURE_LEVEL_10_1,
       D3D_FEATURE_LEVEL_11_0,
       D3D_FEATURE_LEVEL_11_1
    };

    UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.Windowed = true;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = Window;

    {
        Microsoft::WRL::ComPtr<ID3D11Device> Device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

        hr = D3D11CreateDeviceAndSwapChain(nullptr,
            D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            DeviceFlags,
            Levels,
            ARRAYSIZE(Levels),
            D3D11_SDK_VERSION,
            &desc,
            swapChain.GetAddressOf(),
            Device.GetAddressOf(),
            &FeatureLevel,
            Context.GetAddressOf());

        Device.As(&this->Device);
        Context.As(&this->DeviceContext);
        swapChain.As(&this->SwapChain);
    }

    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

    BackBuffer->GetDesc(&BackBufferDesc);

    ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
    Viewport.Height = (float)BackBufferDesc.Height;
    Viewport.Width = (float)BackBufferDesc.Width;
    Viewport.MinDepth = 0;
    Viewport.MaxDepth = 1;

    DeviceContext->RSSetViewports(1, &Viewport);

    hr = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, RenderTarget.GetAddressOf());

    return hr;
}

HRESULT DeviceResources::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL Levels[] = {
        D3D_FEATURE_LEVEL_9_1,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1
    };

    UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3D11Device> Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
    

    hr = D3D11CreateDevice(nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        DeviceFlags,
        Levels,
        ARRAYSIZE(Levels),
        D3D11_SDK_VERSION,
        &Device,
        &FeatureLevel,
        &Context);

    if (FAILED(hr))
    {

    }

    Device.As(&this->Device);
    Context.As(&this->DeviceContext);
    return hr;
}
HRESULT DeviceResources::CreateWindowResources(HWND Window)
{
    HRESULT hr = S_OK;

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.Windowed = true;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.OutputWindow = Window;

    Microsoft::WRL::ComPtr<IDXGIDevice3> Device;
    this->Device.As(&Device);

    Microsoft::WRL::ComPtr<IDXGIAdapter> Adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> Factory;

    hr = Device->GetAdapter(&Adapter);

    if (SUCCEEDED(hr)) 
    {
        Adapter->GetParent(IID_PPV_ARGS(&Factory));

        hr = Factory->CreateSwapChain(this->Device.Get(), &desc, &this->SwapChain);
    }

    hr = ConfigureBackBuffer();

    return hr;
}

HRESULT DeviceResources::ConfigureBackBuffer()
{
    HRESULT hr = S_OK;

    hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
    if (FAILED(hr))
    {
        OutputDebugStringA("The swap chain's buffer was not obtained.");
        return hr;
    }

    hr = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, RenderTarget.GetAddressOf());
    if (FAILED(hr))
    {
        OutputDebugStringA("The device failed to create a render target view.");
        return hr;
    }

    BackBuffer->GetDesc(&BackBufferDesc);

    CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, static_cast<UINT>(BackBufferDesc.Width), static_cast<UINT>(BackBufferDesc.Height), 1, 1, D3D11_BIND_DEPTH_STENCIL);
    Device->CreateTexture2D(&depthStencilDesc, nullptr, &DepthStencil);

    CD3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    Device->CreateDepthStencilView(DepthStencil.Get(), &DepthStencilViewDesc, &DepthStencilView);

    ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
    Viewport.Height = (float)BackBufferDesc.Height;
    Viewport.Width = (float)BackBufferDesc.Width;
    Viewport.MinDepth = 0;
    Viewport.MaxDepth = 1;

    DeviceContext->RSSetViewports(1, &Viewport);

    return hr;
}
HRESULT DeviceResources::ReleaseBackBuffer()
{
    HRESULT hr = S_OK;

    RenderTarget.Reset();
    BackBuffer.Reset();

    DepthStencilView.Reset();
    DepthStencil.Reset();

    DeviceContext->Flush();

    return hr;
}

bool DeviceResources::GoFullScreen()
{
    HRESULT hr = S_OK;

    hr = SwapChain->SetFullscreenState(true, nullptr);

    ReleaseBackBuffer();
    hr = SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr))
        OutputDebugStringA("The swap chain could not resize.");
    hr = ConfigureBackBuffer();

    _FullScreen = true;
    return SUCCEEDED(hr);
}
bool DeviceResources::GoWindowed()
{
    HRESULT hr = S_OK;

    hr = SwapChain->SetFullscreenState(false, nullptr);

    ReleaseBackBuffer();
    hr = SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    hr = ConfigureBackBuffer();

    _FullScreen = false;
    return hr;
}

float DeviceResources::GetAspectRatio()
{
    return static_cast<float>(BackBufferDesc.Width) / static_cast<float>(BackBufferDesc.Height);
}
void DeviceResources::Present()
{
    SwapChain->Present(1, 0);
}