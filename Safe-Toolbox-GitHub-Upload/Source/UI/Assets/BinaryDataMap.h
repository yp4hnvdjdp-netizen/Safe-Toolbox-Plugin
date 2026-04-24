#pragma once

#include <juce_graphics/juce_graphics.h>

#include "BinaryData.h"

namespace SafeToolboxBinaryData
{
    inline const char* getResource (const juce::String& originalFilename, int& dataSize) noexcept
    {
        return BinaryData::getNamedResource (originalFilename.toRawUTF8(), dataSize);
    }
}

