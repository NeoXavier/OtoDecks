/*
  ==============================================================================

    DeckGUI.cpp
    Created: 29 Jan 2024 5:45:11pm
    Author:  Xavier Neo

  ==============================================================================
*/

#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include <JuceHeader.h>

//==============================================================================
DeckGUI::DeckGUI (DJAudioPlayer *_djAudioPlayer,
                  juce::AudioFormatManager &formatManagerToUse,
                  juce::AudioThumbnailCache &cacheToUse,
                  PlaylistComponent *_playlistComponent, juce::String _channel)
    : djAudioPlayer{ _djAudioPlayer },
      waveform (formatManagerToUse, cacheToUse),
      playlistComponent{ _playlistComponent }, channel{ _channel }
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	// Buttons
	addAndMakeVisible (playButton);
	addAndMakeVisible (nextButton);
	addAndMakeVisible (resetButton);

	// Sliders
	addAndMakeVisible (volSlider);
	addAndMakeVisible (speedSlider);

	volSlider.setRange (0, 1);
	volSlider.setValue (0.8);
	volSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBarVertical);
	volSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);

	speedSlider.setRange (0, 10, 0.5);
	speedSlider.setSliderStyle (juce::Slider::SliderStyle::Rotary);
	speedSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 90, 20);

	// Slider Labels
	addAndMakeVisible (volLabel);
	volLabel.setJustificationType (juce::Justification::centred);
	volLabel.attachToComponent (&volSlider, false);
	addAndMakeVisible (speedLabel);
	speedLabel.attachToComponent (&speedSlider, false);
	speedLabel.setJustificationType (juce::Justification::centred);

	// Waveform
	addAndMakeVisible (waveform);
	waveform.setInterceptsMouseClicks (false, false);

	addAndMakeVisible (positionSlider);
	positionSlider.setSliderStyle (juce::Slider::SliderStyle::LinearBar);
	positionSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 90, 20);
	positionSlider.setAlpha (0);

	// Queue list
	addAndMakeVisible (queueComponent);
	queueComponent.getHeader ().addColumn ("Queue", 1, 100);
	queueComponent.setModel (this);

	// Add listeners
	playButton.addListener (this);
	resetButton.addListener (this);
	nextButton.addListener (this);
	volSlider.addListener (this);
	positionSlider.addListener (this);
	speedSlider.addListener (this);

	startTimer (200);
}

DeckGUI::~DeckGUI () { stopTimer (); }

void
DeckGUI::paint (juce::Graphics &g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	g.fillAll (getLookAndFeel ().findColour (
	    juce::ResizableWindow::backgroundColourId));  // clear the background
}

void
DeckGUI::resized ()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

	int numOfRows = 6;
	int numOfCols = 3;
	float rowH = getHeight () / numOfRows;
	float colW = getWidth () / numOfCols;
	int padding = 20;

	resetButton.setBounds (0, 0, colW, rowH);
	playButton.setBounds (colW, 0, colW, rowH);
	nextButton.setBounds (colW * 2, 0, colW, rowH);

	positionSlider.setBounds (0, rowH, getWidth (), rowH * 2);
	waveform.setBounds (0, rowH, getWidth (), rowH * 2);

    queueComponent.setBounds (0, rowH * 3, colW, rowH * 2);
    speedSlider.setBounds (colW, rowH * 3 + padding, colW, rowH * 2 - padding);
	volSlider.setBounds (colW * 2, rowH * 3 + padding, colW,
	                     rowH * 2 - padding);
}

void
DeckGUI::buttonClicked (juce::Button *button)
{
	if (button == &playButton)
		{
			if (isPlaying) { djAudioPlayer->play (); }
			else { djAudioPlayer->pause (); }
		}
	if (button == &nextButton) {}
	{
		djAudioPlayer->reset ();
	}
	if (button == &resetButton) { djAudioPlayer->reset (); }
}

void
DeckGUI::sliderValueChanged (juce::Slider *slider)
{
	if (slider == &volSlider) { djAudioPlayer->setGain (slider->getValue ()); }
	if (slider == &positionSlider)
		{
			djAudioPlayer->setPosition (slider->getValue ());
			DBG (slider->getValue ());
		}
	if (slider == &speedSlider)
		{
			djAudioPlayer->setSpeed (slider->getValue ());
		}
}

void
DeckGUI::timerCallback ()
{
	// DBG ("DeckGUI::timerCallback");
	waveform.setPositionRelative (djAudioPlayer->getPositionRelative ());
}

int
DeckGUI::getNumRows ()
{
    if (channel == "Left") { return playlistComponent->leftFiles.size (); }
    if (channel == "Right") { return playlistComponent->rightFiles.size (); }
    return 0;
}

void
DeckGUI::paintRowBackground (juce::Graphics &g, int rowNumber, int width,
                             int height, bool rowIsSelected)
{
	if (rowIsSelected) { g.fillAll (juce::Colours::orange); }
	else { g.fillAll (juce::Colours::darkgrey); }
};

void
DeckGUI::paintCell (juce::Graphics &g, int rowNumber, int columnId, int width,
                    int height, bool rowIsSelected)
{
	if (columnId == 1)
		{
			if (channel == "Left")
				{
					g.drawText (
					    playlistComponent->leftFiles[rowNumber].getFileName (),
					    2, 0, width - 4, height,
					    juce::Justification::centredLeft, true);
				}
			else if (channel == "Right")
				{
					g.drawText (playlistComponent->rightFiles[rowNumber]
					                .getFileName (),
					            2, 0, width - 4, height,
					            juce::Justification::centredLeft, true);
				}
		}
};
