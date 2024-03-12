#pragma once

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put
   all your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
  public:
	//==============================================================================
	MainComponent ();
	~MainComponent () override;

    // AudioAppComponent Virtual Functions
	//==============================================================================
	void prepareToPlay (int samplesPerBlockExpected,
	                    double sampleRate) override;
	void getNextAudioBlock (
	    const juce::AudioSourceChannelInfo &bufferToFill) override;
	void releaseResources () override;

    // Component Virtual Functions
	//==============================================================================
	void paint (juce::Graphics &g) override;
	void resized () override;

  private:
	//==============================================================================
	// Your private member variables go here...

	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbCache{ 100 };

    // Playlist table
    PlaylistComponent playlistComponent;

    // Each deck has a DJAudioPlayer and a DeckGUI
	DJAudioPlayer player1{ formatManager };
	DeckGUI deck1{ &player1, formatManager, thumbCache, &playlistComponent, "Left"};

	DJAudioPlayer player2{ formatManager };
	DeckGUI deck2{ &player2, formatManager, thumbCache, &playlistComponent, "Right"};

	juce::MixerAudioSource mixerSource;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
