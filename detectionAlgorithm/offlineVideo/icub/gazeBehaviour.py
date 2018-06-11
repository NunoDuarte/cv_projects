import math


class GazeBehaviour:

    def __init__(self):
        pass

    def open(self, filename):
        f = open(filename + '.txt', 'w')
        return f

    def record(self, timestamp, markers, allBalls, faces, fixation, labels, file):
        epsilon = 5  # the threshold in pixels allowed

        for ball in allBalls:

            if len(ball[0]) == 1:
                distX = fixation[0] - ball[0][0][0]
                distY = fixation[1] - ball[0][0][1]

                if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
                    file.write('[' + str(timestamp) + ', ' + str(ball[1]) + ']\n')
                    self.dictionary(ball[1])

                for face in faces:

                    cX = face[0]
                    cY = face[1]
                    cW = face[0] + face[2]
                    cH = face[1] + face[3]
                    threshold = 50

                    if cX - 30 < fixation[0] < cW + 30 and cY - 30 - threshold < fixation[1] < cH + 30:
                        file.write('[' + str(timestamp) + ', 1' + ']\n')
                        print("iCub Face")

    def dictionary(self, index):
        return {
            0: print("Brick"),
            2: print("iCub Hand"),
            3: print("Human Hand"),
            4: print("iCub Tower"),
            5: print("Human Tower")
        }[index]

    def close(self, file):
        file.close()
