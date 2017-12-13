from main import Shapes


# initialize Algorithm to detect Shapes
main = Shapes()
contours, shapes, ratio = main.Constructor()

print(shapes)
print(len(contours))