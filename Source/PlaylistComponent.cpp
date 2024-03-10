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
	tableComponent.getHeader ().addColumn ("L Queue Buttons", 2, 200);
	tableComponent.getHeader ().addColumn ("R Queue Buttons", 3, 200);
	tableComponent.setModel (this);

	// Load Button
	addAndMakeVisible (loadButton);
	loadButton.addListener (this);

	auto start
	    = juce::File::getSpecialLocation (juce::File::currentExecutableFile);
	while (start.exists () && !start.isRoot ())
		{
			if (start.getSiblingFile ("tracks").exists ())
				{
					start = start.getSiblingFile ("tracks");
					break;
				}
			start = start.getParentDirectory ();
		}

	juce::Array<juce::File> fileArray;
	start.findChildFiles (fileArray, juce::File::findFiles, false);
	for (const auto &file : fileArray)
		{
			DBG (file.getFullPathName ());
			loadedFiles.push_back (file);
			loadedTitles.push_back (file.getFileName ());
		}
	tableComponent.updateContent ();
}

PlaylistComponent::~PlaylistComponent () {}

void
PlaylistComponent::paint (juce::Graphics &g)
{

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

	tableComponent.setBounds (0, 0, getWidth (), getHeight () - 20);

	loadButton.setBounds (0, getHeight () - 20, getWidth (), 20);
}

int
PlaylistComponent::getNumRows ()
{
	return loadedTitles.size ();
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
	if (columnId == 1)
		{
			g.drawText (loadedTitles[rowNumber], 2, 0, width - 4, height,
			            juce::Justification::centredLeft, true);
		}
}

juce::Component *
PlaylistComponent::refreshComponentForCell (
    int rowNumber, int columnId, bool isRowSelected,
    Component *existingComponentToUpdate)
{

	if (columnId == 2)
		{
            // If the cell has no button OR
            // if the cell has a button that belongs to another row, as TableListBox only renders 10 rows at a time and resuses the deleted buttons for the new rows
			if (existingComponentToUpdate == nullptr || existingComponentToUpdate->getComponentID() != juce::String{std::to_string(rowNumber) + "L"})
            {
					juce::TextButton *btn
					    = new juce::TextButton{ "Add to L Queue" };
					juce::String id{ std::to_string (rowNumber) + "L" };
					btn->setComponentID (id);
					btn->addListener (this);
					existingComponentToUpdate = btn;
				}
		}

	if (columnId == 3)
		{
			if (existingComponentToUpdate == nullptr || existingComponentToUpdate->getComponentID() != juce::String{std::to_string(rowNumber) + "R"})
				{
					juce::TextButton *btn
					    = new juce::TextButton{ "Add to R Queue" };
					juce::String id{ std::to_string (rowNumber) + "R" };
					btn->setComponentID (id);
					btn->addListener (this);
					existingComponentToUpdate = btn;
				}
		}
	return existingComponentToUpdate;
}

void
PlaylistComponent::buttonClicked (juce::Button *button)
{
	if (button == &loadButton)
		{
			auto fileChooserFlags
			    = juce::FileBrowserComponent::openMode
			      | juce::FileBrowserComponent::canSelectFiles;
			fChooser.launchAsync (
			    fileChooserFlags, [this] (const juce::FileChooser &chooser) {
				    juce::File chosenFile = chooser.getResult ();
				    if (chosenFile.exists ())
					    {
						    loadedFiles.push_back (chosenFile);
						    loadedTitles.push_back (chosenFile.getFileName ());
						    tableComponent.updateContent ();
					    }
			    });
		}
	else
		{
			juce::String id = button->getComponentID ().toStdString ();
			DBG (button->getComponentID ());
			if (id.getLastCharacter () == 'L')
				{
					std::cout << "Add to L Queue" << std::endl;
					int fileIndex = id.dropLastCharacters (1).getIntValue ();
					leftFiles.push_back (loadedFiles[fileIndex]);
					if (leftDeck != nullptr)
						{
							leftDeck->queueComponent.updateContent ();
						}
				}
			else
				{
					std::cout << "Add to R Queue" << std::endl;
					int fileIndex = id.dropLastCharacters (1).getIntValue ();
					rightFiles.push_back (loadedFiles[fileIndex]);
					if (rightDeck != nullptr)
						{
							rightDeck->queueComponent.updateContent ();
						}
				}
		}
}

bool
PlaylistComponent::isInterestedInFileDrag (const juce::StringArray &files)
{
	std::cout << "isInterestedInFileDrag" << std::endl;
	return true;
}

void
PlaylistComponent::filesDropped (const juce::StringArray &files, int x, int y)
{
	std::cout << "DeckGUI:: files dropped" << std::endl;
	if (files.size () == 1)
		{
			DBG (juce::URL{ juce::File{ files[0] } }.getFileName ());
			juce::File loadedFile{ files[0] };
			loadedFiles.push_back (loadedFile);
			loadedTitles.push_back (loadedFile.getFileName ());
		}
	tableComponent.updateContent ();
}

void
PlaylistComponent::setDeck (DeckGUI *deck, juce::String id)
{
	if (id == "Left") { leftDeck = deck; }
	else { rightDeck = deck; }
}
