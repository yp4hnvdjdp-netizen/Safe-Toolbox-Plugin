#pragma once

#include <array>
#include <cmath>

class DcBlocker final
{
public:
    void prepare (double sampleRate, int numChannels) noexcept;
    void reset() noexcept;
    float processSample (float input, int channel) noexcept;

private:
    struct ChannelState
    {
        float x1 = 0.0f;
        float y1 = 0.0f;
    };

    std::array<ChannelState, 2> states {};
    int channels = 2;
    float r = 0.995f;
};
