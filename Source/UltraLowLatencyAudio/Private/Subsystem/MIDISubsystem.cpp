// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MIDISubsystem.h"

#include "UltraLowLatencyAudio.h"
#include "MIDIThread/MIDIRunnable.h"

void UMidiSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	MidiRunnable = new FMidiRunnable();
	MidiThread = FRunnableThread::Create(
		MidiRunnable,
		TEXT("MIDIThread"),
		0,
		TPri_TimeCritical
		);
}

void UMidiSubsystem::Deinitialize()
{
	if (MidiRunnable)
	{
		MidiRunnable->Stop();
		MidiThread->Kill(true);
		delete MidiRunnable;
		MidiRunnable = nullptr;
	}
}

TArray<FString> UMidiSubsystem::GetAvailableAudioDeviceTypes()
{
	return MidiRunnable ? MidiRunnable->GetAudioEngine().GetAvailableAudioDeviceTypes() : TArray<FString>();
}

void UMidiSubsystem::SetAudioDeviceType(const FString& TypeName)
{
	MidiRunnable->GetAudioEngine().SetAudioDeviceType(TypeName);
}

TArray<FString> UMidiSubsystem::GetMidiDevices()
{
	return MidiRunnable ? MidiRunnable->GetAudioEngine().GetMidiDevices() : TArray<FString>();
}

void UMidiSubsystem::SetMidiInputDevice(const int32 DeviceID)
{
	MidiRunnable->GetAudioEngine().SetMidiInputDevice(GetMidiDevices()[DeviceID]);
}
