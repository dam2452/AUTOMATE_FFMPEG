# AUTOMATE_FFMPEG

## Overview

AUTOMATE_FFMPEG is a comprehensive command-line application designed for efficient video encoding and manipulation. It not only allows users to choose specific video, audio, and subtitle streams and set various encoding parameters but also enables processing of entire hierarchies of folders and hundreds of videos in a streamlined manner. This makes it ideal for bulk video processing tasks.

## Features

- **Start Processing**: Initiate the video processing workflow.
- **Choose Paths**: Set the source and destination paths for video processing.
- **Select Audio/Video/Subtitle Streams**: Choose specific streams from the media file for processing.
- **Select CQ (Constant Quality)**: Adjust the constant quality setting for video encoding.
- **Select Resolution Limit**: Set a maximum resolution limit for video output.
- **Choose Encoder Type**: Select between different encoder types (CPU/GPU-based).
- **Check Settings**: Review the current configuration and settings.
- **Progress Bar**: Visual progress indication for tracking the encoding process of each video, enhancing the user experience and providing clear feedback on the processing status.

## Screenshots

### Main Menu

![image](https://github.com/dam2452/AUTOMATE_FFMPEG/assets/81230036/ecab20ef-4969-40dc-b26d-da6303e852e5)

This screenshot shows the main menu of the AUTOMATE_FFMPEG, where users can select various options.

### Processing Window

![image](https://github.com/dam2452/AUTOMATE_FFMPEG/assets/81230036/b9391ae4-da72-46d8-ae9b-1dd0f5c6bdce)

Here you can see the tool in action, processing a video file with the selected settings.

### Settings Overview

![image](https://github.com/dam2452/AUTOMATE_FFMPEG/assets/81230036/cb92d9f8-9d6e-4da3-b7a0-93282c87b750)

This screenshot displays the settings overview section, where users can review their current configuration.



## Installation

Installing the Video Processing Tool is straightforward:

1. **Executable File**: Simply download the `.exe` file of the Video Processing Tool. There is no need for a complex installation process.

2. **FFmpeg Dependency**:
   - On **Linux**, you can install FFmpeg using the package manager. Open a terminal and run:
     ```
     sudo apt-get update && sudo apt-get install ffmpeg
     ```
   - On **Windows**, use the Windows Package Manager (winget). Open a PowerShell or Command Prompt and run:
     ```
     winget install -e --id FFmpeg.FFmpeg
     ```

3. **Running the Program**: To use the tool, run the downloaded executable. It can be launched directly from the command line or by double-clicking the file in a file explorer.

With FFmpeg installed and the executable ready, you can start using the Video Processing Tool right away.

## Usage

1. **Start the Application**: Run `AUTOMATE_FFMPEG` from the command line.
2. **Choose an Option**: Enter the number corresponding to the desired action (e.g., `1` to start processing).
3. **Follow Prompts**: Input required information as prompted (e.g., file paths, stream choices).

## Dependencies

- FFmpeg: Required for handling video processing.

## Contributing

Contributions to the AUTOMATE_FFMPEG are welcome. Please ensure you follow our contribution guidelines. For major changes, please open an issue first to discuss what you would like to change.

## License

The AUTOMATE_FFMPEG is open-source software licensed under the MIT License.
