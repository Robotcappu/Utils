cmake_minimum_required(VERSION 3.16)
project(Util)

# C++ Standard setzen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-spezifische Flags
if (WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # Mit diesen Flags wird deine EXE statisch gelinkt – 100% autark, wie der Chuck Norris unter den Programmen!
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -municode -static -static-libgcc -static-libstdc++")
    endif()
endif()

# Header-Suchpfade
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/modules/core
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/json
    ${CMAKE_SOURCE_DIR}/modules/settings
    ${CMAKE_SOURCE_DIR}/modules/utilities
    ${CMAKE_SOURCE_DIR}/modules/core
)

# Alle relevanten .cpp-Dateien automatisch sammeln
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/*.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/*.cpp"
)

# Executable erzeugen
add_executable(${PROJECT_NAME}
    main.cpp
    gui.cpp
    modules/cleaners/FindDuplicates.cpp
    modules/cleaners/FindFiles.cpp
    modules/core/Logger.cpp
    modules/core/ConfigHandler.cpp
    modules/settings/Theme.cpp
    modules/settings/ThemeManager.cpp
    modules/utilities/CacheHandler.cpp
    modules/utilities/getFolder.cpp
    modules/utilities/PopupHandler.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_impl_dx9.cpp
    imgui/imgui_impl_win32.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)

# Windows-spezifische Libraries linken
if (WIN32)
    target_link_libraries(${PROJECT_NAME} d3d9 dwmapi gdi32)
endif()
