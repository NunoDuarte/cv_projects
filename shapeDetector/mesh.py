from shapeDetector import shapeDetector
from hough_circle import houghCircle


# initialize Algorithm to detect Shapes
main = shapeDetector()
contours, shapes, ratio = main.construct()

# initialize Algorithm to detect Circles
hough = houghCircle()
circles = hough.construct()


print(shapes)
print(len(contours))
print(circles)