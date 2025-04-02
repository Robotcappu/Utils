// ThemeDefinitions.h
#pragma once
#include "imgui/imgui.h"

struct ThemeColor {
    ImGuiCol_ index;
    ImVec4 color;
};

// Blue-Theme Definition
static const ThemeColor BlueThemeColors[] = {
    { ImGuiCol_WindowBg, ImVec4(0.10f, 0.15f, 0.30f, 1.0f) },
    { ImGuiCol_ChildBg,  ImVec4(0.12f, 0.18f, 0.35f, 1.0f) },
    { ImGuiCol_PopupBg,  ImVec4(0.10f, 0.15f, 0.30f, 0.9f) },
    { ImGuiCol_Border,   ImVec4(0.2f,  0.3f,  0.6f,  1.0f) },
    { ImGuiCol_TitleBg,  ImVec4(0.09f, 0.14f, 0.28f, 1.0f) },
    { ImGuiCol_TitleBgActive, ImVec4(0.11f, 0.16f, 0.33f, 1.0f) },
    { ImGuiCol_TitleBgCollapsed, ImVec4(0.08f, 0.12f, 0.25f, 1.0f) },
    { ImGuiCol_Header,   ImVec4(0.2f,  0.35f, 0.7f,  1.0f) },
    { ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.45f, 0.75f, 1.0f) },
    { ImGuiCol_HeaderActive, ImVec4(0.15f, 0.3f,  0.65f, 1.0f) },
    { ImGuiCol_Button,   ImVec4(0.2f,  0.3f,  0.8f,  1.0f) },
    { ImGuiCol_ButtonHovered, ImVec4(0.3f,  0.4f,  0.9f,  1.0f) },
    { ImGuiCol_ButtonActive, ImVec4(0.15f, 0.25f, 0.6f,  1.0f) },
    { ImGuiCol_FrameBg,  ImVec4(0.12f, 0.18f, 0.35f, 1.0f) },
    { ImGuiCol_FrameBgHovered, ImVec4(0.18f, 0.25f, 0.45f, 1.0f) },
    { ImGuiCol_FrameBgActive, ImVec4(0.10f, 0.15f, 0.30f, 1.0f) },
    { ImGuiCol_Tab,      ImVec4(0.15f, 0.20f, 0.4f,  1.0f) },
    { ImGuiCol_TabHovered, ImVec4(0.2f,  0.3f,  0.7f,  1.0f) },
    { ImGuiCol_TabActive, ImVec4(0.1f,  0.25f, 0.6f,  1.0f) },
    { ImGuiCol_TabUnfocused, ImVec4(0.1f,  0.15f, 0.3f,  1.0f) },
    { ImGuiCol_TabUnfocusedActive, ImVec4(0.15f, 0.2f,  0.45f, 1.0f) },
    { ImGuiCol_SliderGrab, ImVec4(0.3f,  0.4f,  0.9f,  1.0f) },
    { ImGuiCol_SliderGrabActive, ImVec4(0.2f,  0.3f,  0.8f,  1.0f) },
    { ImGuiCol_ScrollbarBg, ImVec4(0.10f, 0.15f, 0.30f, 1.0f) },
    { ImGuiCol_ScrollbarGrab, ImVec4(0.2f,  0.3f,  0.7f,  1.0f) },
    { ImGuiCol_ScrollbarGrabHovered, ImVec4(0.25f, 0.35f, 0.8f, 1.0f) },
    { ImGuiCol_ScrollbarGrabActive, ImVec4(0.15f, 0.25f, 0.6f, 1.0f) },
    { ImGuiCol_Text,     ImVec4(1.0f,  1.0f,  1.0f,  1.0f) },
    { ImGuiCol_TextDisabled, ImVec4(0.5f, 0.5f, 0.5f, 1.0f) }
};

// Neon-Theme Definition
static const ThemeColor NeonThemeColors[] = {
    { ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f) },
    { ImGuiCol_ChildBg,  ImVec4(0.10f, 0.10f, 0.10f, 1.0f) },
    { ImGuiCol_PopupBg,  ImVec4(0.08f, 0.08f, 0.08f, 0.9f) },
    { ImGuiCol_Border,   ImVec4(0.1f,  0.1f,  0.1f,  1.0f) },
    { ImGuiCol_TitleBg,  ImVec4(0.05f, 0.05f, 0.05f, 1.0f) },
    { ImGuiCol_TitleBgActive, ImVec4(0.07f, 0.07f, 0.07f, 1.0f) },
    { ImGuiCol_TitleBgCollapsed, ImVec4(0.05f, 0.05f, 0.05f, 1.0f) },
    { ImGuiCol_Header,   ImVec4(0.0f,  0.6f,  0.0f,  1.0f) },
    { ImGuiCol_HeaderHovered, ImVec4(0.0f,  0.8f,  0.0f,  1.0f) },
    { ImGuiCol_HeaderActive, ImVec4(0.0f,  1.0f,  0.0f,  1.0f) },
    { ImGuiCol_Button,   ImVec4(0.0f,  0.7f,  0.0f,  1.0f) },
    { ImGuiCol_ButtonHovered, ImVec4(0.0f,  0.8f,  0.8f,  1.0f) },
    { ImGuiCol_ButtonActive, ImVec4(0.8f,  0.0f,  0.8f,  1.0f) },
    { ImGuiCol_FrameBg,  ImVec4(0.10f, 0.10f, 0.10f, 1.0f) },
    { ImGuiCol_FrameBgHovered, ImVec4(0.12f, 0.12f, 0.12f, 1.0f) },
    { ImGuiCol_FrameBgActive, ImVec4(0.08f, 0.08f, 0.08f, 1.0f) },
    { ImGuiCol_Tab,      ImVec4(0.12f, 0.12f, 0.12f, 1.0f) },
    { ImGuiCol_TabHovered, ImVec4(0.0f,  0.7f,  0.7f, 1.0f) },
    { ImGuiCol_TabActive, ImVec4(0.0f,  1.0f,  0.0f, 1.0f) },
    { ImGuiCol_TabUnfocused, ImVec4(0.10f, 0.10f, 0.10f, 1.0f) },
    { ImGuiCol_TabUnfocusedActive, ImVec4(0.0f,  0.8f,  0.0f, 1.0f) },
    { ImGuiCol_SliderGrab, ImVec4(0.0f,  0.8f,  0.8f, 1.0f) },
    { ImGuiCol_SliderGrabActive, ImVec4(0.0f,  0.7f,  0.7f, 1.0f) },
    { ImGuiCol_ScrollbarBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f) },
    { ImGuiCol_ScrollbarGrab, ImVec4(0.0f,  0.6f,  0.6f, 1.0f) },
    { ImGuiCol_ScrollbarGrabHovered, ImVec4(0.0f,  0.7f,  0.7f, 1.0f) },
    { ImGuiCol_ScrollbarGrabActive, ImVec4(0.0f,  0.8f,  0.8f, 1.0f) },
    { ImGuiCol_Text,     ImVec4(1.0f,  1.0f,  1.0f, 1.0f) },
    { ImGuiCol_TextDisabled, ImVec4(0.6f, 0.6f, 0.6f, 1.0f) }
};

static const size_t NeonThemeColorCount = sizeof(NeonThemeColors) / sizeof(ThemeColor);
static const size_t BlueThemeColorCount = sizeof(BlueThemeColors) / sizeof(ThemeColor);
