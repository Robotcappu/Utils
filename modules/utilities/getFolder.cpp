// GetFolder.cpp
#include "GetFolder.h"
#include "modules/core/Logger.h"
#include <windows.h>
#include <shobjidl.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace GetFolder
{
    std::string openDialog()
    {
        Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_GENERAL, "Öffne Ordnerauswahl-Dialog", __func__, __FILE__, __LINE__);

        std::string folderPath;
        IFileDialog* fileDialog = nullptr;

        if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&fileDialog))))
        {
            DWORD options;
            fileDialog->GetOptions(&options);
            fileDialog->SetOptions(options | FOS_PICKFOLDERS);

            if (SUCCEEDED(fileDialog->Show(NULL)))
            {
                IShellItem* item;
                if (SUCCEEDED(fileDialog->GetResult(&item)))
                {
                    PWSTR path = nullptr;
                    if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)))
                    {
                        char buffer[MAX_PATH];
                        wcstombs(buffer, path, MAX_PATH);
                        folderPath = buffer;
                        Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_GENERAL, "Ordner ausgewählt: " + folderPath, __func__, __FILE__, __LINE__);
                        CoTaskMemFree(path);
                    }
                    item->Release();
                }
            }
            fileDialog->Release();
        }

        if (folderPath.empty())
        {
            Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_GENERAL, "Ordnerauswahl abgebrochen oder ungültig", __func__, __FILE__, __LINE__);
        }

        return folderPath;
    }

    bool isValidDirectory(const std::string& path)
    {
        bool valid = fs::exists(path) && fs::is_directory(path);
        Logger::instance().log(valid ? LogLevel::LOG_DEBUG : LogLevel::LOG_WARNING,
            LogCategory::LOG_GENERAL,
            (valid ? "Pfad ist gültiger Ordner: " : "Ungültiger Ordnerpfad: ") + path,
            __func__, __FILE__, __LINE__);
        return valid;
    }

    bool isFilePath(const std::string& path)
    {
        bool isFile = fs::exists(path) && fs::is_regular_file(path);
        Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_GENERAL,
            (isFile ? "Pfad ist Datei: " : "Pfad ist keine Datei: ") + path,
            __func__, __FILE__, __LINE__);
        return isFile;
    }

    std::string getContainingFolder(const std::string& filePath)
    {
        std::string folder = fs::path(filePath).parent_path().string();
        Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_GENERAL, "Übergeordneter Ordner: " + folder, __func__, __FILE__, __LINE__);
        return folder;
    }
} // namespace GetFolder