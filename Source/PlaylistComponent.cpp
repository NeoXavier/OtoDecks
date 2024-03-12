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

	// Playlist
	addAndMakeVisible (tableComponent);
	tableComponent.getHeader ().addColumn ("Track title", 1, 400);
	tableComponent.getHeader ().addColumn ("L Queue Buttons", 2, 200);
	tableComponent.getHeader ().addColumn ("R Queue Buttons", 3, 200);
	tableComponent.setModel (this);

	// Set scrollbar colour
	getLookAndFeel ().setColour (juce::ScrollBar::thumbColourId,
	                             juce::Colours::mediumpurple);  // dial

	// Load Button
	addAndMakeVisible (loadButton);
	loadButton.addListener (this);

	// Search bar and label
	addAndMakeVisible (searchBar);
	searchBar.addListener (this);
	addAndMakeVisible (searchLabel);
	searchLabel.setJustificationType (juce::Justification::left);

	// Load files from the tracks directory
	//==========================================================================
	// Get the directory of the current executable
	auto directory
	    = juce::File::getSpecialLocation (juce::File::currentExecutableFile);

	// Walk down the directory tree to find the tracks directory
	while (directory.exists () && !directory.isRoot ())
		{
			if (directory.getSiblingFile ("tracks").exists ())
				{
					directory = directory.getSiblingFile ("tracks");
					break;
				}
			directory = directory.getParentDirectory ();
		}

	juce::Array<juce::File> fileArray;

	// Get all files in the tracks directory
	directory.findChildFiles (fileArray, juce::File::findFiles, false);

	// Add all files to the playlist
	for (const auto &file : fileArray)
		{
			loadedFiles.push_back (file);
			currentTitles.push_back (file.getFileName ());
		}

	// Show all the files loaded in the playlist first
	currentFiles = loadedFiles;

	// Refresh table
	tableComponent.updateContent ();
}

PlaylistComponent::~PlaylistComponent () {}

void
PlaylistComponent::paint (juce::Graphics &g)
{

	g.fillAll (getLookAndFeel ().findColour (
	    juce::ResizableWindow::backgroundColourId));  // clear the background

	g.setColour (juce::Colours::white);
	g.setFont (14.0f);
	g.drawText ("PlaylistComponent", getLocalBounds (),
	            juce::Justification::centred,
	            true);  // draw some placeholder text
}

void
PlaylistComponent::resized ()
{
	// Define layout of components
	searchLabel.setBounds (0, 0, 100, 25);
	searchBar.setBounds (100, 0, getWidth () - 100, 25);
	tableComponent.setBounds (0, 25, getWidth (), getHeight () - 45);
	loadButton.setBounds (0, getHeight () - 20, getWidth (), 20);
}

int
PlaylistComponent::getNumRows ()
{
	// Number of rows is the number of titles to be displayed
	return currentTitles.size ();
}

void
PlaylistComponent::paintRowBackground (juce::Graphics &g, int rowNumber,
                                       int width, int height,
                                       bool rowIsSelected)
{
	// Hightlight selected rows
	if (rowIsSelected) { g.fillAll (juce::Colours::mediumorchid); }
	else { g.fillAll (juce::Colours::darkgrey); }
}

void
PlaylistComponent::paintCell (juce::Graphics &g, int rowNumber, int columnId,
                              int width, int height, bool rowIsSelected)
{
	// Draw each title in the first column
	if (columnId == 1)
		{
			g.drawText (currentTitles[rowNumber], 2, 0, width - 4, height,
			            juce::Justification::centredLeft, true);
		}
}

