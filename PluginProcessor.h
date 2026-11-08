#pragma once

#include <JuceHeader.h>

class CourtneyVocalChainAudioProcessor  : public juce::AudioProcessor
{
public:
    CourtneyVocalChainAudioProcessor();
    ~CourtneyVocalChainAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Courtney Vocal Chain"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Elements DSP
    juce::dsp::Gain<float> inputGain;
    juce::dsp::Gain<float> outputGain;
    
    juce::dsp::WaveShaper<float> distortionModule;
    juce::dsp::Compressor<float> compressorModule;
    juce::dsp::Reverb reverbModule;
    juce::dsp::Chorus<float> modulationModule;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayModule{ 192000 };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CourtneyVocalChainAudioProcessor)
};
