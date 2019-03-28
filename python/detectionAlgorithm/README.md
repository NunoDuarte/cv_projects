## Getting Started

This folder is the main repository for the Detection Algorithm Python modules that are separated into several subfolders:
1. Detection of Circles [circlesDetection](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/circlesDetection) 
<br /> 1.1. Offline Mode [offline](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/circlesDetection/offline)
<br /> 1.2. Online Mode [online](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/circlesDetection/online)

2. Offline Video Processing [offlineVideo](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/offlineVideo)
<br /> 2.1. Human-Human Data [hhi](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/offlineVideo/hhi)
<br /> 2.2. Human-Robot Data [icub](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/offlineVideo/icub)
<br /> 2.3. Simplified Version [simple](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm/offlineVideo/simple)

## openCV project of contextual detection in a HHI environment

# inputs:
	- video .mp4 
	- gaze_positions:	
		- timestamps
		- norm_pos_x
		- norm_pos_y
	- world_viz_timestamps.npy

# output:
	- time
	- Ball location
	- What it is looking: Ball, Marker, Face

# what to do:
1. for onlineStream dont forget to clean the code of the hhi and icub folders

## Authors

* **Nuno Duarte** - *Initial work* - [NunoDuarte](https://github.com/NunoDuarte)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* openCV github

