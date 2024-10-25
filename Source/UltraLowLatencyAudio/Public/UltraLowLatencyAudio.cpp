// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraLowLatencyAudio.h"

DEFINE_LOG_CATEGORY(LogJUCEModule)

#define LOCTEXT_NAMESPACE "FUltraLowLatencyAudioModule"

void FUltraLowLatencyAudioModule::StartupModule()
{
	UE_LOG(LogJUCEModule, Log, TEXT("UltraLowLatencyAudio module starting up..."));
}

void FUltraLowLatencyAudioModule::ShutdownModule()
{
	UE_LOG(LogJUCEModule, Log, TEXT("UltraLowLatencyAudio module shutting down..."));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUltraLowLatencyAudioModule, UltraLowLatencyAudio)