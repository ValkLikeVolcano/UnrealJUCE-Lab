// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1
#define JUCE_DONT_SET_USING_JUCE_NAMESPACE 1

//Unreal
#include "Engine.h"
#include "Modules/ModuleManager.h"
#include "SlateBasics.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

//JUCE
#include "juce_core/juce_core.h"
#include "juce_cryptography/juce_cryptography.h"
#include "juce_events/juce_events.h"
#include "juce_data_structures/juce_data_structures.h"
#include "juce_graphics/juce_graphics.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_gui_extra/juce_gui_extra.h"
#include "juce_opengl/juce_opengl.h"
#include "juce_video/juce_video.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_audio_utils/juce_audio_utils.h"

//JUCE Modules
#include "Audio/AudioPCH.h"