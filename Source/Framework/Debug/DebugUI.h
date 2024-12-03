#pragma once

#include "../Graphics/Color.h"
#include "../Graphics/ColorList.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include <entt.hpp>
#include <vector>
#include <string>


namespace GameDev2D
{
    class BatchRenderer;
    class SpriteFont;

    //Constants
    const float DEBUG_TEXT_SPACING = 4.0f;

    //DebugUI class, will display the Game's FPS and other relevant information on screen. You may
    //also register a function pointer for most datatype and it will display it on screen as well
    class DebugUI
    {
    public:
        DebugUI();
        ~DebugUI();
        
        void SetTextColor(const Color& color);

        entt::sink<unsigned long long()> UnsignedLongLongSlot;
        entt::sink<unsigned int()> UnsignedIntSlot;
        entt::sink<int()> IntSlot;
        entt::sink<float()> FloatSlot;
        entt::sink<double()> DoubleSlot;
        entt::sink<Vector2()> Vector2Slot;
        entt::sink<std::string()> StringSlot;


    private:
        //Draws the debug text to the screen
        void OnLateRender(BatchRenderer& batchRenderer);
        void DrawOutputData(BatchRenderer& batchRenderer, std::vector<std::string>& output, Vector2& position);

        template <typename type>
        void GetOutputString(entt::sigh<type()>& signal, std::vector<std::string>& output)
        {
            signal.collect([&output](type value) { output.push_back(std::to_string(value)); });
        }

        void GetOutputString(entt::sigh<Vector2()>& signal, std::vector<std::string>& output)
        {
            signal.collect([&output](Vector2 value) { output.push_back(std::to_string(value.x) + "," + std::to_string(value.y)); });
        }

        void GetOutputString(entt::sigh<std::string()>& signal, std::vector<std::string>& output)
        {
            signal.collect([&output](std::string value) { output.push_back(value); });
        }

        //Member variables
        entt::sigh<unsigned long long()> m_UnsignedLongLongSignal;
        entt::sigh<unsigned int()> m_UnsignedIntSignal;
        entt::sigh<int()> m_IntSignal;
        entt::sigh<float()> m_FloatSignal;
        entt::sigh<double()> m_DoubleSignal;
        entt::sigh<Vector2()> m_Vector2Signal;
        entt::sigh<std::string()> m_StringSignal;
        SpriteFont* m_SpriteFont;
    };
}

