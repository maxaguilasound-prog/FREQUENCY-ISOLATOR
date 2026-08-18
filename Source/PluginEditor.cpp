#include "PluginEditor.h"

FrequencyIsolatorAudioProcessorEditor::FrequencyIsolatorAudioProcessorEditor(FrequencyIsolatorAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(760, 300);

    title.setText("FREQUENCY ISOLATOR", juce::dontSendNotification);
    title.setFont(juce::Font(24.0f, juce::Font::bold));
    addAndMakeVisible(title);

    subtitle.setText("Octave-band isolation", juce::dontSendNotification);
    addAndMakeVisible(subtitle);

    const juce::String names[10] = { "31.5", "63", "125", "250", "500", "1K", "2K", "4K", "8K", "16K" };
    for (int i = 0; i < 10; ++i)
    {
        bandButtons[i].setButtonText(names[i]);
        bandButtons[i].setRadioGroupId(1001);
        bandButtons[i].setClickingTogglesState(true);
        bandButtons[i].onClick = [this, i]
        {
            if (auto* parameter = processor.apvts.getParameter("band"))
                parameter->setValueNotifyingHost(static_cast<float>(i) / 9.0f);
        };
        addAndMakeVisible(bandButtons[i]);
    }
    bandButtons[5].setToggleState(true, juce::dontSendNotification);

    bypass.setButtonText("Bypass");
    addAndMakeVisible(bypass);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processor.apvts, "bypass", bypass);

    outputLabel.setText("Output", juce::dontSendNotification);
    addAndMakeVisible(outputLabel);

    output.setSliderStyle(juce::Slider::LinearHorizontal);
    output.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 24);
    output.setRange(-24.0, 12.0, 0.01);
    output.setTextValueSuffix(" dB");
    addAndMakeVisible(output);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processor.apvts, "output", output);
}

void FrequencyIsolatorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff181818));
    g.setColour(juce::Colour(0xff2b2b2b));
    g.fillRoundedRectangle(18.0f, 80.0f, static_cast<float>(getWidth() - 36), 105.0f, 8.0f);
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawFittedText("ISOLATE - selected octave band is passed; frequencies outside the band are strongly attenuated.",
                     24, 198, getWidth() - 48, 35, juce::Justification::centred, 2);
}

void FrequencyIsolatorAudioProcessorEditor::resized()
{
    title.setBounds(24, 18, 400, 30);
    subtitle.setBounds(24, 48, 300, 22);

    const int left = 24;
    const int top = 98;
    const int gap = 6;
    const int width = (getWidth() - 48 - gap * 9) / 10;

    for (int i = 0; i < 10; ++i)
        bandButtons[i].setBounds(left + i * (width + gap), top, width, 58);

    bypass.setBounds(24, 238, 100, 24);
    outputLabel.setBounds(470, 234, 55, 24);
    output.setBounds(525, 234, 205, 24);
}
