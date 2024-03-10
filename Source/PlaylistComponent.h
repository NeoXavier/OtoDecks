/*
  ==============================================================================

    PlaylistComponent.h
    Created: 31 Jan 2024 1:36:18pm
    Author:  Xavier Neo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener,
                          public juce::FileDragAndDropTarget

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
    
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked (juce::Button* button) override;

    // FileDragAndDropTarget
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Files to be used by DeckGUI
    std::vector<juce::File> leftFiles;
    std::vector<juce::File> rightFiles;


  private:
	juce::TableListBox tableComponent;
    
	juce::TextButton loadButton{ "LOAD" };

    // File Chooser
	// https://docs.juce.com/master/classFileChooser.html#ac888983e4abdd8401ba7d6124ae64ff3
	juce::FileChooser fChooser{ "Select a file..." };

    std::vector<juce::String> loadedTitles;
    std::vector<juce::File> loadedFiles;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
