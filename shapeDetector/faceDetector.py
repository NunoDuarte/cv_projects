import cv2
import logging as log
import datetime as dt

class faceDetector:

    def __init__(self):
        pass

    def detecting(self, frame, anterior, faceCascade):

        faces = []

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (31, 31), 0)
        thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]

        facesDetect = faceCascade.detectMultiScale(
            thresh,
            scaleFactor=1.2,
            minNeighbors=3,
            minSize=(30, 30)
        )

        # Draw a rectangle around the faces
        for (x, y, w, h) in facesDetect:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            faces.append([x, y, w, h])

        if anterior != len(facesDetect):
            anterior = len(facesDetect)
            log.info("faces: "+str(len(facesDetect))+" at "+str(dt.datetime.now()))

        return anterior, faces