// Function that is called when the table is refreshed, used to update custom
// components in cells
juce::Component *
PlaylistComponent::refreshComponentForCell (
    int rowNumber, int columnId, bool isRowSelected,
    Component *existingComponentToUpdate)
{

	// Adding buttons for queing songs in the left and right decks, in the
	// second and third columns
	if (columnId == 2)
		{
			/*
			    If the cell has no button OR
			    if the cell has a button that belongs to another row, as
			    TableListBox only renders 10 rows at a time and resuses the
			    deleted buttons for the new rows
			*/
			if (existingComponentToUpdate == nullptr
			    || existingComponentToUpdate->getComponentID ()
			           != juce::String{ std::to_string (rowNumber) + "L" })
				{
					juce::TextButton *btn
					    = new juce::TextButton{ "Add to L Queue" };
					juce::String id{ std::to_string (rowNumber) + "L" };
					btn->setComponentID (id);
					btn->addListener (this);
					existingComponentToUpdate = btn;
				}
		}

	// Same logic as left column above but for right column
	if (columnId == 3)
		{
			if (existingComponentToUpdate == nullptr
			    || existingComponentToUpdate->getComponentID ()
			           != juce::String{ std::to_string (rowNumber) + "R" })
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
	// Load button event handler
	if (button == &loadButton)
		{
			// Open file chooser
			auto fileChooserFlags
			    = juce::FileBrowserComponent::openMode
			      | juce::FileBrowserComponent::canSelectFiles;
			fChooser.launchAsync (
			    fileChooserFlags, [this] (const juce::FileChooser &chooser) {
				    // Load files to the respective vectors if a valid file is
				    // selected
				    juce::File chosenFile = chooser.getResult ();
				    if (chosenFile.exists ())
					    {
						    loadedFiles.push_back (chosenFile);
						    currentFiles.push_back (chosenFile);
						    currentTitles.push_back (
						        chosenFile.getFileName ());

						    // Refresh table
						    tableComponent.updateContent ();
					    }
			    });
		}

	// All other buttons, i.e add to queue buttons
	else
		{
			juce::String id = button->getComponentID ().toStdString ();

			// If the button clicked is a left queue button
			if (id.getLastCharacter () == 'L')
				{
					// Remove identifier from the id string and use id as index
					int fileIndex = id.dropLastCharacters (1).getIntValue ();
					leftFiles.push_back (currentFiles[fileIndex]);

					if (leftDeck
					    != nullptr)  // If left deck is set, update the queue
						{
							leftDeck->queueComponent.updateContent ();
						}
				}

			// Button clicked is a right queue button
			else
				{
					// Similar logic as left button event handler above
					int fileIndex = id.dropLastCharacters (1).getIntValue ();
					rightFiles.push_back (currentFiles[fileIndex]);
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
	return true;
}

void
PlaylistComponent::filesDropped (const juce::StringArray &files, int x, int y)
{
	// Add dropped file to the respective vectors
	if (files.size () == 1)
		{
			juce::File loadedFile{ files[0] };
			loadedFiles.push_back (loadedFile);
			currentTitles.push_back (loadedFile.getFileName ());
			currentFiles.push_back (loadedFile);
		}
	tableComponent.updateContent ();
}

// Search bar event handler
// This function is called every time the text in the search bar changes
void
PlaylistComponent::textEditorTextChanged (juce::TextEditor &)
{
	// Reset the display vectors
	currentTitles.clear ();
	currentFiles.clear ();

	std::vector<juce::String> searchedTitles;
	std::vector<juce::File> searchedFiles;

	// Iterate over all file loaded
	for (juce::File file : loadedFiles)
		{
			juce::String title = file.getFileName ();
			// If the title contains the search string, add it to the display
			// vectors
			if (title.containsIgnoreCase (searchBar.getText ()))
				{
					searchedTitles.push_back (title);
					searchedFiles.push_back (file);
				}
		}

	// Update the display vectors
	currentTitles = searchedTitles;
	currentFiles = searchedFiles;

	// Refresh the table
	tableComponent.updateContent ();
}

/*
 * Set the references to the decks
 * @param deck: Reference to the deck to be set
 * @param id: The id of the deck, either "Left" or "Right"
 */
void
PlaylistComponent::setDeck (DeckGUI *deck, juce::String id)
{
	if (id == "Left") { leftDeck = deck; }
	else { rightDeck = deck; }
}
