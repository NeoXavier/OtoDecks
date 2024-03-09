/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 31 Jan 2024 1:36:18pm
    Author:  Xavier Neo

  ==============================================================================
*/

#include "PlaylistComponent.h"
#include <JuceHeader.h>

//==============================================================================
PlaylistComponent::PlaylistComponent ()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	addAndMakeVisible (tableComponent);
	tableComponent.getHeader ().addColumn ("Track title", 1, 400);
	tableComponent.getHeader ().addColumn ("", 2, 200);
    tableComponent.setModel(this);

    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
}

PlaylistComponent::~PlaylistComponent () {}

void
PlaylistComponent::paint (juce::Graphics &g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	g.fillAll (getLookAndFeel ().findColour (
	    juce::ResizableWindow::backgroundColourId));  // clear the background

	g.setColour (juce::Colours::grey);
	g.drawRect (getLocalBounds (), 1);  // draw an outline around the component

	g.setColour (juce::Colours::white);
	g.setFont (14.0f);
	g.drawText ("PlaylistComponent", getLocalBounds (),
	            juce::Justification::centred,
	            true);  // draw some placeholder text

}

void
PlaylistComponent::resized ()
{

	tableComponent.setBounds (0, 0, getWidth (), getHeight ());

}

int
PlaylistComponent::getNumRows ()
{
    return trackTitles.size();
}

void
PlaylistComponent::paintRowBackground (juce::Graphics &g, int rowNumber,
                                       int width, int height,
                                       bool rowIsSelected)
{
	// just hightlight selected rows
	if (rowIsSelected) { g.fillAll (juce::Colours::orange); }
	else { g.fillAll (juce::Colours::darkgrey); }
}

void
PlaylistComponent::paintCell (juce::Graphics &g, int rowNumber, int columnId,
                              int width, int height, bool rowIsSelected)
{
	g.drawText (trackTitles[rowNumber], 2, 0, width - 4, height,
	            juce::Justification::centredLeft, true);
}

juce::Component* 
PlaylistComponent::refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{

    if (columnId == 2)
    {

        if (existingComponentToUpdate == nullptr)
        {

            juce::TextButton* btn = new juce::TextButton("Play");
            existingComponentToUpdate = btn;
            btn->addListener(this);

            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);

        }

    }

    return existingComponentToUpdate;

}

void PlaylistComponent::buttonClicked (juce::Button* button)
{
    DBG("PlaylistComponent::buttonClicked ");

    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked" << trackTitles[id]);
}
