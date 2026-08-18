#pragma once
#include <JuceHeader.h>

class OctaveBandFilter
{
public:
    void prepare(double sampleRate, int maxBlockSize, int numChannels)
    {
        sr = sampleRate;
        channels = numChannels;
        maxBlock = maxBlockSize;
        rebuild();
    }

    void setCentreFrequency(double f)
    {
        if (std::abs(f - centre) > 0.001) { centre = f; rebuild(); }
    }

    int getLatencySamples() const noexcept { return static_cast<int>(taps.size() / 2); }

    void process(juce::AudioBuffer<float>& buffer)
    {
        if (buffer.getNumSamples() == 0 || taps.empty()) return;
        const int n = buffer.getNumSamples();
        const int L = static_cast<int>(taps.size());
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* x = buffer.getWritePointer(ch);
            auto& h = history[static_cast<size_t>(ch)];
            if (static_cast<int>(h.size()) < L - 1) h.resize(static_cast<size_t>(L - 1), 0.0f);
            std::vector<float> input(static_cast<size_t>(n));
            std::copy(x, x + n, input.begin());
            h.insert(h.end(), input.begin(), input.end());
            for (int i = 0; i < n; ++i)
            {
                double y = 0.0;
                const size_t base = static_cast<size_t>(i);
                for (int k = 0; k < L; ++k)
                    y += static_cast<double>(taps[static_cast<size_t>(k)]) * static_cast<double>(h[base + static_cast<size_t>(L - 1 - k)]);
                x[i] = static_cast<float>(y);
            }
            h.erase(h.begin(), h.begin() + n);
        }
    }

private:
    void rebuild()
    {
        if (sr <= 0.0 || channels <= 0) return;
        constexpr int length = 2049;
        taps.assign(length, 0.0f);
        history.assign(static_cast<size_t>(channels), {});
        const double nyquist = sr * 0.5;
        double lo = std::max(10.0, centre / std::sqrt(2.0));
        double hi = std::min(20000.0, centre * std::sqrt(2.0));
        lo = juce::jlimit(1.0, nyquist * 0.98, lo);
        hi = juce::jlimit(lo + 1.0, nyquist * 0.999, hi);
        const double f1 = lo / sr, f2 = hi / sr;
        const int mid = (length - 1) / 2;
        auto lp = [](double fc, double m)
        {
            if (std::abs(m) < 1.0e-12) return 2.0 * fc;
            return std::sin(2.0 * juce::MathConstants<double>::pi * fc * m) / (juce::MathConstants<double>::pi * m);
        };
        for (int n = 0; n < length; ++n)
        {
            const double m = static_cast<double>(n - mid);
            const double window = 0.42 - 0.5 * std::cos(2.0 * juce::MathConstants<double>::pi * n / (length - 1)) + 0.08 * std::cos(4.0 * juce::MathConstants<double>::pi * n / (length - 1));
            taps[static_cast<size_t>(n)] = static_cast<float>((lp(f2, m) - lp(f1, m)) * window);
        }
    }
    double sr = 48000.0, centre = 1000.0;
    int channels = 2, maxBlock = 1024;
    std::vector<float> taps;
    std::vector<std::vector<float>> history;
};
