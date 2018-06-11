import math


class GazeBehaviour:

    def __init__(self):
        pass

    def open(self, filename):
        f = open(filename + '.txt', 'w')
        return f

    def record(self, timestamp, markers, allBalls, faces, fixation, labels, file):
        epsilon = 80  # the threshold in pixels allowed

        for ball in allBalls:
            print('ball')
            print(ball)

            if len(ball[0]) == 1:
                distX = fixation[0] - ball[0][0][0]
                distY = fixation[1] - ball[0][0][1]

                if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
                    file.write('time: ' + str(timestamp) + ' Fixation intercepts Ball: ' + str(ball[1]) + '\n')
                    self.dictionary(ball[1])

                for face in faces:

                    cX = face[0]
                    cY = face[1]
                    cW = face[0] + face[2]
                    cH = face[1] + face[3]
                    threshold = 50

                    if cX - 30 < fixation[0] < cW + 30 and cY - 30 - threshold < fixation[1] < cH + 30:
                        file.write('time: ' + str(timestamp) + ' Fixation intercepts iCub´s_Face: ' + str(
                            face) + '\n')
                        print("iCub")

    def dictionary(self, index):
        return {
            1: print("Human Tower"),
            2: print("iCub Tower"),
            3: print("Brick"),
            4: print("Human Hand"),
            5: print("iCub Hand")
        }[index]

    def close(self, file):
        file.close()
