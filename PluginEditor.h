#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GainTutorialAudioProcessorEditor  : public AudioProcessorEditor,
										  public Slider::Listener
{
public:
    GainTutorialAudioProcessorEditor (GainTutorialAudioProcessor&);
    ~GainTutorialAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider *slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainTutorialAudioProcessor& processor;

	Slider gainSlider;
	Slider dryWetSlider;

	OwnedArray<AudioProcessorValueTreeState::SliderAttachment> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainTutorialAudioProcessorEditor)
};