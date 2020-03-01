#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class SineDistortionAudioProcessorEditor : public AudioProcessorEditor {
public:
	SineDistortionAudioProcessorEditor(SineDistortionAudioProcessor&);
	~SineDistortionAudioProcessorEditor();

	void paint(Graphics&) override;
	void resized() override;

private:
	Slider gainSlider;
	Slider wetDrySlider;
    Slider outputVolumeSlider;

	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> wetDrySliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>
        outputVolumeSliderAttachement;

	SineDistortionAudioProcessor& processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineDistortionAudioProcessorEditor)
};
