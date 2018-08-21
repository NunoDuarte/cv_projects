# openCVdevelop

Assignments using OpenCV and/or useful stuff that I learned to use with OpenCV

## Deep Neural Networks (current project) 
1.  python3 dnnImageClassifier.py -i images/eagle.png -p bvlc_googlenet.prototxt -m bvlc_googlenet.caffemodel -l synset_words.txt

2. import caffe
export PYTHONPATH=/home/nduarte/software/caffe/python

# in case you cant find a module
python3 

import $package

help(package)

e.g.
import cv2
help(cv2.face)

# to open onlineStream or offlineVideo project:
export PATH=/home/nuno/anaconda3/bin:$PATH

source activate cpy3.5_cv

python3 onlineDetection --buffer 68

### new goals:
1. run haarclassifier using opencv with gpu enable
2. use a pre-train network to detect and mark objects in a image
3. move the application to video
4. train a new network to the objects of my choosing
5. check the speed in the new machine
