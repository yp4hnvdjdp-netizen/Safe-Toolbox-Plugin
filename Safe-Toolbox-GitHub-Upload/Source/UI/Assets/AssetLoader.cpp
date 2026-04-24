#include "AssetLoader.h"

#include "AssetIds.h"
#include "BinaryDataMap.h"

#include <juce_graphics/juce_graphics.h>

namespace SafeToolboxAssets
{
    std::unique_ptr<juce::Drawable> loadDrawable (const juce::String& originalFilename)
    {
        int dataSize = 0;
        if (const auto* data = SafeToolboxBinaryData::getResource (originalFilename, dataSize))
            return juce::Drawable::createFromImageData (data, (size_t) dataSize);

        return {};
    }

    juce::Image loadImage (const juce::String& originalFilename)
    {
        int dataSize = 0;
        if (const auto* data = SafeToolboxBinaryData::getResource (originalFilename, dataSize))
        {
            juce::MemoryInputStream stream (data, (size_t) dataSize, false);
            return juce::ImageFileFormat::loadFrom (stream);
        }

        return {};
    }
}
