
#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "modules/core/Logger.h"

#include <iostream>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <thread>
#include <tchar.h>
#include <thread>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter);

LRESULT __stdcall WindowProcess(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
    case WM_SIZE:
    {
        if (gui::device && wideParameter != SIZE_MINIMIZED)
        {
            gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
            gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
            gui::ResetDevice();
        }
    }
        return 0;

    case WM_SYSCOMMAND:
    {
        if ((wideParameter & 0xfff0) == SC_KEYMENU)
            return 0;
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
        return 0;

    case WM_LBUTTONDOWN:
    {
        gui::position = MAKEPOINTS(longParameter);
    }

    case WM_MOUSEMOVE:
    {
        if (wideParameter == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(longParameter);
            auto rect = ::RECT{};

            GetWindowRect(gui::window, &rect);

            rect.left += points.x - gui::position.x;
            rect.top += points.y - gui::position.y;

            if (gui::position.x >= 0 &&
                gui::position.x <= gui::WIDTH &&
                gui::position.y >= 0 && gui::position.y <= 19)
            {
                SetWindowPos(
                    gui::window,
                    HWND_TOPMOST,
                    rect.left,
                    rect.top,
                    0, 0,
                    SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
            }
        }
        return 0;
    }
    }

    return DefWindowProcW(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(
    const wchar_t *windowName,
    const wchar_t *className) noexcept
{
    WNDCLASSEXW windowClass = {};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WindowProcess;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandleW(NULL);
    windowClass.hIcon = 0;
    windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = className;
    windowClass.hIconSm = 0;

    if (!RegisterClassExW(&windowClass))
    {
        MessageBoxW(NULL, L"Fensterklasse konnte nicht registriert werden.", L"Fehler", MB_ICONERROR);
        return;
    }

    window = CreateWindowW(
        className,
        windowName,
        WS_POPUP,
        50,
        50,
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        windowClass.hInstance,
        NULL);

    if (!window)
    {
        MessageBoxW(NULL, L"Fenster konnte nicht erstellt werden.", L"Fehler", MB_ICONERROR);
        return;
    }

    if (!window)
    {
        MessageBoxW(NULL, L"Fenster konnte nicht erstellt werden.", L"Fehler", MB_ICONERROR);
        return;
    }

    // Nachdem das Fenster erfolgreich erstellt wurde, also direkt vor ShowWindow/UpdateWindow:
    LONG exStyle = GetWindowLong(window, GWL_EXSTYLE);
    SetWindowLong(window, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);

    // Setze den Farbschlüssel: Alle Pixel in Schwarz (0,0,0) werden transparent
    SetLayeredWindowAttributes(window, RGB(0, 0, 0), 0, LWA_COLORKEY);

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
    if (window)
    {
        DestroyWindow(window);
        window = nullptr;
    }
    UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (d3d->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            window,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentParameters,
            &device) < 0)
        return false;

    return true;
}

void gui::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (d3d)
    {
        d3d->Release();
        d3d = nullptr;
    }
}

void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    // Stil und Farbschema anpassen
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 8.f; // Eckenrundung für Fenster
    style.FrameRounding = 8.f;  // Eckenrundung für Buttons, Eingabefelder etc.
    style.TabRounding = 8.f;
    style.GrabRounding = 8.f;
    style.PopupRounding = 8.f;
    style.ItemSpacing = ImVec2(10, 5); // Abstand zwischen Elementen

    // Farbschema anpassen
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);       // Hintergrundfarbe von Fenstern
    style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.25f, 0.3f, 1.0f);        // Button-Hintergrund
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.35f, 0.4f, 1.0f); // Button-Hintergrund beim Hovern
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.2f, 0.25f, 1.0f); // Button-Hintergrund beim Klicken

    // ImGui für die Verwendung mit DirectX und Win32 initialisieren
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

    io.IniFilename = NULL;
}

void gui::DestroyImGui() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    // Start the Dear ImGui frame
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
    ImGui::EndFrame();

    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    const auto result = device->Present(0, 0, 0, 0);

    // Handle loss of D3D9 device
    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

void gui::Render() noexcept
{
    RECT clientRect;
    GetClientRect(window, &clientRect);
    float currentWidth = static_cast<float>(clientRect.right - clientRect.left);
    float currentHeight = static_cast<float>(clientRect.bottom - clientRect.top);

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({currentWidth, currentHeight});
    ImGui::Begin("SauberZauber", &exit, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::BeginTabBar("MainTabBar"))
    {
        // Tab für Reinigungsvorgänge
        if (ImGui::BeginTabItem("Menu"))
        {
            ImGui::Separator();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Templates & Einstellungen"))
        {
            if (ImGui::CollapsingHeader("Templates"))
            {
                ImGui::Text("Change the look of the Window");
            }
            // Einklappbarer Bereich für die Log-Dateien
            if (ImGui::CollapsingHeader("Log Dateien"))
            {
                static int currentLevel = 0; // 0: DEBUG, 1: INFO, 2: WARNING, 3: ERROR
                const char *levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

                if (ImGui::Combo("Log Level", &currentLevel, levels, IM_ARRAYSIZE(levels)))
                {
                    switch (currentLevel)
                    {
                    case 0:
                        Logger::instance().setMinLogLevel(LogLevel::LOG_DEBUG);
                        break;
                    case 1:
                        Logger::instance().setMinLogLevel(LogLevel::LOG_INFO);
                        break;
                    case 2:
                        Logger::instance().setMinLogLevel(LogLevel::LOG_WARNING);
                        break;
                    case 3:
                        Logger::instance().setMinLogLevel(LogLevel::LOG_ERROR);
                        break;
                    }
                }

                // Checkboxen für die einzelnen Log-Kategorien
                static bool enabledGeneral = true, enabledSystem = true, enabledFiles = true, enabledCleaning = true, enabledConfig = true;
                if (ImGui::Checkbox("GENERAL", &enabledGeneral))
                    Logger::instance().enableCategory(LogCategory::LOG_GENERAL, enabledGeneral);
                if (ImGui::Checkbox("SYSTEM", &enabledSystem))
                    Logger::instance().enableCategory(LogCategory::LOG_SYSTEM, enabledSystem);
                if (ImGui::Checkbox("FILES", &enabledFiles))
                    Logger::instance().enableCategory(LogCategory::LOG_FILES, enabledFiles);
                if (ImGui::Checkbox("CLEANING", &enabledCleaning))
                    Logger::instance().enableCategory(LogCategory::LOG_CLEANING, enabledCleaning);
                if (ImGui::Checkbox("CONFIG", &enabledConfig))
                    Logger::instance().enableCategory(LogCategory::LOG_CONFIG, enabledConfig);
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
