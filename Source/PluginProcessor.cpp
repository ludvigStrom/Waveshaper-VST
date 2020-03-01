#include "PluginProcessor.h"
#include "PluginEditor.h"

SineDistortionAudioProcessor::SineDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	), apvts(*this, nullptr, "Parameters", createParameters())
	
#endif
{

}

SineDistortionAudioProcessor::~SineDistortionAudioProcessor() {
}

const String SineDistortionAudioProcessor::getName() const {
	return JucePlugin_Name;
}

bool SineDistortionAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SineDistortionAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SineDistortionAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SineDistortionAudioProcessor::getTailLengthSeconds() const {
	return 0.0;
}

int SineDistortionAudioProcessor::getNumPrograms() {
	return 1;
}

int SineDistortionAudioProcessor::getCurrentProgram() {
	return 0;
}

void SineDistortionAudioProcessor::setCurrentProgram(int index) {
}

const String SineDistortionAudioProcessor::getProgramName(int index) {
	return {};
}

void SineDistortionAudioProcessor::changeProgramName(int index, const String& newName) {
}

void SineDistortionAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	gain = *apvts.getRawParameterValue("GAIN_SLIDER");
	dryWet = *apvts.getRawParameterValue("WETDRY_SLIDER");
}

void SineDistortionAudioProcessor::releaseResources() {
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SineDistortionAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void SineDistortionAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
	
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	auto gains = *apvts.getRawParameterValue("GAIN_SLIDER");
	auto dryWets = *apvts.getRawParameterValue("WETDRY_SLIDER");

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for (int channel = 0; channel < totalNumInputChannels; ++channel) {
		auto* channelData = buffer.getWritePointer(channel);

		for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

			//assign current sample to tmpSound;
			temporarySound = buffer.getSample(channel, sample);

			//save original sound for dry wet mix;
			originalSound = temporarySound;

			//sinus waveshaper
			temporarySound = std::sin(temporarySound * gains);

			//hardClip
			temporarySound = std::tanh(temporarySound);

			//dry/wet
			temporarySound = ((temporarySound * dryWets) + (originalSound * (1.f - dryWets)) / 2.f);

			channelData[sample] = temporarySound;
		}
	}
}

bool SineDistortionAudioProcessor::hasEditor() const {
	return true;
}

AudioProcessorEditor* SineDistortionAudioProcessor::createEditor() {
	return new SineDistortionAudioProcessorEditor(*this);
}

void SineDistortionAudioProcessor::getStateInformation(MemoryBlock& destData) {
	auto state = apvts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);

}

void SineDistortionAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(apvts.state.getType()))
			apvts.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new SineDistortionAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout SineDistortionAudioProcessor::createParameters() {
	std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

	parameters.push_back(std::make_unique<AudioParameterFloat>("GAIN_SLIDER", "Gain slider", 0.0f, 50.0f, 1.0f));
	parameters.push_back(std::make_unique<AudioParameterFloat>("WETDRY_SLIDER", "Wet Dry slider", 0.0f, 1.0f, 1.0f));

	return { parameters.begin(), parameters.end() };
}