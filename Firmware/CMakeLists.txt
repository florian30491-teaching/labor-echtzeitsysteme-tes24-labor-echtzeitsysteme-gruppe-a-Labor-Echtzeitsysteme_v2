cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Labor-Echtzeitsyteme)

# ---------------------------------------------------------------------------
# Aufgaben-Auswahl
#
# Waehle die aktive Aufgabe per CMake-Cache-Variable:
#   cmake -DAUFGABE=3 ..
# oder in CLion unter:
#   Settings -> Build, Execution, Deployment -> CMake -> CMake options
#   Eintrag: -DAUFGABE=3
#
# Gueltige Werte: 1 – 8  (Standard: 1)
# ---------------------------------------------------------------------------
set(AUFGABE "1" CACHE STRING "Aktive Aufgabe (1-8)")
set_property(CACHE AUFGABE PROPERTY STRINGS 1 2 3 4 5 6 7 8)

if(AUFGABE LESS 1 OR AUFGABE GREATER 8)
    message(FATAL_ERROR "Ungueltige Aufgabe '${AUFGABE}'. Erlaubte Werte: 1-8.")
endif()

message(STATUS "")
message(STATUS "========================================")
message(STATUS "  Kompiliere Aufgabe ${AUFGABE}")
message(STATUS "========================================")
message(STATUS "")

# Zweistellige Nummer bilden (z.B. 3 -> 03)
if(AUFGABE LESS 10)
    set(AUFGABE_NR "0${AUFGABE}")
else()
    set(AUFGABE_NR "${AUFGABE}")
endif()

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Debug-spezifische Optimierungen
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "=== Debug Build aktiviert - Optimierungen werden angewendet ===")

    # Keine Optimierung + maximale Debug-Symbole
    # FORCE überschreibt den Wert aus starm-clang.cmake
    set(CMAKE_C_FLAGS_DEBUG   "-O0 -g3 -gdwarf-4" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -gdwarf-4" CACHE STRING "" FORCE)
    set(CMAKE_ASM_FLAGS_DEBUG "-g3 -gdwarf-4"      CACHE STRING "" FORCE)

    # Debug-spezifische Compiler-Flags
    add_compile_options(
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>   # Frame-Pointer beibehalten
            $<$<CONFIG:Debug>:-fno-inline-functions>     # Kein Inlining -> Call-Stack vollständig
            $<$<CONFIG:Debug>:-fno-optimize-sibling-calls> # Tail-Call-Optimierung aus
            $<$<CONFIG:Debug>:-fstack-usage>             # Stack-Nutzung pro Funktion
            $<$<CONFIG:Debug>:-fdiagnostics-color=always>
    )

    # Debug-Makros
    add_compile_definitions(
            $<$<CONFIG:Debug>:DEBUG=1>
            $<$<CONFIG:Debug>:USE_FULL_ASSERT=1>
    )

    # Linker: nicht strippen, Map-Datei ausgeben
    add_link_options(
            $<$<CONFIG:Debug>:-Wl,--cref>
            $<$<CONFIG:Debug>:-Wl,--print-memory-usage>
            $<$<CONFIG:Debug>:-Wl,-Map=${CMAKE_PROJECT_NAME}_debug.map>
    )
endif()

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Zentraler Aufgaben-Dispatcher (wertet AUFGABE_NR zur Compile-Zeit aus)
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgaben.c
    # Alle Aufgaben-Implementierungen (welche aktiv ist steuert #if AUFGABE_NR)
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_00.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_01.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_02.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_03.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_04.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_05.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_06.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_07.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben/aufgabe_08.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Aufgaben-Header
    ${CMAKE_CURRENT_SOURCE_DIR}/Aufgaben
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Aufgabennummer als Praeprozessor-Symbol verfuegbar (z.B. #if AUFGABE_NR == 3)
    AUFGABE_NR=${AUFGABE}
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
