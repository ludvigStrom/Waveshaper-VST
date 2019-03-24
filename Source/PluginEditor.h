#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SineDistAudioProcessorEditor  : public AudioProcessorEditor,
										  public Slider::Listener
{
public:
    SineDistAudioProcessorEditor (SineDistAudioProcessor&);
    ~SineDistAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider *slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SineDistAudioProcessor& processor;

	Slider gainSlider;
	Slider dryWetSlider;


public:
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttach;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> wetAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineDistAudioProcessorEditor)
};