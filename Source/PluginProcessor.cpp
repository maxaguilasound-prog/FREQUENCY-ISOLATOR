#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr float centres[] = {31.5f, 63.f, 125.f, 250.f, 500.f, 1000.f, 2000.f, 4000.f, 8000.f, 16000.f};

FrequencyIsolatorAudioProcessor::FrequencyIsolatorAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout FrequencyIsolatorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back(std::make_unique<juce::AudioParameterChoice>("band", "Band",
        juce::StringArray{"31.5 Hz", "63 Hz", "125 Hz", "250 Hz", "500 Hz", "1 kHz", "2 kHz", "4 kHz", "8 kHz", "16 kHz"}, 5));
    p.push_back(std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));
    p.push_back(std::make_unique<juce::AudioParameterFloat>("output", "Output", juce::NormalisableRange<float>(-24.f, 12.f, .01f), 0.f));
    return {p.begin(), p.end()};
}

void FrequencyIsolatorAudioProcessor::prepareToPlay(double sr, int bs)
{
    filter.prepare(sr, bs, getTotalNumOutputChannels());
    const int i = juce::jlimit(0, 9, static_cast<int>(apvts.getRawParameterValue("band")->load()));
    filter.setCentreFrequency(centres[i]);
    setLatencySamples(filter.getLatencySamples());
}

void FrequencyIsolatorAudioProcessor::releaseResources() {}

bool FrequencyIsolatorAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const
{
    const auto input = l.getMainInputChannelSet();
    const auto output = l.getMainOutputChannelSet();
    return (input == juce::AudioChannelSet::mono() || input == juce::AudioChannelSet::stereo()) && input == output;
}

void FrequencyIsolatorAudioProcessor::processBlock(juce::AudioBuffer<float>& b, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const auto* bandParam = apvts.getRawParameterValue("band");
    const auto* bypassParam = apvts.getRawParameterValue("bypass");
    const auto* outputParam = apvts.getRawParameterValue("output");
    const int bandIndex = juce::jlimit(0, 9, static_cast<int>(bandParam->load()));
    filter.setCentreFrequency(centres[bandIndex]);
    if (! (bypassParam->load() > 0.5f))
    {
        filter.process(b);
        const float outputDb = outputParam->load();
        b.applyGain(juce::Decibels::decibelsToGain(outputDb));
    }
}

juce::AudioProcessorEditor* FrequencyIsolatorAudioProcessor::createEditor()
{
    return new FrequencyIsolatorAudioProcessorEditor(*this);
}

void FrequencyIsolatorAudioProcessor::getStateInformation(juce::MemoryBlock& d)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, d);
}

void FrequencyIsolatorAudioProcessor::setStateInformation(const void* data, int size)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FrequencyIsolatorAudioProcessor();
}
