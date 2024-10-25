// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MIDISubsystem.generated.h"

class FMidiRunnable;

/**
 * \class UMidiSubsystem
 * \brief Manages MIDI input devices and handles MIDI input in the application.
 *
 * The UMidiSubsystem class is responsible for managing MIDI input devices, initializing and deinitializing
 * the MIDI thread, and providing methods to interact with MIDI devices. It uses the FMidiRunnable class
 * to handle MIDI input and audio processing in a separate thread.
 *
 * \details
 * - Initializes the MIDI thread using FMidiRunnable during subsystem initialization.
 * - Deinitializes the MIDI thread and cleans up resources during subsystem deinitialization.
 * - Provides methods to retrieve available MIDI devices and set the active MIDI input device.
 * - Uses FMidiRunnable to manage the audio engine and handle MIDI input.
 *
 * \note
 * - Logs important information and errors using the Unreal Engine logging system.
 * - Ensures proper initialization and cleanup of the MIDI thread and associated resources.
 *
 * \author Nic
 * \date 2024
 */
UCLASS()
class ULTRALOWLATENCYAUDIO_API UMidiSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="Audio Driver")
	TArray<FString> GetAvailableAudioDeviceTypes();
	UFUNCTION(BlueprintCallable, Category="Audio Driver")
	void SetAudioDeviceType(const FString& TypeName);

	
	/** MIDI input **/
	UFUNCTION(BlueprintCallable, Category="MIDI")
	TArray<FString> GetMidiDevices();
	UFUNCTION(BlueprintCallable, Category="MIDI")
	void SetMidiInputDevice(const int32 DeviceID);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	FMidiRunnable* MidiRunnable = nullptr;
	FRunnableThread* MidiThread = nullptr;
};
