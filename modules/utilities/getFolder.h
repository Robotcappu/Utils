// GetFolder.h
#pragma once

#include <string>

namespace GetFolder
{
    // Öffnet einen Dialog zur Auswahl eines Ordners
    // Gibt leeren String zurück bei Abbruch
    std::string openDialog();

    // Validiert, ob ein Pfad existiert und ein Ordner ist
    bool isValidDirectory(const std::string& path);

    // Prüft, ob ein Pfad eine Datei ist (nicht Ordner)
    bool isFilePath(const std::string& path);

    // Gibt den übergeordneten Ordner einer Datei zurück
    std::string getContainingFolder(const std::string& filePath);
} // namespace GetFolder