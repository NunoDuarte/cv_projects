# Useful multi-purpose openCV projects
Code is available in two programming languages:
- [Python](https://github.com/NunoDuarte/openCVdevelop/tree/master/python) modules using openCV
- [C++](https://github.com/NunoDuarte/openCVdevelop/tree/master/c++)  modules using openCV

One folder is reserved for ready to use DeepNN approaches in Python
- [Deep](https://github.com/NunoDuarte/openCVdevelop/tree/master/deep) Learning modules using openCV

### in case you cant find a module
python3 

import $package

help(package)

e.g.
import cv2
help(cv2.face)

### to open onlineStream or offlineVideo project:
export PATH=/home/nuno/anaconda3/bin:$PATH

source activate cpy3.5_cv

python3 onlineDetection --buffer 68

## Goals:
- [ ] run haarclassifier using opencv with gpu enable
- [ ] use a pre-train network to detect and mark objects in a image
- [ ] move the application to video
- [ ] train a new network to the objects of my choosing
- [ ] check the speed in the new machine

## Authors

* **Nuno Duarte** - *Initial work* - [NunoDuarte](https://github.com/NunoDuarte)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* openCV github python3 c++ Tensorflow Caffe

