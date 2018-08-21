import math


class GazeBehaviour:

    def __init__(self):
        pass

    def open(self, filename):
        f = open(filename + '.txt', 'w')
        return f

    def record(self, timestamp, allBalls, faces, fixation, labels, file):
        epsilon = 30  # the threshold in pixels allowed

        for ball in allBalls:
            if len(ball[0]) == 1:
                distX = fixation[0] - ball[0][0][0]
                distY = fixation[1] - ball[0][0][1]

                if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
                    #                   time               color of ball
                    file.write('[' + str(timestamp) + ',' + str(ball[1]) + ']\n')
                    print("inside")
                    print(ball[1])
                    print(ball)
                    self.dictionary(ball[1])

                for face in faces:

                    cX = face[0]
                    cY = face[1]
                    cW = face[0] + face[2]
                    cH = face[1] + face[3]
                    threshold = 50

                    if cX - 30 < fixation[0] < cW + 30 and cY - 30 - threshold < fixation[1] < cH + 30:
                        file.write('[' + str(timestamp) + ', 1' + ']\n')
                        print("inside")
                        print("iCub Face")
                        return

    def zero(self,):
        return "Brick"

    def two(self,):
        return "iCub Hand"

    def three(self,):
        return "Human Hand"

    def four(self,):
        return "iCub Tower"

    def five(self,):
        return "Human Tower"

    def dictionary(self, index):
        switcher = {
            0: self.zero,
            2: self.two,
            3: self.three,
            4: self.four,
            5: self.five,
        }
        # Get the function from switcher dictionary
        func = switcher.get(index, lambda: "Invalid month")
        # Execute the function
        print(func())

    def close(self, file):
        file.close()
