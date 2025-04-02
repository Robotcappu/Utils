#pragma once

#include <string>
#include <vector>
#include "modules/core/ConfigHandler.h"
#include "imgui/imgui.h"
#include "json/json.hpp"

// Built-in Themes
enum class BuiltinTheme
{
    Light,
    Dark,
    Blue,
    Neon
};

/*
 * Der ThemeManager verwaltet sowohl eingebaute (Built-in) Themes
 * als auch benutzerdefinierte Themes (Custom).
 * Alles wird in config["Themes"] gespeichert.
 * Außerdem merkt er sich in config["CurrentTheme"] das aktuell genutzte Theme.
 */
class ThemeManager
{
public:
    // Konstruktor mit Pfad zur Konfigurationsdatei
    explicit ThemeManager(const std::string& configPath);

    // Lädt die Konfiguration und wendet das aktuelle Theme an
    void loadFromConfig();

    // Speichert die Konfiguration (z.B. nach Änderungen)
    void saveToConfig();

    // Gibt den Namen des aktuell ausgewählten Themes zurück
    std::string getCurrentThemeName() const;

    // Setzt das aktuelle Theme anhand seines Namens (z.B. "Dark", "Blue", "MeinEigenesTheme")
    // Wenn es nicht existiert, wird automatisch auf "Dark" gewechselt
    void setCurrentTheme(const std::string& themeName);

    // Gibt eine Liste aller Theme-Namen zurück (Built-in + Custom)
    std::vector<std::string> getAllThemeNames() const;

    // Gibt eine Liste aller benutzerdefinierten Themes zurück
    std::vector<std::string> getCustomThemeNames() const;

    // Legt ein neues Custom-Theme an (mit Beispielwerten); überschreibt falls Name schon existiert
    void createCustomTheme(const std::string& themeName);

    // Löscht ein Theme. Wenn das aktuell ausgewählte Theme gelöscht wird, wechselt es zu "Dark".
    // Gibt true zurück, wenn gelöscht wurde; false, wenn das Theme gar nicht existierte.
    bool deleteTheme(const std::string& themeName);

    // Aktualisiert eine Farbe im aktuell ausgewählten Theme (nur für Custom Themes).
    // colorName entspricht dabei dem Schlüssel (z.B. "ImGuiCol_WindowBg"),
    // color sind die neuen RGBA-Werte.
    void updateCustomColor(const std::string& colorName, const ImVec4& color);

    // Speichert den aktuellen ImGui-Style als Custom Theme unter dem übergebenen Namen
    void saveCurrentThemeAs(const std::string& themeName);

private:
    // Wendet ein Theme an (unterscheidet zwischen Built-in und Custom).
    void applyTheme(const std::string& themeName);

    // Prüft, ob es sich um ein Built-in Theme handelt
    bool isBuiltinTheme(const std::string& themeName) const;

    // Wendet ein Built-in Theme an (Light, Dark, Blue, Neon)
    void applyBuiltinTheme(BuiltinTheme theme);

    // Wendet ein Custom-Theme an (Farben aus JSON)
    void applyCustomTheme(const nlohmann::json& colorData);

    // Gibt den enum-Wert für ein Built-in Theme aus dem Namen zurück (z.B. "Dark" -> BuiltinTheme::Dark)
    BuiltinTheme builtinThemeFromName(const std::string& name) const;

private:
    ConfigHandler configHandler;  // zum Laden/Speichern der JSON
    nlohmann::json config;        // lokale Kopie der JSON-Daten
    std::string currentThemeName; // aktuell gewähltes Theme
};
