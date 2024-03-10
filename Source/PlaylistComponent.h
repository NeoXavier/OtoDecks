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

	int getNumRows () override;

	void paintRowBackground (juce::Graphics &g, int rowNumber, int width,
	                         int height, bool rowIsSelected) override;

	void paintCell (juce::Graphics &g, int rowNumber, int columnId, int width,
	                int height, bool rowIsSelected) override;

	juce::Component *
	refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected,
	                         Component *existingComponentToUpdate) override;

	void buttonClicked (juce::Button *button) override;

	// FileDragAndDropTarget
	bool isInterestedInFileDrag (const juce::StringArray &files) override;
	void filesDropped (const juce::StringArray &files, int x, int y) override;

	//==============================================================================
	/**Override of TextEditor::Listener function to be called whenever the user
	changes the text in the object in some way*/
	void textEditorTextChanged (juce::TextEditor &) override;

	// Files to be used by DeckGUI
	std::vector<juce::File> leftFiles;
	std::vector<juce::File> rightFiles;

	void setDeck (DeckGUI *deck, juce::String id = "");

  private:
	juce::TableListBox tableComponent;

	juce::TextButton loadButton{ "LOAD" };

	// File Chooser
	// https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
	juce::FileChooser fChooser{ "Select a file..." };

	juce::TextEditor searchBar;
    juce::Label searchLabel{"Search", "Search:"};

	std::vector<juce::String> currentTitles;
	std::vector<juce::File> loadedFiles;
	std::vector<juce::File> currentFiles;

	DeckGUI *leftDeck = nullptr;
	DeckGUI *rightDeck = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
