#include <juce_core/juce_core.h>

#include "../Source/Parameters/PresetDefinitions.h"

class PresetContractTests final : public juce::UnitTest
{
public:
    PresetContractTests()
        : juce::UnitTest ("Safe Toolbox Preset Contract", "Safe Toolbox")
    {
    }

    void runTest() override
    {
        beginTest ("Preset list is available");
        const auto names = getPresetNames();
        expect (names.size() > 0);
        expectEquals (names[0], juce::String ("Clean Safe"));
    }
};

static PresetContractTests presetContractTests;
