#include "ConfigHandler.h"
#include <fstream>
#include <cstdlib>
#ifdef _WIN32
#include <direct.h>   // _mkdir
#include <sys/stat.h>
#endif

#ifdef _WIN32
// Hilfsfunktion: Prüft, ob ein Verzeichnis existiert.
static bool directoryExists(const std::string& dirName)
{
    struct _stat info;
    if (_stat(dirName.c_str(), &info) != 0)
        return false;
    return (info.st_mode & _S_IFDIR) != 0;
}
#endif

ConfigHandler::ConfigHandler(const std::string& configFileName)
{
#ifdef _WIN32
    // Hole den APPDATA-Pfad aus der Umgebungsvariable
    const char* appData = std::getenv("APPDATA");
    if (appData)
    {
        std::string appDataPath(appData);
        std::string dir = appDataPath + "\\UtilsApp";
        // Falls der Ordner nicht existiert, erstellen
        if (!directoryExists(dir))
        {
            _mkdir(dir.c_str());
        }
        // Setze den Pfad zur Konfigurationsdatei im UtilsApp-Ordner
        path = dir + "\\" + configFileName;
    }
    else
    {
        // Fallback: Benutze den übergebenen Dateinamen
        path = configFileName;
    }
#else
    // Auf anderen Plattformen einfach den übergebenen Pfad verwenden
    path = configFileName;
#endif

    // Falls die Datei nicht existiert, lege ein Default-JSON-Objekt an und speichere es
    std::ifstream testFile(path);
    if (!testFile.good())
    {
        configData = nlohmann::json::object();  // Leere Default-Konfiguration
        save();
    }
}

bool ConfigHandler::load()
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    try
    {
        file >> configData;
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool ConfigHandler::save()
{
    std::ofstream file(path);
    if (!file.is_open())
        return false;

    try
    {
        // Speichert die Konfiguration formatiert (4 Leerzeichen pro Einrückung)
        file << configData.dump(4);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

nlohmann::json& ConfigHandler::getConfig()
{
    return configData;
}

const nlohmann::json& ConfigHandler::getConfig() const
{
    return configData;
}

void ConfigHandler::setConfig(const nlohmann::json& config)
{
    configData = config;
}
