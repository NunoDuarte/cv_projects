from shapeDetector import shapeDetector
from hough_circle import houghCircle
import argparse
import cv2
import imutils
import numpy as np
import math

# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, help="Path to the image")
args = vars(ap.parse_args())

# load the image, clone it for output, and then convert it to grayscale
image = cv2.imread(args["image"])
image = imutils.resize(image, width=700)
output = image.copy()
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# initialize Algorithm to detect Shapes
main = shapeDetector()
contours, shapes, ratio = main.construct()

# initialize Algorithm to detect Circles
hough = houghCircle()
circles = hough.construct()

epsilon = 100  # the threshold in pixels allowed

# loop over the (x, y) coordinates and radius of the circles
# for (x, y, r) in circles:
i = 0
print(i)
for c in contours:

    # compute the center of the contour, then detect the name of the
    # shape using only the contour
    M = cv2.moments(c)
    if M["m00"] != 0:
        cX = int((M["m10"] / M["m00"]) * ratio)
        cY = int((M["m01"] / M["m00"]) * ratio)

        # distX = x - cX
        # distY = y - cY

        # if math.sqrt(pow(distX, 2) + pow(distY, 2)) < epsilon:
        shape = shapes[i]

        # multiply the contour (x, y)-coordinates by the resize ratio,
        # then draw the contours and the name of the shape on the image
        c = c.astype("float")
        c *= ratio
        c = c.astype("int")
        cv2.drawContours(image, [c], -1, (0, 255, 0), 2)
        cv2.putText(image, shape, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)

        # show the output image
        resized = imutils.resize(image, width=750)
        cv2.imshow("Image_shapes", resized)
        cv2.waitKey(0)

            # # draw the circle in the output image, then draw a rectangle
            # # corresponding to the center of the circle
            # cv2.circle(output, (x, y), r, (0, 255, 0), 4)
            # cv2.rectangle(output, (x - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)
    i = i + 1
#
#
# # show the output image
# cv2.imshow("output", np.hstack([image, output]))
# cv2.waitKey(0)