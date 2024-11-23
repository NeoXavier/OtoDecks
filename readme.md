# OtoDecks

An Audio Mixing/DJ App built with C++ and JUCE

## Description

Key Features:
- Simultanious playback of 2 audio files
- Volume and speed control
- Waveform display
- Scrubbing and seeking of audio files
- Uploading of audio files
- Search engine for audio files

## Getting Started

### Dependencies

* JUCE (v8.0.4)

If you wish to use the run\_proj.sh script, you will need to have xcode and xcode command line tools installed.

### Pre-Compilation Setup
If you wish to have some default audio files included in the app, you can place them in a `tracks` folder in the root directory.
These files will be included in the app without the need to upload them.

### Building and Running

1. Clone the repository using the following command:
```
git clone https://github.com/NeoXavier/OtoDecks.git
```

2. Open the OtoDecks.jucer file in Projucer

3. Export the project to your desired IDE

4. Build the application

5. Run OtoDecks

#### Building and Running with the run project script
Some might appreciate the convenience of building and running the application stright from the terminal.
As such, a script has been provided to do just that. However, it only works for MacOS.

After cloning the repository, you can run the following command to build and run the application:
```
./run_proj.sh
```

## Acknowledgments

* [JUCE Documentation](https://juce.com/learn/documentation/)
