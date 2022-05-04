# C++ modules using openCV

This folder is where all the modules developed in c++ are located when using openCV

This folder for the c++ modules are separated into several subfolders:
1. [detectionAlgorithm](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/detectionAlgorithm) - Small solutions to several specific problems (more description below)
2. [haarClassifier](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/haarClassifier) - Face classifier using Haar Cascade algorithm
3. [videoCamera](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/videoCamera) - Opening and viewing the video output of a video camera
4. [zmq](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/zmq) - Asynchronous messaging library to connect with Pupil-Labs video output

### inside detection Algorithm
1. [colorDetection](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/detectionAlgorithm/colorDetection) - CV project for color segmentation
2. [gazeDetection](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/detectionAlgorithm/gazeDetection) - Receive Pupil-Labs gaze fixation and publish to CV video frame
3. [objectDetection](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/detectionAlgorithm/objectDetection) - CV project for object segmentation from color segmentation
4. [scenarioDetection](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/detectionAlgorithm/scenarioDetection) - Face (haar) + Object (color) segmentation approach

### inside haar Classifier
1. [cpu](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/haarClassifier/cpu) - CPU version
2. [gpu](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/haarClassifier/gpu) - GPU CUDA version

#### inside cpu 
1. [image](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/haarClassifier/cpu/image) - For single image
2. [video](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/haarClassifier/cpu/video) - For video frame

#### inside gpu 
1. [haarCascades](https://github.com/NunoDuarte/openCVdevelop/tree/master/cpp/haarClassifier/gpu/haarCascades) -  GPU CUDA version of Haar cascade algorithm
