from mesh import meshingAlg
import numpy as np
import cv2

cap = cv2.VideoCapture('world_viz.mp4')

mesh = meshingAlg()

while (True):
    ret, frame = cap.read()
    # print(frame)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    mesh.mesh(frame)

    cv2.imshow('frame', frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

    cv2.waitKey(0)

cap.release()
cv2.destroyAllWindows()