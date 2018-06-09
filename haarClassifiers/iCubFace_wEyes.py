import numpy as np
import cv2

# watch_cascade = cv2.CascadeClassifier('ipad-cascade-10stagesnew.xml')
icub_cascade = cv2.CascadeClassifier('cascade-icub-30v30.xml')
#icub_cascade = cv2.CascadeClassifier('cascade-icub-60v60.xml')
cascPath = "cascade-icub-60v60.xml"
faceCascade = cv2.CascadeClassifier(cascPath)
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')

faces = []
faceTrain = []

cap = cv2.VideoCapture(0)

while 1:
    ret, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    icub = icub_cascade.detectMultiScale(gray, 10, 10)
#    blurred = cv2.GaussianBlur(gray, (31, 31), 0)
#    thresh = cv2.threshold(blurred, 127, 255,cv2.THRESH_TOZERO)[1]
#    icub = icub_cascade.detectMultiScale(
#    	thresh,
#    	scaleFactor=1.3,
#    	minNeighbors=10,
#    	minSize=(50, 50),
#    	maxSize=(100, 100)
#    )

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (31, 31), 0)
    thresh = cv2.threshold(blurred, 127, 255, cv2.THRESH_TOZERO)[1]
    facesDetect = faceCascade.detectMultiScale(
        thresh,
        scaleFactor=1.3,
        minNeighbors=3,
        minSize=(50, 50),
        maxSize=(100, 100)
    )

    # Draw a rectangle around the faces
    for (x, y, w, h) in facesDetect:
        # print(x,y,w,h)
        num_eyes = 0

        roi_gray = gray[y:y + h, x:x + w]
        faces.append([x, y, w, h])
        faceTrain.append(gray[y:y + w, x:x + h])
        eyes = eye_cascade.detectMultiScale(roi_gray)
        for (ex, ey, ew, eh) in eyes:
            if len(eyes) == 2:
                cv2.rectangle(roi_gray, (ex, ey), (ex + ew, ey + eh), (0, 255, 0), 2)
                num_eyes = num_eyes + 1
                print("hello")
        if num_eyes == 2:
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
            faces = []
            faceTrain = []

    # for (x,y,w,h) in icub:
    #     cv2.rectangle(img, (x,y), (x+w, y+h), (255,255,0), 2)
    #     font = cv2.FONT_HERSHEY_SIMPLEX
    #     cv2.putText(img, 'iCub', (x-w, y-h), font, 0.5, (0,255,255), 2, cv2.LINE_AA)

    cv2.imshow('img', img)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()