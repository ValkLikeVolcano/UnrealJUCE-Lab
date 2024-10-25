// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"

/**
 * \class FMidiRunnable
 * \brief Manages a separate thread for handling MIDI input and audio processing.
 *
 * The FMidiRunnable class is responsible for running a separate thread to handle MIDI input and audio processing,
 * ensuring low-latency performance. It uses an instance of FAudioEngine to manage audio and MIDI input/output.
 *
 * \details
 * - Runs a separate thread to handle MIDI input and audio processing.
 * - Initializes the audio device through FAudioEngine with specified input/output channels, sample rate, and buffer size.
 * - Periodically sleeps to prevent blocking the main thread.
 * - Provides methods to start, stop, and exit the thread, ensuring proper resource management.
 * - Uses atomic variables to manage the running state of the thread.
 *
 * \note
 * - Logs important information and errors using the Unreal Engine logging system.
 * - Ensures proper initialization and cleanup of the audio device through FAudioEngine.
 *
 * \author Nic
 * \date 2024
 */
class FMidiRunnable final : public FRunnable
{
public:
	FMidiRunnable();
	virtual ~FMidiRunnable() override;

	// FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	
	FAudioEngine& GetAudioEngine() { return AudioEngine; }

private:
	std::atomic<bool> bIsRunning;
	double SampleRate = 48000.0; // I'd like to set them from the editor eventually
	int BufferSize = 256;
	
	FAudioEngine AudioEngine;
};
