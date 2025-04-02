
#include "gui.h"
#include <thread>

int __stdcall wWinMain(
    HINSTANCE hinstance,
    HINSTANCE hprevicousInstance,
    PWSTR lpCmdLine,
    int nCmdShow)
{
    gui::CreateHWindow(L"Util", L"Util Classs");
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::exit)
    {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}
