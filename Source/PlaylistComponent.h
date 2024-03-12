/*
  ==============================================================================

    PlaylistComponent.h
    Created: 31 Jan 2024 1:36:18pm
    Author:  Xavier Neo

  ==============================================================================
*/

#pragma once

#include "DeckGUI.h"
#include <JuceHeader.h>

//==============================================================================
/*
 */

// Forward delcaration of DeckGUI to avoid circular dependency
class DeckGUI;

class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::FileDragAndDropTarget,
                          public juce::TextEditor::Listener

{
  public:
	PlaylistComponent ();
	~PlaylistComponent () override;

	void paint (juce::Graphics &) override;
	void resized () override;

    // TableListBoxModel virtual functions
    //==============================================================================
	int getNumRows () override;

	void paintRowBackground (juce::Graphics &g, int rowNumber, int width,
	                         int height, bool rowIsSelected) override;

	void paintCell (juce::Graphics &g, int rowNumber, int columnId, int width,
	                int height, bool rowIsSelected) override;

	juce::Component *
	refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected,
	                         Component *existingComponentToUpdate) override;

    // Button::Listener virtual functions
    //==============================================================================
	void buttonClicked (juce::Button *button) override;

	// FileDragAndDropTarget virtual functions
    //==============================================================================
	bool isInterestedInFileDrag (const juce::StringArray &files) override;
	void filesDropped (const juce::StringArray &files, int x, int y) override;

    // TextEditor::Listener virtual functions
	//==============================================================================
	void textEditorTextChanged (juce::TextEditor &) override;


	// File lists to be read by their respective decks
	std::vector<juce::File> leftFiles;
	std::vector<juce::File> rightFiles;

    // To set the references to the decks 
    // This cannot be done through the constructor as it will lead to a circular dependency problem
	void setDeck (DeckGUI *deck, juce::String id = "");

  private:
    // Playlist
	juce::TableListBox tableComponent;

    // Load Button
	juce::TextButton loadButton{ "LOAD" };

	// File Chooser
	// Reference: https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
	juce::FileChooser fChooser{ "Select a file..." };

    // Search Bar
	juce::TextEditor searchBar;
    juce::Label searchLabel{"Search", "Search:"};

    // Vectors for the loaded files and their titles
	std::vector<juce::File> loadedFiles; // Files loaded into the program
    std::vector<juce::String> currentTitles; // Song names of files to be displayed in the table (may be affected by search)
	std::vector<juce::File> currentFiles; // Files to be displayed in the table (may be affected by search)

    // References to the decks
	DeckGUI *leftDeck = nullptr;
	DeckGUI *rightDeck = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
