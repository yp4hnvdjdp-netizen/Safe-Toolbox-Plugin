#include "RotaryKnobLookAndFeel.h"
#include "../Assets/AssetIds.h"
#include "../Assets/AssetLoader.h"
#include "PluginColours.h"

namespace
{
    juce::String shortCaptionForSlider (const juce::String& name)
    {
        const auto lower = name.trim().toLowerCase();

        if (lower == "input gain") return "IN";
        if (lower == "drive amount") return "AMOUNT";
        if (lower == "drive tone") return "TONE";
        if (lower == "drive mix") return "MIX";
        if (lower == "drive output") return "OUT";
        if (lower == "filter cutoff") return "CUTOFF";
        if (lower == "filter resonance") return "RES";
        if (lower == "filter drive") return "DRIVE";
        if (lower == "delay time") return "TIME";
        if (lower == "delay feedback") return "FEED";
        if (lower == "delay damping") return "DAMP";
        if (lower == "delay mix") return "MIX";
        if (lower == "reverb size") return "SIZE";
        if (lower == "reverb decay") return "DECAY";
        if (lower == "reverb damping") return "DAMP";
        if (lower == "reverb width") return "WIDTH";
        if (lower == "reverb mix") return "MIX";
        if (lower == "wobble rate") return "RATE";
        if (lower == "wobble depth") return "DEPTH";
        if (lower == "wobble shape") return "SHAPE";
        if (lower == "wobble stereo phase") return "PHASE";
        if (lower == "wobble mix") return "MIX";
        if (lower == "stereo width") return "WIDTH";
        if (lower == "mono bass frequency") return "MONO";
        if (lower == "mid / side balance") return "BAL";
        if (lower == "width high cut") return "HIGH CUT";
        if (lower == "output gain") return "GAIN";
        if (lower == "safety ceiling") return "CEILING";
        if (lower == "limiter strength") return "LIMIT";
        if (lower == "limiter release") return "REL";
        if (lower == "soft clip") return "CLIP";

        auto firstWord = name.upToFirstOccurrenceOf (" ", false, false);
        if (firstWord.isNotEmpty())
            return firstWord.toUpperCase();

        return name.toUpperCase();
    }

    const juce::Drawable* knobFace()
    {
        static const auto face = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::knobFaceSvg);
        return face.get();
    }

    const juce::Drawable* knobHalo()
    {
        static const auto halo = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::knobHaloSvg);
        return halo.get();
    }

    const juce::Drawable* knobCap()
    {
        static const auto cap = SafeToolboxAssets::loadDrawable (SafeToolboxAssets::Ids::knobCapSvg);
        return cap.get();
    }
}

void RotaryKnobLookAndFeel::drawRotarySlider (juce::Graphics& g,
                                              int x, int y, int width, int height,
                                              float sliderPos, float rotaryStartAngle,
                                              float rotaryEndAngle, juce::Slider& slider)
{
    juce::ignoreUnused (slider);

    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (2.0f);
    auto knobSize = juce::jmin (bounds.getWidth(), bounds.getHeight());
    auto knob = bounds.withSizeKeepingCentre (knobSize, knobSize);
    auto centre = knob.getCentre();
    auto radius = knobSize * (role == KnobRole::Primary ? 0.46f : 0.40f);
    auto outline = knob.expanded (0.5f);
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour (juce::Colour (0xff090b0f).withAlpha (role == KnobRole::Primary ? 0.92f : 0.88f));
    g.fillEllipse (knob);
    g.setColour (juce::Colour (0xff242932).withAlpha (role == KnobRole::Primary ? 0.95f : 0.80f));
    g.drawEllipse (outline, role == KnobRole::Primary ? 1.2f : 0.95f);

    if (const auto* halo = knobHalo())
    {
        const auto haloInset = role == KnobRole::Primary ? 0.010f : 0.024f;
        halo->drawWithin (g, knob.reduced (knobSize * haloInset), juce::RectanglePlacement::centred, slider.isMouseOverOrDragging() ? 1.0f : (role == KnobRole::Primary ? 0.78f : 0.62f));
    }

    if (const auto* face = knobFace())
    {
        face->drawWithin (g, knob.reduced (knobSize * (role == KnobRole::Primary ? 0.035f : 0.075f)), juce::RectanglePlacement::centred, role == KnobRole::Primary ? 0.90f : 0.74f);
    }

    g.setColour (juce::Colours::white.withAlpha (role == KnobRole::Primary ? 0.08f : 0.05f));
    g.fillEllipse (knob.reduced (knobSize * (role == KnobRole::Primary ? 0.18f : 0.22f)).translated (-knobSize * 0.07f, -knobSize * 0.07f));

    juce::Path arc;
    arc.addCentredArc (centre.x, centre.y, radius, radius, 0.0f, rotaryStartAngle, angle, true);
    g.setColour (accentColour.withAlpha (role == KnobRole::Primary ? 0.98f : 0.78f));
    g.strokePath (arc, juce::PathStrokeType (role == KnobRole::Primary ? 4.8f : 3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto pointer = juce::Line<float> (centre,
                                      juce::Point<float> (centre.x + std::cos (angle) * radius * (role == KnobRole::Primary ? 0.94f : 0.88f),
                                                          centre.y + std::sin (angle) * radius * (role == KnobRole::Primary ? 0.94f : 0.88f)));
    g.setColour (juce::Colours::white.withAlpha (role == KnobRole::Primary ? 0.95f : 0.84f));
    g.drawLine (pointer, role == KnobRole::Primary ? 2.7f : 1.9f);

    g.setColour (juce::Colours::white.withAlpha (0.22f));
    g.drawEllipse (knob.reduced (knobSize * 0.30f), 1.0f);

    if (const auto* cap = knobCap())
    {
        cap->drawWithin (g, knob.reduced (knobSize * 0.30f), juce::RectanglePlacement::centred, 0.95f);
    }

    const auto caption = shortCaptionForSlider (slider.getName());

    if (knobSize >= 54.0f)
    {
        auto captionArea = knob.removeFromTop (juce::jlimit (10, 16, (int) std::round (knobSize * 0.14f))).reduced (2, 0);
        g.setColour (juce::Colours::white.withAlpha (role == KnobRole::Primary ? 0.88f : 0.70f));
        g.setFont (juce::FontOptions (role == KnobRole::Primary ? 8.8f : 7.8f, juce::Font::bold));
        g.drawFittedText (caption, captionArea.toNearestInt(), juce::Justification::centred, 1);
    }

    if (knobSize >= 68.0f && role == KnobRole::Primary)
    {
        auto valueArea = knob.removeFromBottom (juce::jlimit (12, 16, (int) std::round (knobSize * 0.15f))).reduced (0, 1);
        g.setColour (juce::Colours::white.withAlpha (role == KnobRole::Primary ? 0.94f : 0.72f));
        g.setFont (juce::FontOptions (role == KnobRole::Primary ? 9.8f : 8.6f));
        g.drawFittedText (slider.getTextFromValue (slider.getValue()), valueArea.toNearestInt(), juce::Justification::centred, 1);
    }
}
