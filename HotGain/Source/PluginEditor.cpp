/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HotGainAudioProcessorEditor::HotGainAudioProcessorEditor (HotGainAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

	gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	gainSlider.setRange(-48.0, 0.0);
	gainSlider.setNumDecimalPlacesToDisplay(2);
	//gainSlider.setValue(-5.0);
	gainSlider.setColour(Slider::thumbColourId,Colours::red);
	gainSlider.addListener(this);
	addAndMakeVisible(gainSlider);


	addAndMakeVisible(gainLabel);
	gainLabel.setText("GAIN", dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);
	gainLabel.setColour(Label::textColourId, Colours::aqua);

	sliderAttach = new AudioProcessorValueTreeState::SliderAttachment(processor.parameters,GAIN_ID,gainSlider);

	setSize(200, 400);

}

HotGainAudioProcessorEditor::~HotGainAudioProcessorEditor()
{
}

//==============================================================================
void HotGainAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::black);

}

void HotGainAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	gainSlider.setBounds(getWidth()/3,getHeight()/5,getWidth()/2,getHeight()-200);
}

void HotGainAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
	if (slider == &gainSlider)
	{
		processor.logVolume = pow(10,gainSlider.getValue()/20); //to get a logarithmic value we do (10 to the power of (our value/20)) 
	}
}
