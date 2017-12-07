import cv2
import imutils
import numpy as np

img = cv2.imread('table_wMarkers.jpg', 0)
a = 1
print(a)
img = cv2.medianBlur(img, 5)
cimg = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
print(cimg)

cimg = imutils.resize(cimg, width=750)

circles = cv2.HoughCircles(img, cv2.HOUGH_GRADIENT, 1, 100,
                            param1=5, param2=3, minRadius=10, maxRadius=20)

circles = np.uint16(np.around(circles))
print(circles)
for i in circles[0, :]:
    # draw the outer circle
    cv2.circle(cimg, (i[0], i[1]), i[2], (0, 255, 0), 2)
    # draw the center of the circle
    cv2.circle(cimg, (i[0], i[1]), 2, (0, 0, 255), 3)

cv2.imshow('detected circles', cimg)
cv2.waitKey(0)
cv2.destroyAllWindows()