#include "ThemeManager.h"
#include "ThemeDefinition.h"
#include "Logger.h" // <<< Logging eingebunden
#include <algorithm>

BuiltinTheme ThemeManager::builtinThemeFromName(const std::string &name) const
{
    if (name == "Light")
        return BuiltinTheme::Light;
    if (name == "Dark")
        return BuiltinTheme::Dark;
    if (name == "Blue")
        return BuiltinTheme::Blue;
    return BuiltinTheme::Dark; // Fallback
}

void ThemeManager::applyTheme(const std::string &themeName)
{
    auto &themes = config["Themes"];
    if (!themes.contains(themeName))
    {
        Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                               "Theme '" + themeName + "' not found. Falling back to 'Dark'.",
                               __FUNCTION__, __FILE__, __LINE__);
        applyBuiltinTheme(BuiltinTheme::Dark);
        currentThemeName = "Dark";
        config["CurrentTheme"] = "Dark";
        return;
    }

    auto &themeObj = themes[themeName];
    if (!themeObj.contains("type"))
    {
        Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                               "Theme '" + themeName + "' has no type. Falling back to 'Dark'.",
                               __FUNCTION__, __FILE__, __LINE__);
        applyBuiltinTheme(BuiltinTheme::Dark);
        currentThemeName = "Dark";
        config["CurrentTheme"] = "Dark";
        return;
    }

    std::string t = themeObj["type"].get<std::string>();
    if (t == "builtin")
    {
        BuiltinTheme bt = builtinThemeFromName(themeName);
        applyBuiltinTheme(bt);
    }
    else if (t == "custom")
    {
        if (themeObj.contains("colors"))
        {
            applyCustomTheme(themeObj["colors"]);
        }
        else
        {
            Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                                   "Custom theme '" + themeName + "' has no colors defined. Falling back to 'Dark'.",
                                   __FUNCTION__, __FILE__, __LINE__);
            applyBuiltinTheme(BuiltinTheme::Dark);
            currentThemeName = "Dark";
            config["CurrentTheme"] = "Dark";
            return;
        }
    }

    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                           "Applied theme: " + themeName,
                           __FUNCTION__, __FILE__, __LINE__);
}

bool ThemeManager::isBuiltinTheme(const std::string &themeName) const
{
    static const std::vector<std::string> builtinNames = {"Light", "Dark", "Blue", "Neon"};
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
        for (size_t i = 0; i < BlueThemeColorCount; ++i)
        {
            style.Colors[BlueThemeColors[i].index] = BlueThemeColors[i].color;
        }
        break;
    case BuiltinTheme::Neon:
        for (size_t i = 0; i < NeonThemeColorCount; ++i)
        {
            style.Colors[NeonThemeColors[i].index] = NeonThemeColors[i].color;
        }
        break;
    }
}

void ThemeManager::applyCustomTheme(const nlohmann::json &colorData)
{
    ImGuiStyle &style = ImGui::GetStyle();
    for (auto it = colorData.begin(); it != colorData.end(); ++it)
    {
        std::string colorName = it.key();
        if (!it->is_array() || it->size() < 4)
        {
            Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                                   "Invalid color array for key: " + colorName,
                                   __FUNCTION__, __FILE__, __LINE__);
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
        // Weitere Farbschlüssel bei Bedarf ergänzen
    }
}

ThemeManager::ThemeManager(const std::string &configPath)
    : configHandler(configPath)
{
    if (!configHandler.load())
    {
        Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                               "Failed to load config. Creating default configuration.",
                               __FUNCTION__, __FILE__, __LINE__);

        ensureDefaultConfig();
    }

    loadFromConfig();
}

void ThemeManager::loadFromConfig()
{
    bool loaded = configHandler.load();
    config = configHandler.getConfig();

    bool configInvalid = !loaded || config.is_null() || !config.is_object();

    if (configInvalid || !config.contains("Themes") || !config.contains("CurrentTheme"))
    {
        Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                               "Invalid or incomplete config. Replacing with default configuration.",
                               __FUNCTION__, __FILE__, __LINE__);

        ensureDefaultConfig();
    }

    currentThemeName = config["CurrentTheme"].get<std::string>();
    applyTheme(currentThemeName);
}

void ThemeManager::saveToConfig()
{
    configHandler.setConfig(config);
    configHandler.save();
    Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_CONFIG,
                           "Configuration saved.",
                           __FUNCTION__, __FILE__, __LINE__);
}

std::string ThemeManager::getCurrentThemeName() const
{
    return currentThemeName;
}

std::vector<std::string> ThemeManager::getAllThemeNames() const
{
    std::vector<std::string> names;
    if (config.contains("Themes") && config["Themes"].is_object())
    {
        for (auto it = config["Themes"].begin(); it != config["Themes"].end(); ++it)
        {
            names.push_back(it.key());
        }
    }
    return names;
}

std::vector<std::string> ThemeManager::getCustomThemeNames() const
{
    std::vector<std::string> customNames;
    if (config.contains("Themes") && config["Themes"].is_object())
    {
        for (auto it = config["Themes"].begin(); it != config["Themes"].end(); ++it)
        {
            if (it.value().contains("type") && it.value()["type"].get<std::string>() == "custom")
                customNames.push_back(it.key());
        }
    }
    return customNames;
}

