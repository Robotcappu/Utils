// PopupHandler.h
#pragma once

#include <string>
#include <functional>

namespace PopupHandler
{
    // Typalias Callback: Bestätigung = True, Abbruch = False
    using PopupCallback = std::function<void(bool)>;

    // Öffnet Popup mit ID und Bestätigung
    void openConfirmationPopup(const std::string& popupID, const std::string& message, PopupCallback callback);

    // Render um Popups zu verarbeiten
    void render();
}
