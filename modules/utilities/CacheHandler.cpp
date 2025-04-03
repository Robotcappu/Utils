// CacheHandler.cpp
#include "CacheHandler.h"
#include "modules/core/Logger.h"
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

namespace CacheHandler
{
    static std::string getCacheDir()
    {
#ifdef _WIN32
        const char* appData = std::getenv("APPDATA");
        if (appData)
        {
            std::string basePath = std::string(appData) + "\\UtilsApp\\cache";
            fs::create_directories(basePath);
            return basePath;
        }
#endif
        std::string fallback = "cache";
        fs::create_directories(fallback);
        return fallback;
    }

    static std::string getCacheFilePath(const std::string& cacheKey)
    {
        return getCacheDir() + "/" + cacheKey + ".json";
    }

    bool loadCache(const std::string& cacheKey, nlohmann::json& outData)
    {
        std::ifstream inFile(getCacheFilePath(cacheKey));
        if (!inFile.is_open())
        {
            Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_FILES,
                "Konnte Cache-Datei nicht öffnen: " + cacheKey, __func__, __FILE__, __LINE__);
            return false;
        }

        try
        {
            inFile >> outData;
            Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_FILES,
                "Cache erfolgreich geladen: " + cacheKey, __func__, __FILE__, __LINE__);
        }
        catch (...)
        {
            Logger::instance().log(LogLevel::LOG_ERROR, LogCategory::LOG_FILES,
                "Fehler beim Parsen der Cache-Datei: " + cacheKey, __func__, __FILE__, __LINE__);
            return false;
        }
        return true;
    }

    bool saveCache(const std::string& cacheKey, const nlohmann::json& data)
    {
        std::ofstream outFile(getCacheFilePath(cacheKey));
        if (!outFile.is_open())
        {
            Logger::instance().log(LogLevel::LOG_WARNING, LogCategory::LOG_FILES,
                "Konnte Cache-Datei nicht speichern: " + cacheKey, __func__, __FILE__, __LINE__);
            return false;
        }

        try
        {
            outFile << data.dump(4);
            Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_FILES,
                "Cache erfolgreich gespeichert: " + cacheKey, __func__, __FILE__, __LINE__);
        }
        catch (...)
        {
            Logger::instance().log(LogLevel::LOG_ERROR, LogCategory::LOG_FILES,
                "Fehler beim Schreiben der Cache-Datei: " + cacheKey, __func__, __FILE__, __LINE__);
            return false;
        }
        return true;
    }

    bool deleteCache(const std::string& cacheKey)
    {
        bool success = fs::remove(getCacheFilePath(cacheKey));
        Logger::instance().log(success ? LogLevel::LOG_INFO : LogLevel::LOG_WARNING,
            LogCategory::LOG_FILES,
            (success ? "Cache gelöscht: " : "Löschen fehlgeschlagen für Cache: ") + cacheKey,
            __func__, __FILE__, __LINE__);
        return success;
    }
} // namespace CacheHandler
