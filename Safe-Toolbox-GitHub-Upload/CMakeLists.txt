cmake_minimum_required(VERSION 3.22)

project(SafeToolbox VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JUCE_DIR "/Users/josephshively/Desktop/JUCE" CACHE PATH "Path to a JUCE checkout")
if (NOT EXISTS "${JUCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "Set JUCE_DIR to a valid JUCE checkout")
endif()

add_subdirectory("${JUCE_DIR}" JUCE)

juce_add_plugin(SafeToolbox
    COMPANY_NAME "Safe Audio"
    BUNDLE_ID "com.safeaudio.safetoolbox"
    PRODUCT_NAME "Safe Toolbox"
    FORMATS AU VST3 Standalone
    PLUGIN_MANUFACTURER_CODE Safe
    PLUGIN_CODE Stbx
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_AU_EXPORT_PREFIX SafeToolbox)

target_sources(SafeToolbox
    PRIVATE
        Source/SafeToolboxProcessor.cpp
        Source/SafeToolboxProcessor.h
        Source/SafeToolboxEditor.cpp
        Source/SafeToolboxEditor.h
        Source/UI/Assets/AssetIds.h
        Source/UI/Assets/AssetLoader.cpp
        Source/UI/Assets/AssetLoader.h
        Source/UI/Assets/BinaryDataMap.h
        Source/Parameters/ParameterIDs.h
        Source/Parameters/ParameterLayout.cpp
        Source/Parameters/ParameterLayout.h
        Source/Parameters/ParameterSnapshot.h
        Source/Parameters/ParameterUtilities.h
        Source/Parameters/PresetDefinitions.cpp
        Source/Parameters/PresetDefinitions.h
        Source/DSP/DcBlocker.cpp
        Source/DSP/DcBlocker.h
        Source/DSP/DelayProcessor.cpp
        Source/DSP/DelayProcessor.h
        Source/DSP/DriveProcessor.cpp
        Source/DSP/DriveProcessor.h
        Source/DSP/FilterProcessor.cpp
        Source/DSP/FilterProcessor.h
        Source/DSP/MeterState.cpp
        Source/DSP/MeterState.h
        Source/DSP/SampleSanitizer.h
        Source/DSP/SafetyLimiter.cpp
        Source/DSP/SafetyLimiter.h
        Source/DSP/SignalChain.cpp
        Source/DSP/SignalChain.h
        Source/DSP/ReverbProcessor.cpp
        Source/DSP/ReverbProcessor.h
        Source/UI/Components/HeaderComponent.cpp
        Source/UI/Components/HeaderComponent.h
        Source/UI/Components/MeterComponent.cpp
        Source/UI/Components/MeterComponent.h
        Source/UI/Components/ModulePanel.cpp
        Source/UI/Components/ModulePanel.h
        Source/UI/Components/PresetBarComponent.cpp
        Source/UI/Components/PresetBarComponent.h
        Source/UI/Components/SafeModePanel.cpp
        Source/UI/Components/SafeModePanel.h
        Source/UI/LookAndFeel/PluginColours.h
        Source/UI/LookAndFeel/SafeToolboxLookAndFeel.cpp
        Source/UI/LookAndFeel/SafeToolboxLookAndFeel.h
        Source/UI/LookAndFeel/RotaryKnobLookAndFeel.cpp
        Source/UI/LookAndFeel/RotaryKnobLookAndFeel.h
        Source/UI/LookAndFeel/Theme.h
        Source/DSP/WobbleProcessor.cpp
        Source/DSP/WobbleProcessor.h
        Source/DSP/WidthProcessor.cpp
        Source/DSP/WidthProcessor.h
        Tests/ProcessorTests.cpp
        Tests/PresetTests.cpp
        Tests/SafetyTests.cpp)

set(SAFE_TOOLBOX_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/Assets/colorbox_ui.png")
set(SAFE_TOOLBOX_EXPORT_SVGS
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_brand_mark.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_safe_badge.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_panel_texture.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_icon_compare.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_icon_copy.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_icon_paste.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_icon_panic.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_meter_ticks.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_knob_face.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_backdrop_texture.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_led_green.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_meter_accent.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_knob_halo.svg"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/export/svg/safe_toolbox_knob_cap.svg")
if (EXISTS "${SAFE_TOOLBOX_BACKGROUND_IMAGE}")
    juce_add_binary_data(SafeToolboxAssets
        SOURCES
            "${SAFE_TOOLBOX_BACKGROUND_IMAGE}"
            ${SAFE_TOOLBOX_EXPORT_SVGS})
else()
    message(WARNING "Assets/colorbox_ui.png is missing; the editor will show a TODO placeholder until the image is restored.")
endif()

target_compile_definitions(SafeToolbox
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_include_directories(SafeToolbox
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries(SafeToolbox
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

if (TARGET SafeToolboxAssets)
    target_link_libraries(SafeToolbox PRIVATE SafeToolboxAssets)
endif()
