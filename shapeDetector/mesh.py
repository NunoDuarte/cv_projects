from shapeDetector import shapeDetector


# initialize Algorithm to detect Shapes
main = shapeDetector()
contours, shapes, ratio = main.construct()

print(shapes)
print(len(contours))