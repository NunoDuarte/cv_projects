# import the necessary packages
import argparse
import imutils
import cv2


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True,
        help="path to the input image")
args = vars(ap.parse_args())
print(args["image"])

# load the image, convert it to grayscale, blur it slightly,
# and threshold it
image = cv2.imread(args["image"])
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
blurred = cv2.GaussianBlur(gray, (5, 5), 0)
thresh = cv2.adaptiveThreshold(blurred,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv2.THRESH_BINARY,11,2)

# cv2.imshow('image1', image)
# cv2.imshow('image2', gray)
# cv2.imshow('image3', blurred)
cv2.imshow('image4', thresh)
cv2.waitKey(0)
cv2.destroyAllWindows()

