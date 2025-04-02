#pragma once

#include <string>
#include "json/json.hpp"

class ConfigHandler
{
public:
    // Konstruktor erwartet den Dateinamen (z.B. "config.json")
    // Die Datei wird dann im AppData\UtilsApp-Ordner abgelegt (auf Windows)
    explicit ConfigHandler(const std::string& configFileName);

    // Lädt die Konfiguration aus der Datei
    bool load();

    // Speichert die aktuelle Konfiguration in die Datei
    bool save();

    // Gibt einen modifizierbaren Zugriff auf die Konfigurationsdaten (JSON)
    nlohmann::json& getConfig();

    // Gibt einen nur-lesenden Zugriff auf die Konfigurationsdaten (JSON)
    const nlohmann::json& getConfig() const;

    // Überschreibt die Konfiguration mit dem übergebenen JSON-Objekt
    void setConfig(const nlohmann::json& config);

private:
    std::string path;            // Vollständiger Pfad zur Konfigurationsdatei
    nlohmann::json configData;   // Hier werden die Konfigurationsdaten gespeichert
};
