import math
from pylsl import StreamInfo, StreamOutlet

class gazeBehaviour:

    def __init__(self):
        pass

    def open(self):
        f = open('gaze-CENTER-MARKER.txt', 'w')
        return f

    def record(self, timestamp, markers, ball, faces, fixation, labels, file):
        epsilon = 80  # the threshold in pixels allowed

        distX = fixation[0] - ball[0][0]
        distY = fixation[1] - ball[0][1]

        if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
            file.write('time: '+str(timestamp)+' Fixation intercepts Ball: '+str(ball[0])+'\n')
            print("Ball")

        for marker in markers:

            cX = marker[0]
            cY = marker[1]

            distX = fixation[0] - cX
            distY = fixation[1] - cY

            if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
                file.write('time: '+str(timestamp)+' Fixation intercepts Marker: '+str(marker)+'\n')
                print("Marker")

        i = 0
        for face in faces:

            cX = face[0]
            cY = face[1]
            cW = face[0] + face[2]
            cH = face[1] + face[3]

            if cX - 30 < fixation[0] < cW + 30 and cY - 30 < fixation[1] < cH + 30:
                file.write('time: ' + str(timestamp) + ' Fixation intercepts ' + labels[i] + '´s_Face: ' + str(face) + '\n')
                print("Face")

            i = i+1

    def close(self, file):
        file.close()
