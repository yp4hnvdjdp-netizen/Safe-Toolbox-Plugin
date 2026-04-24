#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace SafeToolboxTheme
{
    inline juce::Colour backgroundTop()               { return juce::Colour (0xff11151b); }
    inline juce::Colour backgroundBottom()            { return juce::Colour (0xff07080b); }
    inline juce::Colour shellFill()                   { return juce::Colour (0xff1d2230); }
    inline juce::Colour shellOutline()                { return juce::Colour (0xff2c3440); }
    inline juce::Colour panelFill()                   { return juce::Colour (0xff12161d); }
    inline juce::Colour panelOutline()                { return juce::Colour (0xff2a313d); }
    inline juce::Colour safeModeFill()                { return juce::Colour (0xff11151b); }
    inline juce::Colour safeModeOutline()             { return juce::Colour (0xff2b323d); }
    inline juce::Colour meterFill()                   { return juce::Colour (0xff10141a); }
    inline juce::Colour meterOutline()                { return juce::Colour (0xff2b323d); }
    inline juce::Colour neutralAccent()               { return juce::Colour (0xff6a717d); }
    inline juce::Colour driveAccent()                 { return juce::Colour (0xffff7a59); }
    inline juce::Colour filterAccent()                { return juce::Colour (0xffffd34f); }
    inline juce::Colour delayAccent()                 { return juce::Colour (0xff78e7ff); }
    inline juce::Colour reverbAccent()                { return juce::Colour (0xffcc7bff); }
    inline juce::Colour wobbleAccent()                { return juce::Colour (0xff7eb8ff); }
    inline juce::Colour widthAccent()                 { return juce::Colour (0xfff072c2); }
    inline juce::Colour safeGreen()                   { return juce::Colour (0xff67f2a3); }
    inline juce::Colour mutedText()                   { return juce::Colour (0xffa7abb2); }
    inline juce::Colour lightText()                   { return juce::Colours::white.withAlpha (0.92f); }

    inline constexpr int panelCornerRadius = 18;
    inline constexpr int controlCornerRadius = 8;
}
