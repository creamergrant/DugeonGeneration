#include "DebugUI.h"
#include "../Application/Application.h"
#include "../Graphics/BatchRenderer.h"
#include "../Graphics/SpriteFont.h"
#include "../Utils/Text/Text.h"
#include <GameDev2D.h>
#include <sstream>


namespace GameDev2D
{
    DebugUI::DebugUI() :
        UnsignedLongLongSlot(m_UnsignedLongLongSignal),
        UnsignedIntSlot(m_UnsignedIntSignal),
        IntSlot(m_IntSignal),
        FloatSlot(m_FloatSignal),
        DoubleSlot(m_DoubleSignal),
        Vector2Slot(m_Vector2Signal),
        StringSlot(m_StringSignal),
        m_SpriteFont(nullptr)
    {
        m_SpriteFont = new SpriteFont("OpenSans-CondBold_32");

        Application::Get().LateRenderSlot.connect<&DebugUI::OnLateRender>(this);
    }

    DebugUI::~DebugUI()
    {
        if (m_SpriteFont != nullptr)
        {
            delete m_SpriteFont;
            m_SpriteFont = nullptr;
        }

        Application::Get().LateRenderSlot.disconnect<&DebugUI::OnLateRender>(this);
    }

    void DebugUI::SetTextColor(const Color& color)
    {
        m_SpriteFont->SetColor(color);
    }

    void DebugUI::OnLateRender(BatchRenderer& batchRenderer)
    {
        Camera camera(Viewport(GetScreenWidth(), GetScreenHeight()));
        batchRenderer.BeginScene(&camera);

        const float height = static_cast<float>(Application::Get().GetWindow().GetHeight());
        Vector2 position(DEBUG_TEXT_SPACING, height - m_SpriteFont->GetFont()->GetLineHeight() - DEBUG_TEXT_SPACING);

        std::vector<std::string> output;
        GetOutputString(m_UnsignedIntSignal, output);
        DrawOutputData(batchRenderer, output, position);

        output.clear();
        GetOutputString(m_IntSignal, output);
        DrawOutputData(batchRenderer, output, position);

        output.clear();
        GetOutputString(m_UnsignedLongLongSignal, output);
        DrawOutputData(batchRenderer, output, position);

        output.clear();
        GetOutputString(m_FloatSignal, output);
        DrawOutputData(batchRenderer, output, position);

        output.clear();
        GetOutputString(m_DoubleSignal, output);
        DrawOutputData(batchRenderer, output, position);

        output.clear();
        GetOutputString(m_Vector2Signal, output);
        DrawOutputData(batchRenderer, output, position);

        output.clear();
        GetOutputString(m_StringSignal, output);
        DrawOutputData(batchRenderer, output, position);

        batchRenderer.EndScene();
    }

    void DebugUI::DrawOutputData(BatchRenderer& batchRenderer, std::vector<std::string>& output, Vector2& position)
    {
        for (size_t i = 0; i < output.size(); i++)
        {
            m_SpriteFont->SetText(output.at(i));
            m_SpriteFont->SetPosition(position);
            batchRenderer.RenderSpriteFont(m_SpriteFont);
            position.y -= m_SpriteFont->GetFont()->GetLineHeight() + DEBUG_TEXT_SPACING;
        }
    }
}