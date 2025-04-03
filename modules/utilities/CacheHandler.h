// CacheHandler.h
#pragma once

#include <string>
#include "json/json.hpp"

namespace CacheHandler
{
    // Lädt JSON-Daten aus einer Cache-Datei
    bool loadCache(const std::string& cacheKey, nlohmann::json& outData);

    // Speichert JSON-Daten in einer Cache-Datei
    bool saveCache(const std::string& cacheKey, const nlohmann::json& data);

    // Entfernt eine bestimmte Cache-Datei (optional)
    bool deleteCache(const std::string& cacheKey);
} // namespace CacheHandler
