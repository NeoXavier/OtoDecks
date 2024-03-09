/*
  ==============================================================================

    WaveformDisplay.h
    Created: 30 Jan 2024 12:53:58pm
    Author:  Xavier Neo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void loadURL(juce::URL audioURL);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    /** set the relative position of the playhead */
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumbnail;

    bool fileLoaded;

    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
