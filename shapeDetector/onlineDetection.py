# import files
from mesh import meshingAlg
from find_nearest import find_nearest
from redBalltracking import redBall
from faceDetector import faceDetector
from gazeBehaviour import gazeBehaviour
# import necessary libraries
from collections import deque
import numpy as np
import cv2
import csv
import argparse
import imutils
import logging as log

cap = cv2.VideoCapture('world_viz.mp4')

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video", help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
args = vars(ap.parse_args())
pts = deque(maxlen=args["buffer"])

mesh = meshingAlg()

ballTracking = redBall()

cascPath = "haarcascade_frontalface_default.xml"
faceCascade = cv2.CascadeClassifier(cascPath)
log.basicConfig(filename='faceDetected.log', level=log.INFO)
anterior = 0
face = faceDetector()

print("Preparing Data...")
knownFaces, knownLabels = face.prepare_training_data("training-data", faceCascade)
print("Data prepared")

print(np.asarray(knownLabels))
print(knownFaces)

# create our LBPH face recognizer
face_recognizer = cv2.face.LBPHFaceRecognizer_create()
face_recognizer.train(knownFaces, np.array(knownLabels))

timestamps_gaze = list()
norm_pos_x = list()
norm_pos_y = list()

gaze = gazeBehaviour()
f = gaze.open()

with open('gaze_positions_18-12-2017.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        timestamps_gaze.append(float(row['timestamp']))
        norm_pos_x.append(row['norm_pos_x'])
        norm_pos_y.append(row['norm_pos_y'])
        # print(row['timestamp'], row['norm_pos_x'], row['norm_pos_y'])

# print(timestamps_gaze[2])
# print(norm_pos_y[2])      # dont forget it starts with 0
# print(norm_pos_x[2])

timestamps = np.load('world_viz_timestamps.npy')
cv2.waitKey(0)

i = 0

while (True):
    ret, frame = cap.read()

    # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    if frame is not None:
        frame = imutils.resize(frame, width=750)
        height, width, channels = frame.shape

        frame, markers = mesh.mesh(frame)

        frame, pts, ball = ballTracking.tracking(frame, pts, args)

        anterior, faces, non = face.detecting(frame, anterior, faceCascade)

        # calculate the nearest timestamp for the current frame
        time = timestamps[i]
        time_close, ind = find_nearest(timestamps_gaze, float(time))

        # use the x, y position of the closest timestamp norm_pos_*
        pos_x = norm_pos_x[ind]
        pos_y = norm_pos_y[ind]

        print(int(float(pos_x)*width))
        print(int(height - int(float(pos_y)*height)))
        cv2.circle(frame, (int(float(pos_x)*width), int(height - int(float(pos_y)*height))), 10, (0, 255, 1), thickness=5, lineType=8, shift=0)  # draw circle
        fixation = [(int(float(pos_x)*width)), int(height - int(float(pos_y)*height))]

        # check the gaze behaviour
        if len(ball) is not 0:
            gaze.record(time_close, markers, ball, faces, fixation, f)

        cv2.imshow('frame', frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

    i = i + 1
    #cv2.waitKey(0)

gaze.close(f)
cap.release()
cv2.destroyAllWindows()