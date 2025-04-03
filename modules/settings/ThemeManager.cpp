#include "ThemeManager.h"
#include "ThemeDefinition.h" // Enthält z.B. BlueThemeColors, NeonThemeColors, BlueThemeColorCount, NeonThemeColorCount
#include <algorithm>

BuiltinTheme ThemeManager::builtinThemeFromName(const std::string& name) const
{
    if (name == "Light") return BuiltinTheme::Light;
    if (name == "Dark")  return BuiltinTheme::Dark;
    if (name == "Blue")  return BuiltinTheme::Blue;
    return BuiltinTheme::Dark; // Fallback
}

void ThemeManager::applyTheme(const std::string& themeName)
{
    auto& themes = config["Themes"];
    if (!themes.contains(themeName)) {
        // Notfall: Theme nicht vorhanden -> Dark
        applyBuiltinTheme(BuiltinTheme::Dark);
        currentThemeName = "Dark";
        config["CurrentTheme"] = "Dark";
        return;
    }

    auto& themeObj = themes[themeName];
    if (!themeObj.contains("type")) {
        // Falls "type" fehlt -> fallback
        applyBuiltinTheme(BuiltinTheme::Dark);
        currentThemeName = "Dark";
        config["CurrentTheme"] = "Dark";
        return;
    }

    std::string t = themeObj["type"].get<std::string>();
    if (t == "builtin") {
        // Built-in Theme: Wandle den Namen in den entsprechenden Enum um und wende ihn an.
        BuiltinTheme bt = builtinThemeFromName(themeName);
        applyBuiltinTheme(bt);
    }
    else if (t == "custom") {
        // Custom Theme: Falls Farben definiert sind, wende diese an.
        if (themeObj.contains("colors")) {
            applyCustomTheme(themeObj["colors"]);
        } else {
            // Keine Farben definiert -> fallback auf Dark
            applyBuiltinTheme(BuiltinTheme::Dark);
            currentThemeName = "Dark";
            config["CurrentTheme"] = "Dark";
        }
    }
}

bool ThemeManager::isBuiltinTheme(const std::string& themeName) const
{
    static const std::vector<std::string> builtinNames = { "Light", "Dark", "Blue", "Neon" };
    return std::find(builtinNames.begin(), builtinNames.end(), themeName) != builtinNames.end();
}

void ThemeManager::applyBuiltinTheme(BuiltinTheme theme)
{
    ImGuiStyle &style = ImGui::GetStyle();
    switch (theme)
    {
    case BuiltinTheme::Light:
        ImGui::StyleColorsLight();
        break;
    case BuiltinTheme::Dark:
        ImGui::StyleColorsDark();
        break;
    case BuiltinTheme::Blue:
    {
        // Hier könntest du alternativ auch die Array-Definition verwenden:
        for (size_t i = 0; i < BlueThemeColorCount; ++i) {
            style.Colors[BlueThemeColors[i].index] = BlueThemeColors[i].color;
        }
        break;
    }
    case BuiltinTheme::Neon:
    {
        for (size_t i = 0; i < NeonThemeColorCount; ++i) {
            style.Colors[NeonThemeColors[i].index] = NeonThemeColors[i].color;
        }
        break;
    }
    }
}

void ThemeManager::applyCustomTheme(const nlohmann::json& colorData)
{
    ImGuiStyle &style = ImGui::GetStyle();
    for (auto it = colorData.begin(); it != colorData.end(); ++it)
    {
        std::string colorName = it.key();
        if (!it->is_array() || it->size() < 4) {
            continue;
        }
        float r = (*it)[0].get<float>();
        float g = (*it)[1].get<float>();
        float b = (*it)[2].get<float>();
        float a = (*it)[3].get<float>();
        ImVec4 color(r, g, b, a);

        if (colorName == "ImGuiCol_WindowBg")
            style.Colors[ImGuiCol_WindowBg] = color;
        else if (colorName == "ImGuiCol_Button")
            style.Colors[ImGuiCol_Button] = color;
        else if (colorName == "ImGuiCol_ButtonHovered")
            style.Colors[ImGuiCol_ButtonHovered] = color;
        else if (colorName == "ImGuiCol_ButtonActive")
            style.Colors[ImGuiCol_ButtonActive] = color;
        // Weitere Schlüssel kannst du hier ergänzen.
    }
}

ThemeManager::ThemeManager(const std::string& configPath)
    : configHandler(configPath)
{
    if (!configHandler.load()) {
        nlohmann::json defaultJson;
        defaultJson["Themes"] = nlohmann::json::object({
            {"Light", {{"type", "builtin"}}},
            {"Dark",  {{"type", "builtin"}}},
            {"Blue",  {{"type", "builtin"}}},
            {"Neon",  {{"type", "builtin"}}}
        });
        defaultJson["CurrentTheme"] = "Dark";
        configHandler.setConfig(defaultJson);
        configHandler.save();
    }
    loadFromConfig();
}

