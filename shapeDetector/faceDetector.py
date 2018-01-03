import cv2
import logging as log
import datetime as dt

class faceDetector:

    def __init__(self):
        pass

    def detecting(self, frame, anterior, faceCascade):

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (31, 31), 0)
        thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]

        faces = faceCascade.detectMultiScale(
            thresh,
            scaleFactor=1.1,
            minNeighbors=3,
            minSize=(30, 30)
        )

        # Draw a rectangle around the faces
        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

        if anterior != len(faces):
            anterior = len(faces)
            log.info("faces: "+str(len(faces))+" at "+str(dt.datetime.now()))

        return anterior

