#!/bin/bash
# Compile and run JUCE project

PROJECT_NAME="OtoDecks"  
PROJECT_ROOT="$(pwd)"

# Navigate to the directory where the project is located
cd ${PROJECT_ROOT}/Builds/MacOSX

# Execute the xcodebuild command
#xcodebuild -project "${PROJECT_NAME}.xcodeproj"
xcodebuild -scheme "${PROJECT_NAME} - App" -quiet -destination 'platform=macOS,arch=x86_64'

# Navigate to the directory where the compiled app is located
cd build/Debug

# Run the compiled app
./${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}
