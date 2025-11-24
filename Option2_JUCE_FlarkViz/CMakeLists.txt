cmake_minimum_required(VERSION 3.15)

project(FlarkViz VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find JUCE locally first
set(JUCE_PATHS
    "$ENV{HOME}/JUCE"
    "/usr/share/juce"
    "/usr/local/share/juce"
)

foreach(JUCE_PATH ${JUCE_PATHS})
    if(EXISTS "${JUCE_PATH}/modules")
        message(STATUS "Found JUCE at: ${JUCE_PATH}")
        add_subdirectory(${JUCE_PATH} juce_build)
        set(JUCE_FOUND TRUE)
        break()
    endif()
endforeach()

# If JUCE not found, download it
if(NOT JUCE_FOUND)
    message(STATUS "JUCE not found locally, downloading...")
    include(FetchContent)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 7.0.12
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(JUCE)
endif()

# Source files
set(SOURCE_FILES
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/Audio/AudioAnalyzer.cpp
    Source/Audio/AudioCapture.cpp
    Source/Rendering/PresetRenderer.cpp
    Source/Rendering/ShaderCompiler.cpp
    Source/Rendering/RenderState.cpp
    Source/Rendering/FramebufferManager.cpp
    Source/Rendering/TransitionEngine.cpp
    Source/Presets/PresetManager.cpp
    Source/Presets/PresetLoader.cpp
    Source/Presets/Milk2Loader.cpp
    Source/Presets/Preset.cpp
    Source/Expression/MilkdropEval.cpp
)

# Create executable
juce_add_gui_app(FlarkViz
    PRODUCT_NAME "FlarkViz"
    COMPANY_NAME "flarkAUDIO"
    BUNDLE_ID "com.flarkaudio.flarkviz"
)

target_sources(FlarkViz PRIVATE ${SOURCE_FILES})

target_include_directories(FlarkViz PRIVATE Source)

target_compile_definitions(FlarkViz
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:FlarkViz,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:FlarkViz,JUCE_VERSION>"
)

target_link_libraries(FlarkViz
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        juce::juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
