# import files
from mesh import MeshingAlg
from findNearest import findNearest
from redBalltracking import RedBall
from faceDetector import faceDetector
from gazeBehaviour import GazeBehaviour
# import necessary libraries
from collections import deque
import numpy as np
import cv2
import csv
import os
import argparse
import imutils
import logging as log

dir = 'PERFECT 18-03 ACTIVE'
directory = os.fsencode(dir)

for file in os.listdir(directory):

    filename = os.fsdecode(file)
    cap = cv2.VideoCapture(dir+'/'+filename+'/world_viz.mp4')

    length = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

    # construct the argument parse and parse the arguments
    ap = argparse.ArgumentParser()
    ap.add_argument("-v", "--video", help="path to the (optional) video file")
    ap.add_argument("-b", "--buffer", type=int, default=64, help="max buffer size")
    args = vars(ap.parse_args())
    pts = deque(maxlen=args["buffer"])

    mesh = MeshingAlg()

    ballTracking = RedBall()

    cascPath = "haarcascade_frontalface_default.xml"
    faceCascade = cv2.CascadeClassifier(cascPath)
    log.basicConfig(filename='faceDetected.log', level=log.INFO)
    anterior = 0
    face = faceDetector()

    print("Preparing Data...")
    knownFaces, knownLabels = face.prepare_training_data("training-data", faceCascade)
    print("Data prepared")

    # create our LBPH face recognizer
    face_recognizer = cv2.face.LBPHFaceRecognizer_create()
    face_recognizer.train(knownFaces, np.array(knownLabels))

    timestamps_gaze = list()
    norm_pos_x = list()
    norm_pos_y = list()

    gaze = GazeBehaviour()
    f = gaze.open(filename)

    with open(dir+'/'+filename+'/gaze_postions.csv', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            timestamps_gaze.append(float(row['timestamp']))
            norm_pos_x.append(row['norm_pos_x'])
            norm_pos_y.append(row['norm_pos_y'])
            # print(row['timestamp'], row['norm_pos_x'], row['norm_pos_y'])

    # print(timestamps_gaze[2])
    # print(norm_pos_y[2])      # dont forget it starts with 0
    # print(norm_pos_x[2])

    timestamps = np.load(dir+'/'+filename+'/world_viz_timestamps.npy')

    i = 0
    while i < length:
        ret, frame = cap.read()

        # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        if frame is not None:
            frame = imutils.resize(frame, width=750)
            height, width, channels = frame.shape

            frame, markers = mesh.mesh(frame)

            frame, pts, ball = ballTracking.tracking(frame, pts, args)

            anterior, faces, facesTrained = face.detecting(frame, anterior, faceCascade)
            labels = face.predict(frame, face_recognizer, faces, facesTrained)

            # calculate the nearest timestamp for the current frame
            time = timestamps[i]
            time_close, ind = findNearest(timestamps_gaze, float(time))

            # use the x, y position of the closest timestamp norm_pos_*
            pos_x = norm_pos_x[ind]
            pos_y = norm_pos_y[ind]

            #print(int(float(pos_x)*width))
            #print(int(height - int(float(pos_y)*height)))
            cv2.circle(frame, (int(float(pos_x)*width), int(height - int(float(pos_y)*height))), 10, (0, 255, 1), thickness=5, lineType=8, shift=0)  # draw circle
            fixation = [(int(float(pos_x)*width)), int(height - int(float(pos_y)*height))]

            # check the gaze behaviour
            if len(ball) is not 0:
                gaze.record(time_close, markers, ball, faces, fixation, labels, f)

            cv2.imshow('frame', frame)
        if cv2.waitKey(25) & 0xFF == ord('q'):
            break

        i = i + 1
        cv2.waitKey(0)

gaze.close(f)
cap.release()
cv2.destroyAllWindows()