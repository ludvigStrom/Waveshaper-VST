#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainTutorialAudioProcessor::GainTutorialAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
					//gain(25.f),
					//dryWet(-45.f),
					treeState(*this, nullptr)

#endif
{
	//========================== stateTree 
	NormalisableRange<float> gainRange(0.0f, 50.0f);
	treeState.createAndAddParameter(GAIN_ID, GAIN_NAME, GAIN_NAME, gainRange, gain, nullptr, nullptr, false, true, false, AudioProcessorParameter::genericParameter);
	
	NormalisableRange<float> wetDryRange(-90.0f, 0.0f);
	treeState.createAndAddParameter(WETDRY_ID, WETDRY_NAME, WETDRY_NAME, wetDryRange, dryWet, nullptr, nullptr, false, true, false, AudioProcessorParameter::genericParameter);
}

GainTutorialAudioProcessor::~GainTutorialAudioProcessor()
{
}

//==============================================================================
const String GainTutorialAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GainTutorialAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GainTutorialAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GainTutorialAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GainTutorialAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GainTutorialAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GainTutorialAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GainTutorialAudioProcessor::setCurrentProgram (int index)
{
}

const String GainTutorialAudioProcessor::getProgramName (int index)
{
    return {};
}

void GainTutorialAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GainTutorialAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	//init values using dereferenced raw parameter values.
	gain = *treeState.getRawParameterValue(GAIN_ID); //kanske fel v�rde. decibel:: 
	dryWet = *treeState.getRawParameterValue(WETDRY_ID);
}

void GainTutorialAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GainTutorialAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GainTutorialAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
			gain = knobGain.getNextValue();
			dryWet = knobDryWet.getNextValue();

			//assign current sample to tmpSound;
			tmpSound = buffer.getSample(channel, sample);

			//save original sound for dry wet mix;
			orgSound = tmpSound;

			//sinus waveshaper
			tmpSound = std::sin(tmpSound * gain);

			//hardClip
			tmpSound = std::tanh(tmpSound);

			//dry/wet
			tmpSound = ((tmpSound * dryWet) + (orgSound * (1.f - dryWet)) / 2.f);

			channelData[sample] = tmpSound ;
		}
    }
}

//==============================================================================
bool GainTutorialAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GainTutorialAudioProcessor::createEditor()
{
    return new GainTutorialAudioProcessorEditor (*this);
}

//==============================================================================
void GainTutorialAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GainTutorialAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GainTutorialAudioProcessor();
}
