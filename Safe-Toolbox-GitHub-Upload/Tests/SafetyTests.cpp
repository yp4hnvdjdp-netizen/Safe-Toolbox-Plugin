#include <juce_core/juce_core.h>
#include <limits>

#include "../Source/DSP/SampleSanitizer.h"

class SafetyContractTests final : public juce::UnitTest
{
public:
    SafetyContractTests()
        : juce::UnitTest ("Safe Toolbox Safety Contract", "Safe Toolbox")
    {
    }

    void runTest() override
    {
        beginTest ("Sanitise sample rejects invalid values");
        expectWithinAbsoluteError (SafeToolboxSampleSanitizer::sanitiseSample (std::numeric_limits<float>::quiet_NaN(), 0.25f), 0.25f, 0.0001f);
        expectWithinAbsoluteError (SafeToolboxSampleSanitizer::sanitiseSample (8.0f, 0.0f), 4.0f, 0.0001f);
    }
};

static SafetyContractTests safetyContractTests;
