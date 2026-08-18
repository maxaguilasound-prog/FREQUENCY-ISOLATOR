cmake_minimum_required(VERSION 3.22)
project(FrequencyIsolator VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "" FORCE)

include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.12
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(FrequencyIsolator
    COMPANY_NAME "Unbranded"
    BUNDLE_ID com.unbranded.frequencyisolator
    PLUGIN_MANUFACTURER_CODE Unbr
    PLUGIN_CODE FrIs
    FORMATS VST3
    PRODUCT_NAME "Frequency Isolator"
    VERSION 1.0.0
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(FrequencyIsolator PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/OctaveBandFilter.h)

target_compile_definitions(FrequencyIsolator PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(FrequencyIsolator PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags)

if(APPLE)
    set_target_properties(FrequencyIsolator PROPERTIES XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH YES)
endif()
