// Fill out your copyright notice in the Description page of Project Settings.


#include "MIDIThread/MIDIRunnable.h"
#include "UltraLowLatencyAudio.h"
FMidiRunnable::FMidiRunnable()
	: bIsRunning(true),
	SampleRate(48000)
{
}

FMidiRunnable::~FMidiRunnable()
{
	FMidiRunnable::Stop();
}


bool FMidiRunnable::Init()
{
	UE_LOG(LogJUCEModule, Log, TEXT("MIDI Thread Started"));

	AudioEngine.InitializeAudioDevice(2, 2, SampleRate, BufferSize, "ASIO");

	return true;
}

uint32 FMidiRunnable::Run()
{
	while (bIsRunning)
	{
		FPlatformProcess::Sleep(0.001f); // Small sleep to prevent blocking
	}
	return 0;
}

void FMidiRunnable::Stop()
{
	bIsRunning = false;
}

void FMidiRunnable::Exit()
{
	UE_LOG(LogJUCEModule, Log, TEXT("MIDI Thread Stopped"));
}

