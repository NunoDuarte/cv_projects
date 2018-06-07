# openCVdevelop

Assignments using OpenCV and/or useful stuff that I learned to use with OpenCV

## Deep Neural Networks (current project) 
1.  python3 dnnImageClassifier.py -i images/eagle.png -p bvlc_googlenet.prototxt -m bvlc_googlenet.caffemodel -l synset_words.txt

# in case you cant find a module
python3 

import $package

help(package)

e.g.
import cv2
help(cv2.face)

# to open shapeDetector project:
export PATH=/home/nuno/anaconda3/bin:$PATH

source activate cpy3.5_cv

python3 onlineDetection --buffer 68
