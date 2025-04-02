#include "Theme.h"
#include "ThemeDefinition.h"

void SetTheme(Theme theme)
{
    ImGuiStyle &style = ImGui::GetStyle();
    switch (theme)
    {
    case Theme::Light:
        ImGui::StyleColorsLight();
        break;
    case Theme::Dark:
        ImGui::StyleColorsDark();
        break;
    case Theme::Blue:
    {
        for (size_t i = 0; i < BlueThemeColorCount; ++i)
        {
            style.Colors[BlueThemeColors[i].index] = BlueThemeColors[i].color;
        }
        break;
    }
    case Theme::Neon:
    {
        for (size_t i = 0; i < NeonThemeColorCount; ++i)
        {
            style.Colors[NeonThemeColors[i].index] = NeonThemeColors[i].color;
        }
        break;
    }
    }
}