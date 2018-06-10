# import files
from mesh import MeshingAlg

# import necessary libraries
from collections import deque
import numpy as np
import cv2
import csv
import argparse
import imutils
import logging as log

cap = cv2.VideoCapture(0)

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())
pts = deque(maxlen=args["buffer"])

mesh = MeshingAlg()

timestamps = np.load('world_viz_timestamps.npy')

i = 0
while True:
    topic, frame = cap.read()

    if frame is not None:
        frame = imutils.resize(frame, width=750)
        height, width, channels = frame.shape

        frame, markers = mesh.mesh(frame)
        #
        # anterior, faces, facesTrained = face.detecting(frame, anterior, faceCascade)
        # labels = face.predict(frame, face_recognizer, faces, facesTrained)
        #
        # # calculate the nearest timestamp for the current frame
        # time = timestamps[i]
        # time_close, ind = find_nearest(timestamps_gaze, float(time))
        # #
        # # use the x, y position of the closest timestamp norm_pos_*
        # pos_x = norm_pos_x[ind]
        # pos_y = norm_pos_y[ind]
        #
        # # print(int(float(pos_x)*width))
        # # print(int(height - int(float(pos_y)*height)))
        # cv2.circle(frame, (int(float(pos_x) * width), int(height - int(float(pos_y) * height))), 10, (0, 255, 1),
        #            thickness=5, lineType=8, shift=0)  # draw circle
        # fixation = [(int(float(pos_x) * width)), int(height - int(float(pos_y) * height))]
        #
        # # check the gaze behaviour
        # if len(ball) is not 0:
        #     gaze.record(time_close, markers, ball, faces, fixation, labels, f)

    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

    cv2.imshow('frame', frame)

    i = i + 1
    # cv2.waitKey(0)

cap.release()
cv2.destroyAllWindows()