# import cv2
# import imutils
# import numpy as np
#
# img = cv2.imread('detect_circles_soda.jpg', 0)
# a = 1
# print(a)
# # img = cv2.medianBlur(img, 5)
# cimg = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
# print(cimg)
#
# cimg = imutils.resize(cimg, width=750)
#
# circles = cv2.HoughCircles(img, cv2.HOUGH_GRADIENT, 1, 100,
#                             param1=1, param2=1, minRadius=10, maxRadius=20)
#
# circles = np.uint16(np.around(circles))
# print(circles)
# for i in circles[0, :]:
#     # draw the outer circle
#     cv2.circle(cimg, (i[0], i[1]), i[2], (0, 255, 0), 2)
#     # draw the center of the circle
#     cv2.circle(cimg, (i[0], i[1]), 2, (0, 0, 255), 3)
#
# cv2.imshow('detected circles', cimg)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

# import the necessary packages
import numpy as np
import argparse
import cv2
import imutils

# construct the argument parser and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, help="Path to the image")
args = vars(ap.parse_args())

# load the image, clone it for output, and then convert it to grayscale
image = cv2.imread(args["image"])
image = imutils.resize(image, width=200)
cv2.imshow("image", image)
cv2.waitKey(0)
output = image.copy()
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# detect circles in the image
# circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1.2, 20)
circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1, 100,
                           param1=1, param2=50, minRadius=0, maxRadius=100)

# ensure at least some circles were found
if circles is not None:
    # convert the (x, y) coordinates and radius of the circles to integers
    circles = np.round(circles[0, :]).astype("int")

    # loop over the (x, y) coordinates and radius of the circles
    for (x, y, r) in circles:
        # draw the circle in the output image, then draw a rectangle
        # corresponding to the center of the circle
        cv2.circle(output, (x, y), r, (0, 255, 0), 4)
        cv2.rectangle(output, (x - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)

    # show the output image
    cv2.imshow("output", np.hstack([image, output]))
    cv2.waitKey(0)



