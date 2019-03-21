# import files
from findNearest import findNearest
# import necessary libraries
from collections import deque
import numpy as np
import cv2
import csv
import os
import argparse
import imutils
import logging as log

dir = 'input'
directory = os.fsencode(dir)

for file in os.listdir(directory):

    filename = os.fsdecode(file)
    cap = cv2.VideoCapture(dir+'/'+filename+'/world_viz.mp4')

    length = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    # construct the argument parse and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument("-v", "--video", help="path to the (optional) video file")
    args = vars(ap.parse_args())

    timestamps_gaze = list()
    norm_pos_x = list()
    norm_pos_y = list()


    with open(dir+'/'+filename+'/gaze_postions.csv', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            timestamps_gaze.append(float(row['timestamp']))
            norm_pos_x.append(row['norm_pos_x'])
            norm_pos_y.append(row['norm_pos_y'])

    timestamps = np.load(dir+'/'+filename+'/world_viz_timestamps.npy')

    i = 0
    while i < length:
        ret, frame = cap.read()

        if frame is not None:
            frame = imutils.resize(frame, width=750)
            height, width, channels = frame.shape

            # calculate the nearest timestamp for the current frame
            time = timestamps[i]
            time_close, ind = findNearest(timestamps_gaze, float(time))

            # use the x, y position of the closest timestamp norm_pos_*
            pos_x = norm_pos_x[ind]
            pos_y = norm_pos_y[ind]

            cv2.circle(frame, (int(float(pos_x)*width), int(height - int(float(pos_y)*height))), 10, (0, 255, 1),
                       thickness=5, lineType=8, shift=0)  # draw circle
            fixation = [(int(float(pos_x)*width)), int(height - int(float(pos_y)*height))]

            cv2.imshow('frame', frame)
        if cv2.waitKey(25) & 0xFF == ord('q'):
            break

        i = i + 1


cap.release()
cv2.destroyAllWindows()
