#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <set>

// Log-Level
enum class LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

// Log-Kategorien
enum class LogCategory {
    LOG_GENERAL,
    LOG_SYSTEM,
    LOG_FILES,
    LOG_CLEANING,
    LOG_CONFIG
};

class Logger {
public:
    // Singleton-Zugriff
    static Logger& instance();

    // Loggt eine Nachricht im JSON-Format
    void log(LogLevel level, LogCategory category, const std::string &message,
             const std::string &functionName, const std::string &fileName, int line);

    // Log-Datei ändern
    void setLogFile(const std::string &filePath);

    // Minimalen Log-Level setzen (alle Einträge unter diesem Level werden ignoriert)
    void setMinLogLevel(LogLevel level);

    // Aktiviert oder deaktiviert eine bestimmte Kategorie
    void enableCategory(LogCategory category, bool enable);

private:
    Logger();  // Konstruktor
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream logFile;
    std::mutex logMutex;

    // Konfigurierbare Parameter
    LogLevel m_minLogLevel = LogLevel::LOG_DEBUG; // Standard: alle Logs
    std::set<LogCategory> m_enabledCategories = {
        LogCategory::LOG_GENERAL,
        LogCategory::LOG_SYSTEM,
        LogCategory::LOG_FILES,
        LogCategory::LOG_CLEANING,
        LogCategory::LOG_CONFIG
    };

    std::string getTimestamp();
    std::string logLevelToString(LogLevel level);
    std::string logCategoryToString(LogCategory category);
};

#endif // LOGGER_H
