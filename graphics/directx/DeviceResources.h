#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl\client.h>

class DeviceResources
{
public:
	DeviceResources();
	~DeviceResources();

	HRESULT CreateDeviceResources(HWND Window);
	HRESULT CreateDeviceResources();
	HRESULT CreateWindowResources(HWND Window);

	HRESULT ConfigureBackBuffer();
	HRESULT ReleaseBackBuffer();

	bool FullScreen() const { return _FullScreen; }
	bool GoFullScreen();
	bool GoWindowed();

	float GetAspectRatio();

	ID3D11Device* GetDevice() { return Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return DeviceContext.Get(); }
	ID3D11RenderTargetView* GetRenderTarget() { return RenderTarget.Get(); }
	ID3D11DepthStencilView* GetDepthStencil() { return DepthStencilView.Get(); }

	void Present();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

	D3D_FEATURE_LEVEL FeatureLevel;
	D3D11_TEXTURE2D_DESC BackBufferDesc;
	D3D11_VIEWPORT Viewport;

	bool _FullScreen = false;
};