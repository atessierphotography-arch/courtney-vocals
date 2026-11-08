cmake_minimum_required(VERSION 3.22)
project(CourtneyVocalChain VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.0
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(CourtneyVocalChain
    COMPANY_NAME "OpenSourceAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Osau
    PLUGIN_CODE Cvc1
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Courtney Vocal Chain"
)

target_sources(CourtneyVocalChain PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

target_compile_definitions(CourtneyVocalChain PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(CourtneyVocalChain PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

juce_generate_juce_header(CourtneyVocalChain)
