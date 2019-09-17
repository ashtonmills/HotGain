/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HotGainAudioProcessorEditor  : public AudioProcessorEditor,
									 public Slider::Listener
{
public:
    HotGainAudioProcessorEditor (HotGainAudioProcessor&);
    ~HotGainAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider * slider) override;
	ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> sliderAttach;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HotGainAudioProcessor& processor;
	Slider gainSlider;
	Label gainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HotGainAudioProcessorEditor)
};
