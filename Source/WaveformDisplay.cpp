/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 30 Jan 2024 12:53:58pm
    Author:  Xavier Neo

  ==============================================================================
*/

#include "WaveformDisplay.h"
#include <JuceHeader.h>

//==============================================================================
WaveformDisplay::WaveformDisplay (juce::AudioFormatManager &formatManagerToUse,
                                  juce::AudioThumbnailCache &cacheToUse)
    : audioThumbnail (1000, formatManagerToUse, cacheToUse),
      fileLoaded (false), position (0)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
}

WaveformDisplay::~WaveformDisplay ()
{
	audioThumbnail.addChangeListener (this);
}

void
WaveformDisplay::paint (juce::Graphics &g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
    g.setColour(juce::Colours::orchid);

	if (fileLoaded)
		{
			audioThumbnail.drawChannel (g, getLocalBounds (), 0,
			                            audioThumbnail.getTotalLength (), 0,
			                            1.0f);
			g.setColour (juce::Colours::lightgreen);
            g.drawLine(position * getWidth(), 0, position * getWidth(), getHeight(), 2.0f);

            // Display file name
            g.setFont(10.0f);
            g.drawText(fileName, getLocalBounds(), juce::Justification::centred, true);
		}
	else
		{
			g.setFont (20.0f);
			g.drawText ("No file loaded...", getLocalBounds (),
			            juce::Justification::centred,
			            true);  // draw some placeholder text
		}
}

void
WaveformDisplay::resized ()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
}

void
WaveformDisplay::loadURL (juce::URL audioURL)
{
	audioThumbnail.clear ();
	fileLoaded
	    = audioThumbnail.setSource (new juce::URLInputSource (audioURL));
    fileName = audioURL.getFileName();
}

void
WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster *source)
{
	std::cout << "wfd: change recieved!" << std::endl;
	repaint ();
}

void WaveformDisplay::setPositionRelative (double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
