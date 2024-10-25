// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class FMidiRunnable;

class FMidiCallback final : public juce::MidiInputCallback
{
public:
	explicit FMidiCallback(const FAudioEngine& AudioEngine) : AudioEngine(AudioEngine) {}
	
	virtual void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

private:
	void HandleNoteOn(const juce::MidiMessage& message);
	void HandleNoteOff(const juce::MidiMessage& message);
	void HandleSustainOn(const juce::MidiMessage& message);
	void HandleSustainOff(const juce::MidiMessage& message);

	const FAudioEngine& AudioEngine;
};