void ThemeManager::setCurrentTheme(const std::string &themeName)
{
    if (!config["Themes"].contains(themeName) &&
        (themeName == "Light" || themeName == "Dark" || themeName == "Blue" || themeName == "Neon"))
    {
        config["Themes"][themeName] = {{"type", "builtin"}};
    }

    if (config["Themes"].contains(themeName))
        currentThemeName = themeName;
    else
        currentThemeName = "Dark";

    config["CurrentTheme"] = currentThemeName;
    applyTheme(currentThemeName);
    saveToConfig();

    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                           "Set current theme: " + currentThemeName,
                           __FUNCTION__, __FILE__, __LINE__);
}

bool ThemeManager::deleteTheme(const std::string &themeName)
{
    if (!config["Themes"].contains(themeName))
    {
        Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_CONFIG,
                               "Attempted to delete non-existent theme: " + themeName,
                               __FUNCTION__, __FILE__, __LINE__);
        return false;
    }

    config["Themes"].erase(themeName);
    if (currentThemeName == themeName)
    {
        currentThemeName = "Dark";
        config["CurrentTheme"] = "Dark";
        applyTheme("Dark");
        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                               "Deleted current theme. Fallback to 'Dark'.",
                               __FUNCTION__, __FILE__, __LINE__);
    }
    else
    {
        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                               "Deleted theme: " + themeName,
                               __FUNCTION__, __FILE__, __LINE__);
    }

    saveToConfig();
    return true;
}

void ThemeManager::createCustomTheme(const std::string &themeName)
{
    nlohmann::json colorData = {
        {"ImGuiCol_WindowBg", {0.05f, 0.05f, 0.05f, 1.0f}},
        {"ImGuiCol_Button", {0.2f, 0.5f, 0.2f, 1.0f}},
        {"ImGuiCol_ButtonHovered", {0.3f, 0.6f, 0.3f, 1.0f}},
        {"ImGuiCol_ButtonActive", {0.1f, 0.4f, 0.1f, 1.0f}}};
    config["Themes"][themeName] = {
        {"type", "custom"},
        {"colors", colorData}};
    saveToConfig();

    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                           "Created new custom theme: " + themeName,
                           __FUNCTION__, __FILE__, __LINE__);
}

void ThemeManager::updateCustomColor(const std::string &colorName, const ImVec4 &color)
{
    if (!config["Themes"].contains(currentThemeName))
        return;

    auto &themeObj = config["Themes"][currentThemeName];
    if (themeObj.contains("type") && themeObj["type"].get<std::string>() == "custom")
    {
        if (!themeObj.contains("colors"))
            themeObj["colors"] = nlohmann::json::object();

        themeObj["colors"][colorName] = {color.x, color.y, color.z, color.w};
        saveToConfig();

        Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_CONFIG,
                               "Updated color '" + colorName + "' in theme: " + currentThemeName,
                               __FUNCTION__, __FILE__, __LINE__);
    }
}

void ThemeManager::saveCurrentThemeAs(const std::string &themeName)
{
    ImGuiStyle &style = ImGui::GetStyle();
    nlohmann::json colorData;
    colorData["ImGuiCol_WindowBg"] = {style.Colors[ImGuiCol_WindowBg].x, style.Colors[ImGuiCol_WindowBg].y, style.Colors[ImGuiCol_WindowBg].z, style.Colors[ImGuiCol_WindowBg].w};
    colorData["ImGuiCol_Button"] = {style.Colors[ImGuiCol_Button].x, style.Colors[ImGuiCol_Button].y, style.Colors[ImGuiCol_Button].z, style.Colors[ImGuiCol_Button].w};
    colorData["ImGuiCol_ButtonHovered"] = {style.Colors[ImGuiCol_ButtonHovered].x, style.Colors[ImGuiCol_ButtonHovered].y, style.Colors[ImGuiCol_ButtonHovered].z, style.Colors[ImGuiCol_ButtonHovered].w};
    colorData["ImGuiCol_ButtonActive"] = {style.Colors[ImGuiCol_ButtonActive].x, style.Colors[ImGuiCol_ButtonActive].y, style.Colors[ImGuiCol_ButtonActive].z, style.Colors[ImGuiCol_ButtonActive].w};

    config["Themes"][themeName] = {
        {"type", "custom"},
        {"colors", colorData}};
    saveToConfig();

    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                           "Saved current theme as: " + themeName,
                           __FUNCTION__, __FILE__, __LINE__);
}

void ThemeManager::ensureDefaultConfig()
{
    config = nlohmann::json{
        {"Themes", {{"Light", {{"type", "builtin"}}}, {"Dark", {{"type", "builtin"}}}, {"Blue", {{"type", "builtin"}}}, {"Neon", {{"type", "builtin"}}}}},
        {"CurrentTheme", "Dark"}};
    configHandler.setConfig(config);
    configHandler.save();

    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_CONFIG,
                           "Default configuration created.",
                           __FUNCTION__, __FILE__, __LINE__);
}
