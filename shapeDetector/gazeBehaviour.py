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
                file.write('time: '+str(timestamp)+' Fixation intercepts Marker: '+str(marker)+'\n')

        for face in faces:

            cX = face[0]
            cY = face[1]
            cW = face[0] + face[2]
            cH = face[1] + face[3]

            # print("face0 {}".format(face[0]))
            # print("face1 {}".format(face[1]))
            # print("face2 {}".format(face[2]))
            # print("face3 {}".format(face[3]))
            # print("fixation0 {}".format(fixation[0]))
            # print("fixation1 {}".format(fixation[1]))

            if cX < fixation[0] < cW and cY < fixation[0] < cH:
                file.write('time: ' + str(timestamp) + ' Fixation intercepts Face: ' + str(face)+'\n')

    def close(self, file):
        file.close()
