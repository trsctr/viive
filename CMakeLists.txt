cmake_minimum_required(VERSION 3.22)

project(viive VERSION 0.3.0)


### Dependency versions ###

set(LIB_JUCE_TAG "8.0.11")


### IDE Generator pre-config ###

set(CMAKE_XCODE_GENERATE_SCHEME OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)


### Dependency management ###

include(FetchContent)

set(FETCHCONTENT_BASE_DIR "${PROJECT_SOURCE_DIR}/Libs" CACHE PATH "External dependencies path." FORCE)

FetchContent_Declare(juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        ${LIB_JUCE_TAG}
    GIT_SHALLOW    TRUE
    GIT_CONFIG     advice.detachedHead=false
    SOURCE_DIR     "${FETCHCONTENT_BASE_DIR}/JUCE"
    SUBBUILD_DIR   "${FETCHCONTENT_BASE_DIR}/JUCE-Subbuild"
    BINARY_DIR     "${FETCHCONTENT_BASE_DIR}/JUCE-Build")

FetchContent_MakeAvailable(juce)


### Developer options ###

set(VST3_DEV_COPY_DIR "" CACHE PATH "If set, copies the Debug VST3 build here after each build")


### Plugin Project Setup ###

juce_add_plugin(viive
    VERSION 0.3.0
    FORMATS VST3 Standalone
    PRODUCT_NAME "viive"
    COMPANY_NAME "trsctr"
    PLUGIN_MANUFACTURER_CODE Trsc
    PLUGIN_CODE Viiv
    PLUGIN_CATEGORY "Delay"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)

if(VST3_DEV_COPY_DIR)
    add_custom_command(TARGET viive_VST3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_directory,true>
            "$<TARGET_FILE_DIR:viive_VST3>/../.."
            "${VST3_DEV_COPY_DIR}/viive.vst3")
endif()

target_compile_features(viive PUBLIC cxx_std_20)

get_property(project_targets DIRECTORY "${PROJECT_SOURCE_DIR}" PROPERTY BUILDSYSTEM_TARGETS)
set_target_properties(${project_targets} PROPERTIES CXX_EXTENSIONS OFF)

target_sources(viive PRIVATE
    Source/BitCrusher.cpp
    Source/LofiEngine.cpp
    Source/InsertEffectSelector.cpp
    Source/FilterEngine.cpp
    Source/Tempo.cpp
    Source/Selector.cpp
    Source/KillButton.cpp
    Source/SyncButton.cpp
    Source/StereoDelay.cpp
    Source/StereoFilter.cpp
    Source/ChorusEngine.cpp
    Source/LFO.cpp
    Source/Chorus.cpp
    Source/LevelMeter.cpp
    Source/RotaryKnob.cpp
    Source/DelayEngine.cpp
    Source/Parameters.cpp
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/LFOScope.cpp)

juce_generate_juce_header(viive)

target_compile_definitions(viive
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_link_libraries(viive
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_processors_headless
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


### IDE Generator post-config ###

set_target_properties(${project_targets} PROPERTIES
    FOLDER "Targets"
    XCODE_GENERATE_SCHEME ON)

set_target_properties(viive PROPERTIES FOLDER "")

get_target_property(project_sources viive SOURCES)
get_target_property(juce_library_code viive JUCE_GENERATED_SOURCES_DIRECTORY)
set(juce_header "${juce_library_code}/JuceHeader.h")

list(REMOVE_ITEM project_sources "${juce_header}")

source_group("JUCE Library Code" FILES "${juce_header}")
source_group("JUCE Library Code/CMake Rules" FILES "${juce_header}.rule")
source_group(TREE "${PROJECT_SOURCE_DIR}/Source" FILES ${project_sources})