# import files
from mesh import MeshingAlg
from findNearest import find_nearest

# import necessary libraries
from collections import deque
import numpy as np
import cv2
import csv
import argparse
import imutils

cap = cv2.VideoCapture('world_viz.mp4')

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())
pts = deque(maxlen=args["buffer"])

mesh = MeshingAlg()

timestamps_gaze = list()
norm_pos_x = list()
norm_pos_y = list()

with open('gaze_positions_18-12-2017.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        timestamps_gaze.append(float(row['timestamp']))
        norm_pos_x.append(row['norm_pos_x'])
        norm_pos_y.append(row['norm_pos_y'])

timestamps = np.load('world_viz_timestamps.npy')

i = 0
while (True):
    ret, frame = cap.read()

    # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    if frame is not None:
        frame = imutils.resize(frame, width=750)
        height, width, channels = frame.shape

        frame, markers = mesh.mesh(frame)

        # calculate the nearest timestamp for the current frame
        time = timestamps[i]
        time_close, ind = find_nearest(timestamps_gaze, float(time))

        # use the x, y position of the closest timestamp norm_pos_*
        pos_x = norm_pos_x[ind]
        pos_y = norm_pos_y[ind]

        cv2.circle(frame, (int(float(pos_x)*width), int(height - int(float(pos_y)*height))), 10, (0, 255, 1), thickness=5, lineType=8, shift=0)  # draw circle
        fixation = [(int(float(pos_x)*width)), int(height - int(float(pos_y)*height))]

        cv2.imshow('frame', frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

    i = i + 1
    #cv2.waitKey(0)


cap.release()
cv2.destroyAllWindows()