void ThemeManager::loadFromConfig()
{
    configHandler.load();
    config = configHandler.getConfig();
    if (!config.contains("Themes")) {
        config["Themes"] = nlohmann::json::object();
    }
    if (!config.contains("CurrentTheme")) {
        config["CurrentTheme"] = "Dark";
    }
    currentThemeName = config["CurrentTheme"].get<std::string>();
    applyTheme(currentThemeName);
}

void ThemeManager::saveToConfig()
{
    configHandler.setConfig(config);
    configHandler.save();
}

std::string ThemeManager::getCurrentThemeName() const
{
    return currentThemeName;
}

std::vector<std::string> ThemeManager::getAllThemeNames() const
{
    std::vector<std::string> names;
    if (config.contains("Themes") && config["Themes"].is_object()) {
        for (auto it = config["Themes"].begin(); it != config["Themes"].end(); ++it) {
            names.push_back(it.key());
        }
    }
    return names;
}

std::vector<std::string> ThemeManager::getCustomThemeNames() const
{
    std::vector<std::string> customNames;
    if (config.contains("Themes") && config["Themes"].is_object()) {
        for (auto it = config["Themes"].begin(); it != config["Themes"].end(); ++it) {
            if (it.value().contains("type") && it.value()["type"].get<std::string>() == "custom")
                customNames.push_back(it.key());
        }
    }
    return customNames;
}

void ThemeManager::setCurrentTheme(const std::string& themeName)
{
    if (!config["Themes"].contains(themeName) &&
        (themeName == "Light" || themeName == "Dark" || themeName == "Blue" || themeName == "Neon"))
    {
        config["Themes"][themeName] = { {"type", "builtin"} };
    }
    if (config["Themes"].contains(themeName))
        currentThemeName = themeName;
    else
        currentThemeName = "Dark";
    config["CurrentTheme"] = currentThemeName;
    applyTheme(currentThemeName);
    saveToConfig();
}

bool ThemeManager::deleteTheme(const std::string& themeName)
{
    if (!config["Themes"].contains(themeName))
        return false;
    config["Themes"].erase(themeName);
    if (currentThemeName == themeName) {
        currentThemeName = "Dark";
        config["CurrentTheme"] = "Dark";
        applyTheme("Dark");
    }
    saveToConfig();
    return true;
}

void ThemeManager::createCustomTheme(const std::string& themeName)
{
    nlohmann::json colorData = {
        {"ImGuiCol_WindowBg",       {0.05f, 0.05f, 0.05f, 1.0f}},
        {"ImGuiCol_Button",         {0.2f,  0.5f,  0.2f,  1.0f}},
        {"ImGuiCol_ButtonHovered",  {0.3f,  0.6f,  0.3f,  1.0f}},
        {"ImGuiCol_ButtonActive",   {0.1f,  0.4f,  0.1f,  1.0f}}
    };
    config["Themes"][themeName] = {
        {"type", "custom"},
        {"colors", colorData}
    };
    saveToConfig();
}

void ThemeManager::updateCustomColor(const std::string& colorName, const ImVec4& color)
{
    if (!config["Themes"].contains(currentThemeName))
        return;
    auto& themeObj = config["Themes"][currentThemeName];
    if (themeObj.contains("type") && themeObj["type"].get<std::string>() == "custom") {
        if (!themeObj.contains("colors"))
            themeObj["colors"] = nlohmann::json::object();
        themeObj["colors"][colorName] = { color.x, color.y, color.z, color.w };
        saveToConfig();
    }
}

void ThemeManager::saveCurrentThemeAs(const std::string& themeName)
{
    ImGuiStyle& style = ImGui::GetStyle();
    nlohmann::json colorData;
    // Beispiel: Speichere hier einige Farben; erweitere nach Bedarf
    colorData["ImGuiCol_WindowBg"] = { style.Colors[ImGuiCol_WindowBg].x, style.Colors[ImGuiCol_WindowBg].y, style.Colors[ImGuiCol_WindowBg].z, style.Colors[ImGuiCol_WindowBg].w };
    colorData["ImGuiCol_Button"] = { style.Colors[ImGuiCol_Button].x, style.Colors[ImGuiCol_Button].y, style.Colors[ImGuiCol_Button].z, style.Colors[ImGuiCol_Button].w };
    colorData["ImGuiCol_ButtonHovered"] = { style.Colors[ImGuiCol_ButtonHovered].x, style.Colors[ImGuiCol_ButtonHovered].y, style.Colors[ImGuiCol_ButtonHovered].z, style.Colors[ImGuiCol_ButtonHovered].w };
    colorData["ImGuiCol_ButtonActive"] = { style.Colors[ImGuiCol_ButtonActive].x, style.Colors[ImGuiCol_ButtonActive].y, style.Colors[ImGuiCol_ButtonActive].z, style.Colors[ImGuiCol_ButtonActive].w };
    // Füge weitere Einträge hinzu, wie benötigt

    config["Themes"][themeName] = {
        {"type", "custom"},
        {"colors", colorData}
    };
    saveToConfig();
}
