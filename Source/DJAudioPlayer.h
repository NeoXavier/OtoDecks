/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 29 Jan 2024 4:18:34pm
    Author:  Xavier Neo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
public:
    // Constructor and destructor
    DJAudioPlayer(juce::AudioFormatManager& formatManager);
    ~DJAudioPlayer();

    // Load a URL into the audio player
    void loadURL(juce::URL audioURL);

    // Play and stop the audio
    void play();
    void pause();
    void reset();
    
    // Set the gain and position of the audio
    void setGain(double gain);
    void setSpeed(double speed);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    

    // AudioSource methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    /** get the relative position of the playhead */
    double getPositionRelative();

    /** get the length of the audio file */
    double getMaxLength();

    // If file is loaded into the source
    bool fileLoaded = false;

private:
	juce::AudioFormatManager& formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    
};

