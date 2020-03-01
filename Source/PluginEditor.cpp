
#include "PluginProcessor.h"
#include "PluginEditor.h"

SineDistortionAudioProcessorEditor::SineDistortionAudioProcessorEditor(SineDistortionAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p) {

	setSize(250, 250);

	gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	gainSlider.setRange(0.0f, 50.0f, 0.1);
	gainSlider.setValue(1.0f);
	addAndMakeVisible(gainSlider);

	gainSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
			processor.apvts, "GAIN_SLIDER", gainSlider);


	wetDrySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	wetDrySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	wetDrySlider.setRange(0.0f, 1.0f, 0.1);
	wetDrySlider.setValue(0.0f);
	addAndMakeVisible(wetDrySlider);

	wetDrySliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
			processor.apvts, "WETDRY_SLIDER", wetDrySlider);
}

SineDistortionAudioProcessorEditor::~SineDistortionAudioProcessorEditor() {
}

void SineDistortionAudioProcessorEditor::paint(Graphics& g) {

	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void SineDistortionAudioProcessorEditor::resized() {
	Rectangle<int> area = getLocalBounds();
	area.reduce(15, 15);

	gainSlider.setBounds(area.removeFromLeft(area.getWidth() / 2));
	wetDrySlider.setBounds(area);
}