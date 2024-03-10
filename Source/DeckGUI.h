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
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
 */
class PlaylistComponent;

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce:: Timer,
                public juce::TableListBoxModel
{
  public:
	DeckGUI (DJAudioPlayer *_djAudioPlayer,
            juce::AudioFormatManager& formatManagerToUse, 
            juce::AudioThumbnailCache& cacheToUse,
            PlaylistComponent* _playlistComponent,
            juce::String _channel);
	~DeckGUI () override;


	//==============================================================================
	void paint (juce::Graphics &) override;
	void resized () override;

    // Button::Listener
	void buttonClicked (juce::Button *button) override;

    // Slider::Listener
	void sliderValueChanged (juce::Slider *slider) override;

    // Timer
    void timerCallback() override;

    // TableListBoxModel
	int getNumRows () override;
	void paintRowBackground (juce::Graphics &g, int rowNumber, int width,
	                         int height, bool rowIsSelected) override;
	void paintCell (juce::Graphics &g, int rowNumber, int columnId, int width,
	                int height, bool rowIsSelected) override;

    //Queue List
    juce::TableListBox queueComponent;

  private:
    // Buttons
	juce::TextButton playButton{ "PLAY/PAUSE" };
	juce::TextButton resetButton{ "RESET" };
    juce::TextButton nextButton{ "NEXT" };

    // Sliders
	juce::Slider volSlider;
	juce::Slider positionSlider;
	juce::Slider speedSlider;

    juce::Label volLabel{ "Volume Label", "Volume" };
    juce::Label speedLabel{ "Speed Label", "Speed" };

	DJAudioPlayer *djAudioPlayer;

    WaveformDisplay waveform;

    PlaylistComponent* playlistComponent;

    juce::String channel;

    bool isPlaying = false;
    
    std::vector<juce::File>* files;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
