#include "PluginProcessor.h"
#include "PluginEditor.h"

CourtneyVocalChainAudioProcessor::CourtneyVocalChainAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

CourtneyVocalChainAudioProcessor::~CourtneyVocalChainAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout CourtneyVocalChainAudioProcessor::createParameterLayout()
{
    juce::std::vector<juce::std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("IN_GAIN", "Input Gain", -24.0f, 24.0f, 0.0f));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("OUT_GAIN", "Output Gain", -24.0f, 24.0f, 0.0f));

    params.push_back(juce::std::make_unique<juce::AudioParameterBool>("DIST_BYPASS", "Distortion Bypass", false));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("DIST_DRIVE", "Distortion Drive", 1.0f, 20.0f, 1.0f));

    params.push_back(juce::std::make_unique<juce::AudioParameterBool>("COMP_BYPASS", "Compressor Bypass", false));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("COMP_THRESH", "Threshold", -60.0f, 0.0f, -12.0f));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("COMP_RATIO", "Ratio", 1.0f, 20.0f, 4.0f));

    params.push_back(juce::std::make_unique<juce::AudioParameterBool>("PITCH_BYPASS", "Pitch Bypass", false));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("FORMANT_SHIFT", "Formant Shift", -12.0f, 12.0f, 0.0f));

    params.push_back(juce::std::make_unique<juce::AudioParameterBool>("REVERB_BYPASS", "Reverb Bypass", false));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("REVERB_SIZE", "Reverb Size", 0.0f, 1.0f, 0.5f));

    params.push_back(juce::std::make_unique<juce::AudioParameterBool>("DELAY_BYPASS", "Delay Bypass", false));
    params.push_back(juce::std::make_unique<juce::AudioParameterFloat>("DELAY_TIME", "Delay Time (ms)", 1.0f, 1000.0f, 250.0f));

    return { params.begin(), params.end() };
}

void CourtneyVocalChainAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = (juce::uint32) getTotalNumOutputChannels();

    inputGain.prepare(spec);
    outputGain.prepare(spec);
    compressorModule.prepare(spec);
    reverbModule.prepare(spec);
    modulationModule.prepare(spec);
    delayModule.prepare(spec);

    distortionModule.functionToUse = [] (float x) { return std::tanh(x); };
    distortionModule.prepare(spec);
}

void CourtneyVocalChainAudioProcessor::releaseResources() {}

bool CourtneyVocalChainAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void CourtneyVocalChainAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    inputGain.setGainDecibels(apvts.getRawParameterValue("IN_GAIN")->load());
    inputGain.process(context);

    if (!apvts.getRawParameterValue("DIST_BYPASS")->load())
    {
        float drive = apvts.getRawParameterValue("DIST_DRIVE")->load();
        buffer.applyGain(drive);
        distortionModule.process(context);
    }

    if (!apvts.getRawParameterValue("COMP_BYPASS")->load())
    {
        compressorModule.setThreshold(apvts.getRawParameterValue("COMP_THRESH")->load());
        compressorModule.setRatio(apvts.getRawParameterValue("COMP_RATIO")->load());
        compressorModule.process(context);
    }

    if (!apvts.getRawParameterValue("REVERB_BYPASS")->load())
    {
        juce::dsp::Reverb::Parameters revParams;
        revParams.roomSize = apvts.getRawParameterValue("REVERB_SIZE")->load();
        reverbModule.setParameters(revParams);
        reverbModule.process(context);
    }

    outputGain.setGainDecibels(apvts.getRawParameterValue("OUT_GAIN")->load());
    outputGain.process(context);
}

juce::AudioProcessorEditor* CourtneyVocalChainAudioProcessor::createEditor()
{
    return new CourtneyVocalChainAudioProcessorEditor (*this);
}

void CourtneyVocalChainAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CourtneyVocalChainAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CourtneyVocalChainAudioProcessor();
}
