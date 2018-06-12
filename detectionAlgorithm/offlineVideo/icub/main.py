# import files
from findNearest import findNearest
from balltracking import Ball
from faceDetector import FaceDetector
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

dir = 'input'
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

    ballTracking = Ball()

    cascPath = "cascade-icub-60v60.xml"
    faceCascade = cv2.CascadeClassifier(cascPath)
    log.basicConfig(filename='faceDetected.log', level=log.INFO)
    anterior = 0
    face = FaceDetector()

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

    with open(dir+'/'+filename+'/gaze_positions.csv', newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            timestamps_gaze.append(float(row['timestamp']))
            norm_pos_x.append(row['norm_pos_x'])
            norm_pos_y.append(row['norm_pos_y'])

    print(len(timestamps_gaze))
    timestamps = np.load(dir+'/'+filename+'/world_viz_timestamps.npy')
    print(len(timestamps))
    cv2.waitKey(0)

    i = 0
    ball = []
    time0 = timestamps[i]
    while i < length:
        ret, frame = cap.read()

        # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        if frame is not None:
            frame = imutils.resize(frame, width=750)
            height, width, channels = frame.shape

            frame, pts, ballG = ballTracking.trackingGreen(frame, pts)
            if ballG is not [] and len(ballG) != 0:
                ball.append([ballG, 5])
            frame, pts, ballR = ballTracking.trackingRed(frame, pts)
            if ballR is not [] and len(ballR) != 0:
                ball.append([ballR, 4])
            frame, pts, ballB = ballTracking.trackingBlue(frame, pts)
            if ballB is not [] and len(ballB) != 0:
                ball.append([ballB, 0])
            # frame, pts, ballY = ballTracking.trackingYellow(frame, pts)
            # if ballY is not [] and len(ballY) != 0:
            #     ball.append([ballY, 3])
            # frame, pts, ballC = ballTracking.trackingCyan(frame, pts)
            # if ballC is not [] and len(ballC) != 0:
            #     ball.append([ballC, 2])

            anterior, faces, facesTrained = face.detecting(frame, anterior, faceCascade)
            labels = face.predict(frame, face_recognizer, faces, facesTrained)

            # calculate the nearest timestamp for the current frame
            time = timestamps[i]
            time_close, ind = findNearest(timestamps_gaze, float(time))

            # use the x, y position of the closest timestamp norm_pos_*
            pos_x = norm_pos_x[ind]
            pos_y = norm_pos_y[ind]

            cv2.circle(frame, (int(float(pos_x)*width), int(height - int(float(pos_y)*height))), 10, (0, 255, 1),
                       thickness=5, lineType=8, shift=0)  # draw circle
            fixation = [(int(float(pos_x)*width)), int(height - int(float(pos_y)*height))]

            # check the gaze behaviour
            if len(ball) is not 0:
                gaze.record(time_close-time0, ball, faces, fixation, labels, f)

            cv2.imshow('frame', frame)
        if cv2.waitKey(25) & 0xFF == ord('q'):
            break

        i = i + 1
        # clear the lists
        ball = []
        faces = []

        # wait for key pressed
        cv2.waitKey(0)

gaze.close(f)
cap.release()
cv2.destroyAllWindows()