#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent ()
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize (800, 600);

	// Some platforms require permissions to open input channels so request
	// that here
	if (juce::RuntimePermissions::isRequired (
	        juce::RuntimePermissions::recordAudio)
	    && !juce::RuntimePermissions::isGranted (
	        juce::RuntimePermissions::recordAudio))
		{
			juce::RuntimePermissions::request (
			    juce::RuntimePermissions::recordAudio,
			    [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
		}
	else
		{
			// Specify the number of input and output channels that we want to
			// open
			setAudioChannels (2, 2);
		}

    playlistComponent.setDeck(&deck1, "Left");
    playlistComponent.setDeck(&deck2, "Right");

    addAndMakeVisible (deck1);
    addAndMakeVisible (deck2);
    addAndMakeVisible (playlistComponent);

    formatManager.registerBasicFormats();

}

MainComponent::~MainComponent ()
{
	// This shuts down the audio device and clears the audio source.
	shutdownAudio ();
}

//==============================================================================
void
MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

	mixerSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void
MainComponent::getNextAudioBlock (
    const juce::AudioSourceChannelInfo &bufferToFill)
{
	mixerSource.getNextAudioBlock (bufferToFill);
}

void
MainComponent::releaseResources ()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
	player1.releaseResources ();
	player2.releaseResources ();
}

//==============================================================================
void
MainComponent::paint (juce::Graphics &g)
{
	// (Our component is opaque, so we must completely fill the background with
	// a solid colour)
	g.fillAll (getLookAndFeel ().findColour (
	    juce::ResizableWindow::backgroundColourId));
	// You can add your drawing code here!
}

void
MainComponent::resized ()
{
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
    double sectionHeight = getHeight() / 3;
	deck1.setBounds(0, 0, getWidth ()/2, 2*sectionHeight);
	deck2.setBounds(getWidth()/2, 0, getWidth ()/2, 2*sectionHeight);
	playlistComponent.setBounds(0, 2*sectionHeight, getWidth (), sectionHeight);
}

void
MainComponent::buttonClicked (juce::Button *button)
{
}

void
MainComponent::sliderValueChanged (juce::Slider *slider)
{
}
