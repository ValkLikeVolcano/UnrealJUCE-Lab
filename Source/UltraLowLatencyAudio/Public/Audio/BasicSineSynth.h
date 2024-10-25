
#pragma once


struct FBasicSineWaveSound final : juce::SynthesiserSound
{
	FBasicSineWaveSound() {}

	virtual bool appliesToNote (int /*midiNoteNumber*/) override { return true; }
	virtual bool appliesToChannel (int /*midiChannel*/) override { return true; }
};


struct FBasicSineWaveVoice final : juce::SynthesiserVoice
{
	virtual bool canPlaySound (juce::SynthesiserSound* sound) override
	{
		return dynamic_cast<FBasicSineWaveSound*> (sound) != nullptr;
	}

	virtual void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override
	{
		currentAngle = 0.0;
		level = velocity * 0.3;
		tailOff = 0.0;

		auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
		auto cyclesPerSample = cyclesPerSecond / getSampleRate();

		angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
	}

	virtual void stopNote(float velocity, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			// start a tail-off by setting this flag. The render callback will pick up on
			// this and do a fade out, calling clearCurrentNote() when it's finished.

			if (juce::approximatelyEqual (tailOff, 0.0)) // we only need to begin a tail-off if it's not already doing so - the
				tailOff = 1.0;                     // stopNote method could be called more than once.
		}
		else
		{
			// we're being told to stop playing immediately, so reset everything..
			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	virtual void pitchWheelMoved (int /*newValue*/) override							{}
	virtual void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override	{}

	virtual void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		if (!juce::approximatelyEqual(angleDelta, 0.0))
		{
			if (tailOff > 0.0)
			{
				while (--numSamples >= 0)
				{
					float currentSample = static_cast<float>(std::sin(currentAngle) * level * tailOff);

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					{
						outputBuffer.addSample(i, startSample, currentSample);
					}

					currentAngle += angleDelta;
					++startSample;

					tailOff *= 0.99;

					if (tailOff <= 0.005)
					{
						clearCurrentNote();
						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0)
				{
					float currentSample = (float)(std::sin(currentAngle) * level);

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
					{
						outputBuffer.addSample(i, startSample, currentSample);
					}

					currentAngle += angleDelta;
					++startSample;
				}
			}
		}
	}
	using SynthesiserVoice::renderNextBlock; // why is this here?

private:
	double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};


struct FBasicSynthAudioSource final : juce::AudioSource
{
	FBasicSynthAudioSource () 
	{
		synth.clearVoices();
		
		// Add some voices to our synth, to play the sounds
		for (auto i = 0; i < 5; ++i)
		{
			synth.addVoice (new FBasicSineWaveVoice());
		}

		// ..and add a sound for them to play...
		synth.clearSounds();
		synth.addSound (new FBasicSineWaveSound());
	}

	void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
	{
		midiCollector.reset (sampleRate);

		synth.setCurrentPlaybackSampleRate (sampleRate);

		synth.setNoteStealingEnabled(false);
	}

	void releaseResources() override {}

	void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
	{
		// the synth always adds its output to the audio buffer, so we have to clear it
		// first..
		bufferToFill.clearActiveBufferRegion();

		// fill a midi buffer with incoming messages from the midi input.
		juce::MidiBuffer incomingMidi;
		midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
		
		// and now get the synth to process the midi events and generate its output.
		synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
	}

	//==============================================================================
	// this collects real-time midi messages from the midi input device, and
	// turns them into blocks that we can process in our audio callback
	juce::MidiMessageCollector midiCollector;

	// the synth itself!
	juce::Synthesiser synth;
};
