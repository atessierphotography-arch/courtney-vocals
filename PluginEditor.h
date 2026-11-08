#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CourtneyVocalChainAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CourtneyVocalChainAudioProcessorEditor (CourtneyVocalChainAudioProcessor&);
    ~CourtneyVocalChainAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CourtneyVocalChainAudioProcessor& audioProcessor;

    juce::Slider distDriveSlider;
    juce::Slider compThreshSlider;
    juce::Slider reverbSizeSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> distDriveAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compThreshAttach;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbSizeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CourtneyVocalChainAudioProcessorEditor)
};
