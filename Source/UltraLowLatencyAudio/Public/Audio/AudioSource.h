
#pragma once

class FAudioSource : juce::AudioSource
{
public:
	FAudioSource();
	virtual ~FAudioSource() override;

	virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	virtual void releaseResources() override;

	virtual void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

	juce::MidiMessageCollector MidiCollector;
};
