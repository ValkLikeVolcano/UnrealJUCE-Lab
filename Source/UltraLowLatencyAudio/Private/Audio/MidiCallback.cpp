// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/MIDICallback.h"

#include "UltraLowLatencyAudio.h"
#include "MIDIThread/MIDIRunnable.h"

void FMidiCallback::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
	// call handler functions based on the message type
	if (message.isNoteOn())
	{
		HandleNoteOn(message);
	}
	else if (message.isNoteOff())
	{
		HandleNoteOff(message);
	}
	else if (message.isSustainPedalOn())
	{
		HandleSustainOn(message);
	}
	else if (message.isSustainPedalOff())
	{
		HandleSustainOff(message);
	}
}

void FMidiCallback::HandleNoteOn(const juce::MidiMessage& message)
{
	//UE_LOG(LogJUCEModule, Log, TEXT("MIDIThread Latency %f ms"), juce::Time::getMillisecondCounterHiRes() - message.getTimeStamp() * 1000);

	AudioEngine.BasicSynthAudioSource->synth.noteOn(message.getChannel(), message.getNoteNumber(), message.getFloatVelocity());
}

void FMidiCallback::HandleNoteOff(const juce::MidiMessage& message)
{
	AudioEngine.BasicSynthAudioSource->synth.noteOff(message.getChannel(), message.getNoteNumber(), message.getFloatVelocity(), true);
}

void FMidiCallback::HandleSustainOn(const juce::MidiMessage& message)
{
}

void FMidiCallback::HandleSustainOff(const juce::MidiMessage& message)
{
}
