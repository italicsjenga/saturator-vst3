# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.15)

# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.
project(clipping VERSION 0.1.0)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.
find_package(JUCE CONFIG REQUIRED) # If you've installed JUCE to your system

# or
# add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/FaustEffect.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/faust/FaustEffect.dsp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/generated && faust -scn faustdsp -i -a ${CMAKE_CURRENT_SOURCE_DIR}/faust/faustMinimal.h ${CMAKE_CURRENT_SOURCE_DIR}/faust/FaustEffect.dsp -o ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/FaustEffect.h
    VERBATIM
)

add_custom_target(FaustEffect ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/generated/FaustEffect.h)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (Clipping here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.
juce_add_plugin(Clipping

    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME "Alex Janka" # Specify the name of the plugin's author

    # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
    # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Ajnk # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Clp1 # A unique four-character plugin id with exactly one upper-case character

    # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Clipping") # The name of the final executable, which can differ from the target name

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/installer/Clipping_Win64.exe
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/installer/installer_win.iss
        COMMAND iscc ${CMAKE_CURRENT_BINARY_DIR}/installer/installer_win.iss
        VERBATIM
    )

    add_custom_target(Installer_Win ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/installer/Clipping_Win64.exe)

    add_dependencies(Installer_Win Clipping_VST3)

    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/installer-scripts/installer_win.iss FILE_CONTENTS)
    string(REPLACE "%VERSION%" "${PROJECT_VERSION}" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/installer/installer_win.iss "${FILE_CONTENTS}")

    add_custom_command(TARGET Clipping_VST3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Clipping_VST3> ${CMAKE_CURRENT_BINARY_DIR}/installer/Clipping.vst3
    )
endif()

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# juce_generate_juce_header(Clipping)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.
target_sources(Clipping
    PRIVATE
    src/PluginProcessor.cpp)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.
target_compile_definitions(Clipping
    PUBLIC

    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0 # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0 # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

# juce_add_binary_data(AudioPluginData SOURCES ...)
add_dependencies(Clipping FaustEffect)

if(DEFINED ENV{VST3_DEV_INSTALL})
    set(vst3_dev_install "$ENV{VST3_DEV_INSTALL}/Clipping.vst3")

    add_custom_target(Dev_Install COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Clipping_VST3> ${vst3_dev_install})

    add_dependencies(Dev_Install Clipping_VST3)
endif()

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.
target_link_libraries(Clipping
    PRIVATE

    # AudioPluginData           # If we'd created a binary data target, we'd link to it here
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
