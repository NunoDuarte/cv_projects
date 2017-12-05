# import the necessary packages
from circle_shape import ShapeDetector
from center_of_shape import CenterShape
import argparse
import imutils
import cv2

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True,
                help="path to the input image")
args = vars(ap.parse_args())

image = cv2.imread(args["image"])
image_Shapes = image.copy()

cs = CenterShape()
sd = ShapeDetector()

cnts, ratio = cs.center(image)

# loop over the contours
a = 1

for c in cnts:
    print(a)

    # compute the center of the contour, then detect the name of the
    # shape using only the contour
    M = cv2.moments(c)
    if M["m00"] != 0:
        cX = int((M["m10"] / M["m00"]) * ratio)
        cY = int((M["m01"] / M["m00"]) * ratio)
        shape = sd.detect(c)

        # multiply the contour (x, y)-coordinates by the resize ratio,
        # then draw the contours and the name of the shape on the image
        c = c.astype("float")
        c *= ratio
        c = c.astype("int")
        cv2.drawContours(image_Shapes, [c], -1, (0, 255, 0), 2)
        cv2.putText(image_Shapes, shape, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2)

        # show the output image
        resized = imutils.resize(image_Shapes, width=750)
        cv2.imshow("Image_shapes", resized)
        cv2.waitKey(0)

    a=a+1


