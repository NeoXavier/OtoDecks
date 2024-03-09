/*
  ==============================================================================

    DeckGUI.h
    Created: 29 Jan 2024 5:45:11pm
    Author:  Xavier Neo

  ==============================================================================
*/

#pragma once

#include "DJAudioPlayer.h"
#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
/*
 */
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce:: Timer
{
  public:
	DeckGUI (DJAudioPlayer *_djAudioPlayer,
            juce::AudioFormatManager& formatManagerToUse, 
            juce::AudioThumbnailCache& cacheToUse);
	~DeckGUI () override;


	//==============================================================================
	void paint (juce::Graphics &) override;
	void resized () override;

    // Button::Listener
	void buttonClicked (juce::Button *button) override;

    // Slider::Listener
	void sliderValueChanged (juce::Slider *slider) override;

    // FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Timer
    void timerCallback() override;

  private:
    // Buttons
	juce::TextButton playButton{ "PLAY" };
    juce::TextButton pauseButton{ "PAUSE" };
	juce::TextButton stopButton{ "STOP" };
	juce::TextButton loadButton{ "LOAD" };

    // Sliders
	juce::Slider volSlider;
	juce::Slider positionSlider;
	juce::Slider speedSlider;

    juce::Label volLabel{ "Volume Label", "Volume" };
    juce::Label speedLabel{ "Speed Label", "Speed" };

    // File Chooser
	// https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
	juce::FileChooser fChooser{ "Select a file..." };


	DJAudioPlayer *djAudioPlayer;

    WaveformDisplay waveform;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
