#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SineDistAudioProcessor::SineDistAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
#endif
	treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

SineDistAudioProcessor::~SineDistAudioProcessor()
{
}

//==============================================================================

AudioProcessorValueTreeState::ParameterLayout SineDistAudioProcessor::createParameterLayout() 
{
	
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

	auto gainParam = std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, 0.0f, 50.0f, 25.0f);
	auto wetdryParam = std::make_unique<AudioParameterFloat>(WETDRY_ID, WETDRY_NAME, 0.0f, 1.0f, 0.5f);

	params.push_back(std::move(gainParam));
	params.push_back(std::move(wetdryParam));

	return { params.begin(), params.end() };

}


const String SineDistAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SineDistAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SineDistAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SineDistAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SineDistAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SineDistAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SineDistAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SineDistAudioProcessor::setCurrentProgram (int index)
{
}

const String SineDistAudioProcessor::getProgramName (int index)
{
    return {};
}

void SineDistAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SineDistAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	//init values using dereferenced raw parameter values.
	gain = *treeState.getRawParameterValue(GAIN_ID);
	dryWet = *treeState.getRawParameterValue(WETDRY_ID);
}

void SineDistAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SineDistAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SineDistAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			//read input from knobs. 
			auto gains = *treeState.getRawParameterValue(GAIN_ID);
			auto dryWets = *treeState.getRawParameterValue(WETDRY_ID);

			//assign current sample to tmpSound;
			tmpSound = buffer.getSample(channel, sample);

			//save original sound for dry wet mix;
			orgSound = tmpSound;

			//sinus waveshaper
			tmpSound = std::sin(tmpSound * gains);

			//hardClip
			tmpSound = std::tanh(tmpSound);

			//dry/wet
			tmpSound = ((tmpSound * dryWets) + (orgSound * (1.f - dryWets)) / 2.f);

			channelData[sample] = tmpSound ;
		}
    }
}

//==============================================================================
bool SineDistAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SineDistAudioProcessor::createEditor()
{
    return new SineDistAudioProcessorEditor (*this);
}

//==============================================================================
void SineDistAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	ScopedPointer <XmlElement> xml(treeState.state.createXml());
	copyXmlToBinary(*xml, destData);

}

void SineDistAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	ScopedPointer <XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));

	if (theParams != nullptr) {
		if (theParams->hasTagName(treeState.state.getType())) {
			treeState.state = ValueTree::fromXml(*theParams);
		}
	}

}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    //return new SineDistAudioProcessor(AudioProcessorValueTreeState);
	return new SineDistAudioProcessor();
}
