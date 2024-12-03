#include "IRenderer.h"

namespace GameDev2D
{
    void IRenderer::NewFrame()
    {
        m_LastFrameDrawCalls = m_FrameDrawCalls;
        m_FrameDrawCalls = 0;
    }

    IRenderer::IRenderer() :
        m_TotalDrawCalls(0),
        m_FrameDrawCalls(0)
    {
    }

    unsigned long long IRenderer::GetTotalDrawCalls()
    {
        return m_TotalDrawCalls;
    }

    unsigned long long IRenderer::GetLastFrameDrawCalls()
    {
        return m_LastFrameDrawCalls;
    }
}
