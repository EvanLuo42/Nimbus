#include "Application.h"
#include <chrono>

#include "d3dx12.h"

Application::Application(HINSTANCE hInstance, uint32_t width, uint32_t height)
{
    m_window = std::make_unique<Win32Window>(hInstance, width, height, L"Nimbus Renderer");
    m_device = std::make_unique<Nimbus::Core::Device>(m_window->GetHandle(), width, height);
}

void Application::Run()
{
    while (true)
    {
        if (!m_window->ProcessMessages())
            break;

        if (!m_running)
            break;

        OnUpdate();
        OnRender();
    }
    OnDestroy();
}

void Application::OnUpdate()
{
}

void Application::OnRender() const
{
    m_device->BeginFrame();
    auto* cmd = m_device->GetCommandList();
    const auto rtv = m_device->GetCurrentRTV();
    constexpr float clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    const auto resourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
        m_device->GetBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    cmd->ResourceBarrier(1, &resourceBarrierPresent);
    cmd->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    const auto resourceBarrierRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
    m_device->GetBackBuffer(),
    D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    cmd->ResourceBarrier(1, &resourceBarrierRenderTarget);
    m_device->Present();
}

void Application::OnDestroy()
{
    FreeConsole();
    ExitProcess(0);
}
