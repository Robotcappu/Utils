// PopupHandler.cpp
#include "PopupHandler.h"
#include "imgui/imgui.h"
#include "modules/core/Logger.h"
#include <unordered_map>

namespace PopupHandler
{
    struct PopupData
    {
        std::string message;
        PopupCallback callback;
        bool isOpen = true;
    };

    static std::unordered_map<std::string, PopupData> activePopups;

    void openConfirmationPopup(const std::string& popupID, const std::string& message, PopupCallback callback)
    {
        Logger::instance().log(LogLevel::LOG_DEBUG, LogCategory::LOG_GENERAL,
            "Popup geöffnet: " + popupID + " - " + message, __func__, __FILE__, __LINE__);

        activePopups[popupID] = {message, callback, true};
        ImGui::OpenPopup(popupID.c_str());
    }

    void render()
    {
        for (auto it = activePopups.begin(); it != activePopups.end();)
        {
            const std::string& popupID = it->first;
            PopupData& data = it->second;

            if (data.isOpen && ImGui::BeginPopupModal(popupID.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::TextWrapped("%s", data.message.c_str());
                ImGui::Separator();

                bool actionTaken = false;

                if (ImGui::Button("Ja", ImVec2(120, 0)))
                {
                    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_GENERAL,
                        "Popup bestätigt: " + popupID, __func__, __FILE__, __LINE__);

                    data.callback(true);
                    ImGui::CloseCurrentPopup();
                    data.isOpen = false;
                    actionTaken = true;
                }

                ImGui::SameLine();

                if (ImGui::Button("Nein", ImVec2(120, 0)))
                {
                    Logger::instance().log(LogLevel::LOG_INFO, LogCategory::LOG_GENERAL,
                        "Popup abgebrochen: " + popupID, __func__, __FILE__, __LINE__);

                    data.callback(false);
                    ImGui::CloseCurrentPopup();
                    data.isOpen = false;
                    actionTaken = true;
                }

                ImGui::EndPopup();

                if (actionTaken)
                {
                    it = activePopups.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
}
