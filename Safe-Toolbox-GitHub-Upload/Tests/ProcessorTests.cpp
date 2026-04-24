#include <juce_core/juce_core.h>

#include "../Source/Parameters/ParameterIDs.h"
#include "../Source/Parameters/ParameterSnapshot.h"

class ProcessorContractTests final : public juce::UnitTest
{
public:
    ProcessorContractTests()
        : juce::UnitTest ("Safe Toolbox Processor Contract", "Safe Toolbox")
    {
    }

    void runTest() override
    {
        beginTest ("Frozen parameter version");
        expectEquals (SafeToolboxParamIDs::currentParameterVersion, 1);

        beginTest ("Snapshot defaults are sane");
        const ParameterSnapshot snapshot;
        expectWithinAbsoluteError (snapshot.inputGainDb, 0.0f, 0.0001f);
        expectWithinAbsoluteError (snapshot.safetyCeilingDb, -0.5f, 0.0001f);
        expectWithinAbsoluteError (snapshot.globalMix, 1.0f, 0.0001f);
    }
};

static ProcessorContractTests processorContractTests;
