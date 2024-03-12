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
	queueComponent.getHeader ().addColumn ("Queue", 1, 180);
	queueComponent.autoSizeColumn (1);
	queueComponent.setModel (this);

	// Add listeners
	playButton.addListener (this);
	resetButton.addListener (this);
	nextButton.addListener (this);
	volSlider.addListener (this);
	positionSlider.addListener (this);
	speedSlider.addListener (this);

    // Set colours of sliders
	getLookAndFeel ().setColour (juce::Slider::thumbColourId,
	                             juce::Colours::mediumpurple);  // dial
	getLookAndFeel ().setColour (juce::Slider::trackColourId,
	                             juce::Colours::mediumpurple);  // body
	getLookAndFeel ().setColour (juce::Slider::rotarySliderFillColourId,
	                             juce::Colours::lightslategrey);  // body

	startTimer (200);
}

DeckGUI::~DeckGUI () { stopTimer (); }

void
DeckGUI::paint (juce::Graphics &g)
{
	g.fillAll (getLookAndFeel ().findColour (
	    juce::ResizableWindow::backgroundColourId));  // clear the background
}

void
DeckGUI::resized ()
{
    
	int numOfRows = 6;
	int numOfCols = 6;
	float rowH = getHeight () / numOfRows;
	float colW = getWidth () / numOfCols;
	int padding = 20;

    // Layout of DeckGUI
	resetButton.setBounds (padding, padding, colW * 2 - (2 * padding),
	                       rowH - (2 * padding));
	playButton.setBounds (colW * 2 + padding, padding,
	                      colW * 2 - (2 * padding), rowH - (2 * padding));
	nextButton.setBounds (colW * 4 + padding, padding,
	                      colW * 2 - (2 * padding), rowH - (2 * padding));

	positionSlider.setBounds (0, rowH, getWidth (), rowH * 2);
	waveform.setBounds (0, rowH, getWidth (), rowH * 2);

	queueComponent.setBounds (0, rowH * 3, colW * 3, rowH * 3);
	speedSlider.setBounds (colW * 3, rowH * 3 + padding, colW * 2,
	                       rowH * 3 - padding);
	volSlider.setBounds (colW * 5, rowH * 3 + padding, colW,
	                     rowH * 3 - padding);
}

void
DeckGUI::buttonClicked (juce::Button *button)
{
    // Event listener for play button
	if (button == &playButton)
		{
            // Check if the play button is from the left deck or right deck
			if (channel == "Left") { files = &playlistComponent->leftFiles; }
			else { files = &playlistComponent->rightFiles; }

            // Check if the file is loaded
			if (djAudioPlayer->fileLoaded)
				{
                    // If the file is playing, pause it. If it is paused, play it.
					if (isPlaying)
						{
							djAudioPlayer->pause ();
							isPlaying = false;
						}
					else
						{
							djAudioPlayer->play ();
							isPlaying = true;
						}
				}

            // If the file is not loaded, load the file and play it
			else
				{
					juce::File file = files->at (0);
					djAudioPlayer->loadURL (juce::URL{ file });

					positionSlider.setRange (0,
					                         djAudioPlayer->getMaxLength ());

					waveform.loadURL (juce::URL{ file });

					files->erase (files->begin ());
					queueComponent.updateContent ();

					djAudioPlayer->play ();
					isPlaying = true;
				}
		}

    // Event Listener for next button
	if (button == &nextButton)
		{
			if (channel == "Left") { files = &playlistComponent->leftFiles; }
			else { files = &playlistComponent->rightFiles; }

			juce::File file = files->at (0);

			djAudioPlayer->loadURL (juce::URL{ file });

			positionSlider.setRange (0, djAudioPlayer->getMaxLength ());

			waveform.loadURL (juce::URL{ file });

			files->erase (files->begin ());
			queueComponent.updateContent ();

			djAudioPlayer->play ();
		}
    
    // Event Listener for reset button
	if (button == &resetButton) { djAudioPlayer->reset (); }
}

void
DeckGUI::sliderValueChanged (juce::Slider *slider)
{
    // Event listener for volume slider
	if (slider == &volSlider) { djAudioPlayer->setGain (slider->getValue ()); }

    // Event listener for position slider
	if (slider == &positionSlider)
		{
			djAudioPlayer->setPosition (slider->getValue ());
		}

    // Event listener for speed slider
	if (slider == &speedSlider)
		{
			djAudioPlayer->setSpeed (slider->getValue ());
		}
}

void
DeckGUI::timerCallback ()
{
	waveform.setPositionRelative (djAudioPlayer->getPositionRelative ());
}

int
DeckGUI::getNumRows ()
{
    // Get the number of files in the queue
    // Based of if the deck is left or right
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
    // Fill the queue list with the respective file list
	if (columnId == 1)
		{
            // Left queue
			if (channel == "Left")
				{
					g.drawText (
					    playlistComponent->leftFiles[rowNumber].getFileName (),
					    2, 0, width - 4, height,
					    juce::Justification::centredLeft, true);
				}
            // Right queue
			else if (channel == "Right")
				{
					g.drawText (playlistComponent->rightFiles[rowNumber]
					                .getFileName (),
					            2, 0, width - 4, height,
					            juce::Justification::centredLeft, true);
				}
		}
};
