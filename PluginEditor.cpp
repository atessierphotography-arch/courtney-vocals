#include "PluginProcessor.h"
#include "PluginEditor.h"

CourtneyVocalChainAudioProcessorEditor::CourtneyVocalChainAudioProcessorEditor (CourtneyVocalChainAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (850, 480);

    auto setupSlider = [this](juce::Slider& slider) {
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 18);
        slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour::fromRGB(220, 130, 40));
        slider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour::fromRGB(180, 185, 190));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(40, 40, 40));
        addAndMakeVisible(slider);
    };

    setupSlider(distDriveSlider);
    setupSlider(compThreshSlider);
    setupSlider(reverbSizeSlider);

    distDriveAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DIST_DRIVE", distDriveSlider);
    compThreshAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMP_THRESH", compThreshSlider);
    reverbSizeAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "REVERB_SIZE", reverbSizeSlider);
}

CourtneyVocalChainAudioProcessorEditor::~CourtneyVocalChainAudioProcessorEditor() {}

void CourtneyVocalChainAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fond Blanc Mat
    g.fillAll (juce::Colour::fromRGB (242, 244, 247));

    // En-tête Métallique
    juce::Rectangle<float> headerArea (0, 0, (float) getWidth(), 50.0f);
    juce::ColourGradient headerGradient(
        juce::Colour::fromRGB(210, 215, 220), 0.0f, 0.0f,
        juce::Colour::fromRGB(180, 185, 190), 0.0f, 50.0f, false
    );
    g.setGradientFill(headerGradient);
    g.fillRect(headerArea);

    g.setColour (juce::Colour::fromRGB (30, 32, 35));
    g.setFont (juce::FontOptions(20.0f, juce::Font::bold));
    g.drawText ("COURTNEY VOCAL CHAIN", 20, 0, 400, 50, juce::Justification::centredLeft);

    // Panneaux en Métal Brossé
    auto area = getLocalBounds().reduced(15);
    area.removeFromTop(45);

    int moduleWidth = (area.getWidth() - 30) / 3;

    auto drawMetalPanel = [&g](juce::Rectangle<int> bounds, const juce::String title) {
        g.setColour(juce::Colours::black.withAlpha(0.12f));
        g.fillRoundedRectangle(bounds.toFloat().translated(0, 3), 8.0f);

        juce::ColourGradient metalGrad(
            juce::Colour::fromRGB(225, 228, 232), (float) bounds.getX(), (float) bounds.getY(),
            juce::Colour::fromRGB(195, 200, 205), (float) bounds.getX(), (float) bounds.getBottom(), false
        );
        g.setGradientFill(metalGrad);
        g.fillRoundedRectangle(bounds.toFloat(), 8.0f);

        g.setColour(juce::Colour::fromRGB(255, 255, 255).withAlpha(0.8f));
        g.drawRoundedRectangle(bounds.toFloat().reduced(1.0f), 7.0f, 1.0f);
        g.setColour(juce::Colour::fromRGB(160, 165, 170));
        g.drawRoundedRectangle(bounds.toFloat(), 8.0f, 1.5f);

        g.setColour(juce::Colour::fromRGB(120, 125, 130));
        g.fillEllipse(bounds.getX() + 8, bounds.getY() + 8, 5, 5);
        g.fillEllipse(bounds.getRight() - 13, bounds.getY() + 8, 5, 5);
        g.fillEllipse(bounds.getX() + 8, bounds.getBottom() - 13, 5, 5);
        g.fillEllipse(bounds.getRight() - 13, bounds.getBottom() - 13, 5, 5);

        g.setColour(juce::Colour::fromRGB(40, 45, 50));
        g.setFont(juce::FontOptions(14.0f, juce::Font::bold));
        g.drawText(title, bounds.removeFromTop(35), juce::Justification::centred);
    };

    drawMetalPanel(area.removeFromLeft(moduleWidth), "SATURATION");
    area.removeFromLeft(15);
    drawMetalPanel(area.removeFromLeft(moduleWidth), "COMPRESSION");
    area.removeFromLeft(15);
    drawMetalPanel(area, "REVERB / SPACE");
}

void CourtneyVocalChainAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(15);
    area.removeFromTop(85);

    int moduleWidth = (area.getWidth() - 30) / 3;

    distDriveSlider.setBounds(area.removeFromLeft(moduleWidth).reduced(20));
    area.removeFromLeft(15);
    compThreshSlider.setBounds(area.removeFromLeft(moduleWidth).reduced(20));
    area.removeFromLeft(15);
    reverbSizeSlider.setBounds(area.reduced(20));
}
