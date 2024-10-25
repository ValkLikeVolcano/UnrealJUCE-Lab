
#include "Audio/AudioEngine.h"

#include "UltraLowLatencyAudio.h"
#include "Audio/MIDICallback.h"

FAudioEngine::FAudioEngine()
{
	BasicSynthAudioSource = new FBasicSynthAudioSource();
	BasicSynthAudioSource->prepareToPlay(512, 48000.0);

	MidiCallback = std::make_unique<FMidiCallback>(*this);
}

FAudioEngine::~FAudioEngine()
{
	if(MidiInput)
	{
		MidiInput->stop();
		MidiInput.reset();
	}
	
	AudioDeviceManager.removeAudioCallback(this);
}

void FAudioEngine::InitializeAudioDevice(int NumInputChannels, int NumOutputChannels, double SampleRate, int BufferSize, const FString& DeviceType)
{
	AudioDeviceManager.initialise(NumInputChannels, NumOutputChannels, nullptr, true);
	
	SetAudioDeviceType(DeviceType);
	
	juce::AudioDeviceManager::AudioDeviceSetup Setup;
	AudioDeviceManager.getAudioDeviceSetup(Setup);
	
	Setup.sampleRate = SampleRate;
	Setup.bufferSize = BufferSize;

	auto result = AudioDeviceManager.setAudioDeviceSetup(Setup, true);
	if (result.isNotEmpty())
	{
		UE_LOG(LogJUCEModule, Error, TEXT("Failed to set audio device setup: %s"), *FString(ANSI_TO_TCHAR(result.toRawUTF8())));
	}
	
	AudioDeviceManager.addAudioCallback(this);
	
	juce::String log("Audio Output Device: " + Setup.outputDeviceName);
	log += juce::String(" | Audio Input Device: " + Setup.inputDeviceName);
	UE_LOG(LogJUCEModule, Log, TEXT("%s"), *FString(ANSI_TO_TCHAR(log.getCharPointer().getAddress())));
	UE_LOG(LogJUCEModule, Log, TEXT("Audio Device Type: %hs"), AudioDeviceManager.getCurrentAudioDeviceType().toRawUTF8());
}

TArray<FString> FAudioEngine::GetAvailableAudioDeviceTypes()
{
	TArray<FString> StringArray;
	
	juce::StringArray deviceTypeNames;
	auto& deviceTypes = AudioDeviceManager.getAvailableDeviceTypes();

	for (auto& deviceType : deviceTypes)
	{
		FString deviceTypeName = ANSI_TO_TCHAR(deviceType->getTypeName().toRawUTF8());
		StringArray.Add(deviceTypeName);
		UE_LOG(LogJUCEModule, Log, TEXT("Device Type: %s"), *deviceTypeName);
	}

	return StringArray;
}

void FAudioEngine::SetAudioDeviceType(const FString& TypeName)
{
	const juce::String& JuceTypeName = juce::String(TCHAR_TO_ANSI(*TypeName));
	AudioDeviceManager.setCurrentAudioDeviceType(JuceTypeName, true);
}

void FAudioEngine::audioDeviceIOCallbackWithContext(const float* const* inputChannelData, int numInputChannels,
	float* const* outputChannelData, int numOutputChannels, int numSamples, const juce::AudioIODeviceCallbackContext& context)
{
	juce::AudioSourceChannelInfo bufferToFill;

	bufferToFill.buffer = new juce::AudioBuffer(outputChannelData, numOutputChannels, numSamples);
	bufferToFill.startSample = 0;
	bufferToFill.numSamples = numSamples;
    
	// Process the audio for the synth
	BasicSynthAudioSource->getNextAudioBlock(bufferToFill);

	// Check if buffer is filled with data
	for (int channel = 0; channel < numOutputChannels; ++channel)
	{
		if (outputChannelData[channel] != nullptr)
		{
			for (int sample = 0; sample < numSamples; ++sample)
			{
				if (outputChannelData[channel][sample] != 0.0f)
				{
					break;
				}
			}
		}
	}

	// Clean up the buffer pointer
	delete bufferToFill.buffer;
}

void FAudioEngine::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
	UE_LOG(LogJUCEModule, Log, TEXT("Audio Device About to Start"));
}

void FAudioEngine::audioDeviceStopped()
{
	UE_LOG(LogJUCEModule, Log, TEXT("Audio Device Stopped"));
}

void FAudioEngine::audioDeviceError(const juce::String& errorMessage)
{
	AudioIODeviceCallback::audioDeviceError(errorMessage);
	
	UE_LOG(LogJUCEModule, Error, TEXT("Audio Device Error: %s"), *FString(ANSI_TO_TCHAR(errorMessage.toRawUTF8())));
}


TArray<FString> FAudioEngine::GetMidiDevices()
{
	TArray<FString> DeviceNames;

	auto Devices = juce::MidiInput::getAvailableDevices();
	for (const juce::MidiDeviceInfo Device : Devices)
	{
		DeviceNames.Add(FString(ANSI_TO_TCHAR(Device.name.toRawUTF8())));
	}

	return DeviceNames;
}

bool FAudioEngine::SetMidiInputDevice(const FString& DeviceName)
{
	const auto Devices = juce::MidiInput::getAvailableDevices();

	if (MidiInput)
	{
		MidiInput->stop();
		MidiInput.reset();
	}

	for (const auto& Device : Devices)
	{
		if (Device.name == juce::String(TCHAR_TO_ANSI(*DeviceName)))
		{
			MidiInput = juce::MidiInput::openDevice(Device.identifier, MidiCallback.get());
			if (MidiInput)
			{
				MidiInput->start();
				UE_LOG(LogJUCEModule, Log, TEXT("MIDI Device Set: %s"), *DeviceName);
				return true;
			}
		}
	}
	UE_LOG(LogJUCEModule, Warning, TEXT("Failed to open MIDI Device: %s"), *DeviceName);
	return false;
}
