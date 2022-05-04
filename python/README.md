# Python modules using openCV

This folder is where the modules developed in python for Pupil-Labs Image Processing are located when using openCV

## Getting Started

This folder is the main repository for the Python modules that are separated into several subfolders:
1. [detectionAlgorithm](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/detectionAlgorithm) - Object (color segmentation) + Face (haar cascade) Detection Algorithm
2. [haarClassifiers](https://github.com/NunoDuarte/openCVdevelop/tree/master/python/haarClassifiers) - Face detection using Haar Cascades classifier 

## Setup
add anaconda and activate virtual environment
```
export PATH=/home/nuno/anaconda3/bin:$PATH
source activate cpy3.5_cv
```
open onlineStream
```
python3 onlineDetection --buffer 68
```
or offlineVideo
```
python3 offlineDetection --buffer 68
```
