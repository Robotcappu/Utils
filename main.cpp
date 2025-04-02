// main.cpp
#include "gui.h"
#include "Globals.h"
#include <thread>

ThemeManager* gThemeManager = nullptr;

int __stdcall wWinMain(
    HINSTANCE hinstance,
    HINSTANCE hprevicousInstance,
    PWSTR lpCmdLine,
    int nCmdShow)
{
    gui::CreateHWindow(L"Util", L"Util Classs");
    gui::CreateDevice();
    gui::CreateImGui();

    gThemeManager = new ThemeManager("config.json");
    gThemeManager->loadFromConfig();

    while (gui::exit)
    {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    delete gThemeManager;
    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}
