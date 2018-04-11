#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//=====================================================================
//==================DEFINES FOR VALUE STATE TREE=======================
#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define WETDRY_ID "wetdry"
#define WETDRY_NAME "Wetdry"

//==============================================================================
/**
*/
class GainTutorialAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    GainTutorialAudioProcessor();
    ~GainTutorialAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	LinearSmoothedValue<float> knobGain;
	LinearSmoothedValue<float> knobDryWet;

	//=====================================================
	AudioProcessorValueTreeState treeState;

private:
	double gain;
	double dryWet;
	double tmpSound;
	double orgSound;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainTutorialAudioProcessor)
};
