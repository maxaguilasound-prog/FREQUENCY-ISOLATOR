#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
class FrequencyIsolatorAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
 explicit FrequencyIsolatorAudioProcessorEditor(FrequencyIsolatorAudioProcessor& p) : AudioProcessorEditor(&p), processor(p)
 {
  setSize(760,300); title.setText("FREQUENCY ISOLATOR",juce::dontSendNotification);title.setFont(juce::Font(24.0f,juce::Font::bold));addAndMakeVisible(title); subtitle.setText("Octave-band isolation",juce::dontSendNotification);addAndMakeVisible(subtitle);
  const juce::StringArray names{"31.5","63","125","250","500","1K","2K","4K","8K","16K"};
  for(int i=0;i<10;++i){buttons[i].setButtonText(names[i]);buttons[i].setRadioGroupId(1001);buttons[i].setClickingTogglesState(true);addAndMakeVisible(buttons[i]);buttons[i].onClick=[this,i]{if(auto* param=processor.apvts.getParameter("band"))param->setValueNotifyingHost((float)i/9.0f);};}
  buttons[5].setToggleState(true,juce::dontSendNotification);
  bypass.setButtonText("Bypass");addAndMakeVisible(bypass);bypassAttachment=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.apvts,"bypass",bypass);
  outputLabel.setText("Output",juce::dontSendNotification);addAndMakeVisible(outputLabel);output.setSliderStyle(juce::Slider::LinearHorizontal);output.setTextBoxStyle(juce::Slider::TextBoxRight,false,70,24);output.setRange(-24,12,.01);output.setTextValueSuffix(" dB");addAndMakeVisible(output);outputAttachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,"output",output);
 }
 void paint(juce::Graphics& g) override {g.fillAll(juce::Colour(0xff181818));g.setColour(juce::Colour(0xff2b2b2b));g.fillRoundedRectangle(18,80,(float)getWidth()-36,105,8);g.setColour(juce::Colours::white);g.setFont(12);g.drawFittedText("ISOLATE — selected octave band is passed; frequencies outside the band are strongly attenuated.",24,198,getWidth()-48,35,juce::Justification::centred,2);}
 void resized() override {title.setBounds(24,18,400,30);subtitle.setBounds(24,48,300,22);int left=24,top=98,gap=6,w=(getWidth()-48-gap*9)/10;for(int i=0;i<10;++i)buttons[i].setBounds(left+i*(w+gap),top,w,58);bypass.setBounds(24,238,100,24);outputLabel.setBounds(470,234,55,24);output.setBounds(525,234,205,24);}
private:
 FrequencyIsolatorAudioProcessor& processor;juce::Label title,subtitle;std::array<juce::TextButton,10> buttons;juce::ToggleButton bypass;juce::Slider output;juce::Label outputLabel;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequencyIsolatorAudioProcessorEditor)
};
