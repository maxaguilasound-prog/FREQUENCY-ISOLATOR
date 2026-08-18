#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class FrequencyIsolatorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit FrequencyIsolatorAudioProcessorEditor(FrequencyIsolatorAudioProcessor&);
    ~FrequencyIsolatorAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    FrequencyIsolatorAudioProcessor& processor;
    juce::Label title;
    juce::Label subtitle;
    juce::TextButton bandButtons[10];
    juce::ToggleButton bypass;
    juce::Slider output;
    juce::Label outputLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyIsolatorAudioProcessorEditor)
};
