# Python module using openCV for Gaze Fixation detection in Exported Video from PupilLabs

This folder is where the module developed in python for the offline mode of the detection system

## Getting Started
To note that the each folder should contain the following files:
1. world_viz.mp4 video file
2. gaze_position.csv where the gaze fixations data is stored
3. world_viz_timestamps.npy where the timestamps of the world_viz.mp4 is stored

### Requirements
1. openCV version=3.4.0
2. python3 version=3.5
3. create a 'input' folder to include all the PupilLabs Capture exported folders for processing
4. make sure each folder contains the files mentioned above

### Steps
1. python3 main.py
2. press 'q' in order to quit from the python execution

## Authors

* **Nuno Duarte** - *Initial work* - [NunoDuarte](https://github.com/NunoDuarte)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* openCV github PupilLabs
