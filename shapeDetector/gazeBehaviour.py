import logging as log
import math


class gazeBehaviour:

    def __init__(self):
        pass

    def open(self):
        f = open('gaze.txt', 'w')
        return f

    def record(self, timestamp, markers, ball, faces, fixation, file):
        epsilon = 30  # the threshold in pixels allowed

        distX = fixation[0] - ball[0][0]
        distY = fixation[1] - ball[0][1]

        if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
            file.write('time: '+str(timestamp)+' Fixation intercepts Ball: '+str(ball)+'\n')

        for marker in markers:

            cX = marker[0]
            cY = marker[1]

            distX = fixation[0] - cX
            distY = fixation[1] - cY

            if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
                file.write('time: '+str(timestamp)+' Fixation intercepts marker: '+str(marker)+'\n')

        for face in faces:

            cX = face[0]
            cY = face[1]
            cW = face[2]
            cH = face[3]

            if cX < fixation[0] < cW and cH < fixation[0] < cY:
                file.write('time: ' + str(timestamp) + ' Fixation intercepts Face: ' + str(face)+'\n')

    def close(self, file):
        file.close()
