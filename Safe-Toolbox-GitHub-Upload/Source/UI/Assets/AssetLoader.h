#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace juce
{
    class Drawable;
}

namespace SafeToolboxAssets
{
    std::unique_ptr<juce::Drawable> loadDrawable (const juce::String& originalFilename);
    juce::Image loadImage (const juce::String& originalFilename);
}
