# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
LIB_DAISY_INC_DIR = src/DaisySP/Source
CFLAGS +=
CXXFLAGS += -I$(LIB_DAISY_INC_DIR)

#LIB_DAISY_PATH = ../DaisySP/build
#LIB_DAISY_NAME = libdaisysp

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS += #-L$(LIB_DAISY_PATH) -l$(LIB_DAISY_NAME)

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

SOURCES += src/lowstepper/lowstepper/AverageBuffer.cpp
SOURCES += src/lowstepper/lowstepper/LowStepperLfo.cpp
SOURCES += src/lowstepper/lowstepper/SyncManager.cpp
SOURCES += src/lowstepper/lowstepper/util.cpp

SOURCES += src/lowstepper/lowstepper_rack/LowstepperChannel.cpp
SOURCES += src/lowstepper/lowstepper_rack/LowstepperRack.cpp
SOURCES += src/lowstepper/lowstepper_rack/Names.cpp
SOURCES += src/lowstepper/lowstepper_rack/RackIORef.cpp
SOURCES += src/lowstepper/lowstepper_module.cpp

SOURCES += $(wildcard src/discomfort/*.cpp)
SOURCES += $(wildcard src/discomfort/DiscomfortInternal/*.cpp)

# DaisySP
# TODO this is maybe the worst imaginable way to do this
# Figure out how to get DaisySP to not compile as 32bit ELF format static library
# Alternatively, how to just use the sources within the DaisySP git submodule
SOURCES += src/DaisySP/Source/Control/adsr.cpp
SOURCES += src/DaisySP/Source/Control/adenv.cpp
SOURCES += src/DaisySP/Source/Control/phasor.cpp
SOURCES += src/DaisySP/Source/Drums/analogbassdrum.cpp
SOURCES += src/DaisySP/Source/Drums/analogsnaredrum.cpp
SOURCES += src/DaisySP/Source/Drums/hihat.cpp
SOURCES += src/DaisySP/Source/Drums/synthbassdrum.cpp
SOURCES += src/DaisySP/Source/Drums/synthsnaredrum.cpp
SOURCES += src/DaisySP/Source/Dynamics/crossfade.cpp
SOURCES += src/DaisySP/Source/Dynamics/limiter.cpp
SOURCES += src/DaisySP/Source/Effects/autowah.cpp
SOURCES += src/DaisySP/Source/Effects/chorus.cpp
SOURCES += src/DaisySP/Source/Effects/decimator.cpp
SOURCES += src/DaisySP/Source/Effects/flanger.cpp
SOURCES += src/DaisySP/Source/Effects/overdrive.cpp
SOURCES += src/DaisySP/Source/Effects/phaser.cpp
SOURCES += src/DaisySP/Source/Effects/sampleratereducer.cpp
SOURCES += src/DaisySP/Source/Effects/tremolo.cpp
SOURCES += src/DaisySP/Source/Effects/wavefolder.cpp
SOURCES += src/DaisySP/Source/Filters/ladder.cpp
SOURCES += src/DaisySP/Source/Filters/soap.cpp
SOURCES += src/DaisySP/Source/Filters/svf.cpp
SOURCES += src/DaisySP/Source/Noise/clockednoise.cpp
SOURCES += src/DaisySP/Source/Noise/grainlet.cpp
SOURCES += src/DaisySP/Source/Noise/particle.cpp
SOURCES += src/DaisySP/Source/PhysicalModeling/drip.cpp
SOURCES += src/DaisySP/Source/PhysicalModeling/KarplusString.cpp
SOURCES += src/DaisySP/Source/PhysicalModeling/modalvoice.cpp
SOURCES += src/DaisySP/Source/PhysicalModeling/resonator.cpp
SOURCES += src/DaisySP/Source/PhysicalModeling/stringvoice.cpp
SOURCES += src/DaisySP/Source/Sampling/granularplayer.cpp
SOURCES += src/DaisySP/Source/Synthesis/fm2.cpp
SOURCES += src/DaisySP/Source/Synthesis/formantosc.cpp
SOURCES += src/DaisySP/Source/Synthesis/oscillator.cpp
SOURCES += src/DaisySP/Source/Synthesis/oscillatorbank.cpp
SOURCES += src/DaisySP/Source/Synthesis/variablesawosc.cpp
SOURCES += src/DaisySP/Source/Synthesis/variableshapeosc.cpp
SOURCES += src/DaisySP/Source/Synthesis/vosim.cpp
SOURCES += src/DaisySP/Source/Synthesis/zoscillator.cpp
SOURCES += src/DaisySP/Source/Utility/dcblock.cpp
SOURCES += src/DaisySP/Source/Utility/metro.cpp

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk
