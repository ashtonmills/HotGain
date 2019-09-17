/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//This is the initialization list: to initialise (so when the member is created, it is given a value, rather than reating it empty then giving it a value, this is more efficient apparently).
HotGainAudioProcessor::HotGainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	//logVolume(-5.0f),
	parameters(*this, nullptr)

//this is the constructor
#endif
{
	NormalisableRange<float>gainRange(-48.0f, 0.0f);
	parameters.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_NAME, gainRange,0.5f, nullptr, nullptr);
	parameters.state = ValueTree("savedParams");
}

HotGainAudioProcessor::~HotGainAudioProcessor()
{
}

//==============================================================================
const String HotGainAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HotGainAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HotGainAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HotGainAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HotGainAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HotGainAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HotGainAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HotGainAudioProcessor::setCurrentProgram (int index)
{
}

const String HotGainAudioProcessor::getProgramName (int index)
{
    return {};
}

void HotGainAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HotGainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	previousGain = pow(10, *parameters.getRawParameterValue(GAIN_ID));
}

void HotGainAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HotGainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HotGainAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	float currentGain = pow(10, *parameters.getRawParameterValue(GAIN_ID) / 20);

	if (currentGain == previousGain)
	{
		buffer.applyGain(currentGain);
	}
	else
	{
		//apply gain ramp gradually increments each sample from the start sample to the last sample in the buffer
		//this stops there being a 'click' when you change th values quickly. 
		buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
		previousGain = currentGain;
	}

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

}

//==============================================================================
bool HotGainAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HotGainAudioProcessor::createEditor()
{
    return new HotGainAudioProcessorEditor (*this);
}

//==============================================================================
void HotGainAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	std::unique_ptr<XmlElement> xml (parameters.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void HotGainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr <XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));
	if (theParams != nullptr) 
	{
		if (theParams->hasTagName(parameters.state.getType()))
		{
			parameters.state = ValueTree::fromXml(*theParams);
		}
	}

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HotGainAudioProcessor();
}
