#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GainTutorialAudioProcessorEditor::GainTutorialAudioProcessorEditor (GainTutorialAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (250, 250);

	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, GAIN_ID, gainSlider));
	gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	gainSlider.setRange(0.0, 50.0);
	gainSlider.setValue(25.f);
	gainSlider.addListener(this);
	addAndMakeVisible(gainSlider);

	attachments.add(new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, WETDRY_ID, dryWetSlider));
	dryWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	dryWetSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
	dryWetSlider.setRange(-90, 0);
	dryWetSlider.setValue(-45.f);
	dryWetSlider.addListener(this);
	addAndMakeVisible(dryWetSlider);

}

GainTutorialAudioProcessorEditor::~GainTutorialAudioProcessorEditor()
{	
}

//==============================================================================
void GainTutorialAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void GainTutorialAudioProcessorEditor::resized()
{
	Rectangle<int> area = getLocalBounds();
	area.reduce(15, 15);

	gainSlider.setBounds(area.removeFromLeft(area.getWidth() / 2));
	dryWetSlider.setBounds(area);
}

void GainTutorialAudioProcessorEditor::sliderValueChanged(Slider *slider)
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