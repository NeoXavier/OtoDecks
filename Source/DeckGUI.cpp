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
                  juce::AudioThumbnailCache &cacheToUse)
    : djAudioPlayer{ _djAudioPlayer },
      waveform (formatManagerToUse, cacheToUse)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

    // Buttons
	addAndMakeVisible (playButton);
	addAndMakeVisible (pauseButton);
	addAndMakeVisible (stopButton);


    // Sliders
	addAndMakeVisible (volSlider);
	addAndMakeVisible (speedSlider);

    volSlider.setRange (0, 10);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 20);

    speedSlider.setRange (0, 10);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 20);


    // Slider Labels
    addAndMakeVisible (volLabel);
    addAndMakeVisible (speedLabel);

    volLabel.setJustificationType(juce::Justification::centred);
    speedLabel.setJustificationType(juce::Justification::centred);


    // Waveform
	addAndMakeVisible (waveform);
    waveform.setInterceptsMouseClicks(false, false);

    addAndMakeVisible (positionSlider);
    positionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 90, 20);
    positionSlider.setAlpha(0);

    // Load Button
	addAndMakeVisible (loadButton);


    // Add listeners
	playButton.addListener (this);
	stopButton.addListener (this);
    pauseButton.addListener (this);
	volSlider.addListener (this);
	positionSlider.addListener (this);
	speedSlider.addListener (this);
	loadButton.addListener (this);


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

    int numOfRows = 8;
    int numOfCols = 2;
	float rowH = getHeight () / numOfRows;
    float colW = getWidth() / numOfCols;

	playButton.setBounds (0, 0, colW, rowH);
    pauseButton.setBounds (colW, 0, colW, rowH);
	stopButton.setBounds (0, rowH, getWidth (), rowH);

    volLabel.setBounds (0, rowH * 2, colW, rowH);
    speedLabel.setBounds (colW, rowH * 2, colW, rowH);

	volSlider.setBounds (0, rowH * 3, colW, rowH*2);
	speedSlider.setBounds (colW, rowH * 3, colW, rowH * 2);

    positionSlider.setBounds (0, rowH * 5, getWidth (), rowH*2);
	waveform.setBounds (0, rowH * 5, getWidth (), rowH * 2);


	loadButton.setBounds (0, rowH * 7, getWidth (), rowH);
}

void
DeckGUI::buttonClicked (juce::Button *button)
{
	if (button == &playButton)
		{
			djAudioPlayer->play ();
		}
    if (button == &pauseButton)
    {
        djAudioPlayer->pause ();
    }
	if (button == &stopButton) { djAudioPlayer->stop (); }
	if (button == &loadButton)
		{
			auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
			fChooser.launchAsync (
			    fileChooserFlags, [this] (const juce::FileChooser &chooser) {
				    auto chosenFile = chooser.getResult ();
				    djAudioPlayer->loadURL (juce::URL{ chosenFile });
                    DBG(djAudioPlayer->getMaxLength());
                    positionSlider.setRange(0, djAudioPlayer->getMaxLength());
				    waveform.loadURL (juce::URL{ chosenFile });
			    });
		}
}

void
DeckGUI::sliderValueChanged (juce::Slider *slider)
{
	if (slider == &volSlider) { djAudioPlayer->setGain (slider->getValue ()); }
	if (slider == &positionSlider)
		{
			djAudioPlayer->setPosition (slider->getValue ());
            DBG(slider->getValue());
		}
	if (slider == &speedSlider)
		{
			djAudioPlayer->setSpeed (slider->getValue ());
		}
}

bool
DeckGUI::isInterestedInFileDrag (const juce::StringArray &files)
{
	std::cout << "isInterestedInFileDrag" << std::endl;
	return true;
}

void
DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
	std::cout << "DeckGUI:: files dropped" << std::endl;
	if (files.size () == 1)
		{
			djAudioPlayer->loadURL (juce::URL{ juce::File{ files[0] } });
            DBG(djAudioPlayer->getMaxLength());
            positionSlider.setRange(0, djAudioPlayer->getMaxLength());
		}
}

void
DeckGUI::timerCallback ()
{
	// DBG ("DeckGUI::timerCallback");
	waveform.setPositionRelative (djAudioPlayer->getPositionRelative ());
}
