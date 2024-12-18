/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 29 Jan 2024 4:18:34pm
    Author:  Xavier Neo

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer (juce::AudioFormatManager &_formatManager)
    : formatManager (_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer () {}

// Loads a file into the audio player
void
DJAudioPlayer::loadURL (juce::URL audioURL)
{
	auto *reader
	    = formatManager.createReaderFor (audioURL.createInputStream (false));
	if (reader != nullptr)  // good file!
		{
			std::unique_ptr<juce::AudioFormatReaderSource> newSource (
			    new juce::AudioFormatReaderSource (reader, true));
			transportSource.setSource (newSource.get (), 0, nullptr,
			                           reader->sampleRate);
			readerSource.reset (newSource.release ());
            fileLoaded = true;
		}
	else { std::cout << "Something went wrong loading the file" << std::endl; }
}

// Play audio player
void
DJAudioPlayer::play ()
{
	transportSource.start ();
}

// Pause audio player
void DJAudioPlayer::pause()
{
    transportSource.stop();
}

// Reset to the start of the track
void
DJAudioPlayer::reset ()
{
    transportSource.setPosition (0);
	transportSource.stop ();
}

// Set the volume of the audio player
void
DJAudioPlayer::setGain (double gain)
{
	transportSource.setGain (gain);
}

// Set the position of the audio player
void
DJAudioPlayer::setPosition (double posInSecs)
{
	if (posInSecs < 0 || posInSecs > transportSource.getLengthInSeconds ())
		{
			std::cout << "DJAudioPlayer::setPosition: warning set position "
			          << posInSecs << " greater than length "
			          << transportSource.getLengthInSeconds () << std::endl;
			return;
		}
	transportSource.setPosition (posInSecs);
}

// Set the position of the audio player
void
DJAudioPlayer::setPositionRelative (double pos)
{
	if (pos < 0 || pos > 1.0)
		{
			std::cout << "DJAudioPlayer::setPositionRelative pos should be "
			             "between 0 and 1"
			          << std::endl;
		}
	else
		{
			double posInSecs = transportSource.getLengthInSeconds () * pos;
			setPosition (posInSecs);
		}
}

// Sets the speed of the audio player
void
DJAudioPlayer::setSpeed (double ratio)
{
	if (ratio < 0 || ratio > 100.0)
		{
			std::cout
			    << "DJAudioPlayer::setSpeed: ratio sould be between 0 and 100 "
			    << std::endl;
		}
	else { resampleSource.setResamplingRatio (ratio); }
}

void
DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
	resampleSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void
DJAudioPlayer::getNextAudioBlock (
    const juce::AudioSourceChannelInfo &bufferToFill)
{
	if (readerSource.get () == nullptr)
		{
			bufferToFill.clearActiveBufferRegion ();
			return;
		}
	resampleSource.getNextAudioBlock (bufferToFill);
}

void
DJAudioPlayer::releaseResources ()
{
	transportSource.releaseResources ();
	resampleSource.releaseResources ();
}

double
DJAudioPlayer::getPositionRelative ()
{
	return transportSource.getCurrentPosition ()
	       / transportSource.getLengthInSeconds ();
}

// Gets the length of the loaded audio file
double DJAudioPlayer::getMaxLength()
{
    return transportSource.getLengthInSeconds();
}
