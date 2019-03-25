#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SineDistAudioProcessorEditor::SineDistAudioProcessorEditor (SineDistAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

	setSize (250, 250);
	
	gainAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, gainSlider);
	gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	gainSlider.setRange(0.0, 50.0);
	gainSlider.addListener(this);
	addAndMakeVisible(gainSlider);

	wetAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, WETDRY_ID, dryWetSlider);
	dryWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	dryWetSlider.setRange(0, 1);
	dryWetSlider.addListener(this);
	addAndMakeVisible(dryWetSlider);

}

SineDistAudioProcessorEditor::~SineDistAudioProcessorEditor()
{	
}

//==============================================================================
void SineDistAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void SineDistAudioProcessorEditor::resized()
{
	Rectangle<int> area = getLocalBounds();
	area.reduce(15, 15);

	gainSlider.setBounds(area.removeFromLeft(area.getWidth() / 2));
	dryWetSlider.setBounds(area);
}

void SineDistAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
	if (slider == &gainSlider)
	{
		processor.knobGain.setValue(gainSlider.getValue());
	}

	if (slider == &dryWetSlider)
	{
		processor.knobDryWet.setValue(
			Decibels::decibelsToGain(dryWetSlider.getValue())			
		);
	}
}