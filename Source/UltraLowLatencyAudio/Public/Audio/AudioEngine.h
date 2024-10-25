
#pragma once

struct FBasicSynthAudioSource;
class FMidiCallback;


/**
 * \class FAudioEngine
 * \brief Manages audio and MIDI input/output for the application.
 *
 * The FAudioEngine class is responsible for initializing and managing the audio device,
 * handling audio input/output callbacks, and managing MIDI input devices. It uses the JUCE
 * library for audio and MIDI processing.
 *
 * \details
 * - Initializes the audio device with specified input/output channels, sample rate, and buffer size.
 * - Provides methods to retrieve and set available audio device types.
 * - Implements the juce::AudioIODeviceCallback interface to handle audio device callbacks, including
 *   processing audio input/output, starting/stopping the audio device, and handling audio device errors.
 * - Manages MIDI input devices, allowing the selection of a MIDI input device and handling
 *   incoming MIDI messages through a callback mechanism.
 * - Contains an instance of FBasicSynthAudioSource to generate audio output.
 *
 * \note
 * - Uses a unique pointer to manage the MIDI input device and callback.
 * - Logs important information and errors using the Unreal Engine logging system.
 *
 * \author Nic
 * \date 2024
 */
class FAudioEngine final : public juce::AudioIODeviceCallback
{
public:
	FAudioEngine();
	virtual ~FAudioEngine() override;

	/** Audio Device Manger **/
	void InitializeAudioDevice(int NumInputChannels, int NumOutputChannels, double SampleRate, int BufferSize, const FString& DeviceType = "Windows Audio");
	
	TArray<FString> GetAvailableAudioDeviceTypes();
	void SetAudioDeviceType(const FString& TypeName);

	/** Audio Device Callback Interface **/
	virtual void audioDeviceIOCallbackWithContext(
		const float* const* inputChannelData,
		int numInputChannels,
		float* const* outputChannelData,
		int numOutputChannels,
		int numSamples,
		const juce::AudioIODeviceCallbackContext& context) override;
	virtual void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
	virtual void audioDeviceStopped() override;
	virtual void audioDeviceError(const juce::String& errorMessage) override;


	/** MIDI Input **/
	static TArray<FString> GetMidiDevices();
	bool SetMidiInputDevice(const FString& DeviceName);

	FBasicSynthAudioSource* BasicSynthAudioSource;

private:
	juce::AudioDeviceManager AudioDeviceManager;

	std::unique_ptr<juce::MidiInput> MidiInput;
	std::unique_ptr<FMidiCallback> MidiCallback;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FAudioEngine)
};
