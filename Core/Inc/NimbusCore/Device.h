#pragma once

#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Nimbus::Core
{
class Device
{
public:
    static constexpr uint32_t kFrameCount = 2;

    Device(HWND hwnd, uint32_t width, uint32_t height);
    ~Device();

    void BeginFrame() const;
    void Present();

    [[nodiscard]] ID3D12Device* GetDevice() const
    {
        return m_device.Get();
    }

    [[nodiscard]] ID3D12CommandQueue* GetCommandQueue() const
    {
        return m_commandQueue.Get();
    }

    [[nodiscard]] ID3D12GraphicsCommandList* GetCommandList() const
    {
        return m_commandList.Get();
    }

    [[nodiscard]] IDXGISwapChain4* GetSwapchain() const
    {
        return m_swapChain.Get();
    }

    [[nodiscard]] ID3D12Resource* GetBackBuffer() const
    {
        return m_backBuffers[m_frameIndex].Get();
    }

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTV() const;

private:
    void CreateDevice();
    void CreateCommandObjects();
    void CreateSwapchain(HWND hwnd, uint32_t w, uint32_t h);
    void CreateRTVHeap();
    void CreateFences();

private:
    uint32_t m_width = 0;
    uint32_t m_height = 0;

    ComPtr<IDXGIFactory6> m_factory;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12CommandAllocator> m_commandAllocators[kFrameCount];
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    ComPtr<IDXGISwapChain4> m_swapChain;

    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    std::vector<ComPtr<ID3D12Resource>> m_backBuffers;
    UINT m_rtvDescriptorSize = 0;
    UINT m_frameIndex = 0;

    ComPtr<ID3D12Fence> m_fence;
    uint64_t m_fenceValue = 0;
    HANDLE m_fenceEvent = nullptr;
};
} // namespace Nimbus::Core
