# import files
from mesh import meshingAlg
from find_nearest import find_nearest
from redBalltracking import redBall
from faceDetector import faceDetector
from gazeBehaviouroff import gazeBehaviour
# import necessary libraries
from collections import deque
import numpy as np
import cv2
import csv
import os
import argparse
import imutils
import logging as log

import numpy as np
import cv2 as cv
face_cascade = cv.CascadeClassifier('haarcascade_eye.xml')
# eye_cascade = cv.CascadeClassifier('haarcascade_eye.xml')
img = cv.imread('icub_face1')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

faces = face_cascade.detectMultiScale(gray, 1.3, 5)
for (x,y,w,h) in faces:
    cv.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
    roi_gray = gray[y:y+h, x:x+w]
    roi_color = img[y:y+h, x:x+w]
    # # eyes = eye_cascade.detectMultiScale(roi_gray)
    # for (ex,ey,ew,eh) in eyes:
    #     cv.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
cv.imshow('img',img)
cv.waitKey(0)
cv.destroyAllWindows()