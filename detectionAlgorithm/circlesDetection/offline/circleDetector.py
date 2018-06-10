import cv2
import numpy as np

original = cv2.imread("table_wMarkers.jpg")

gray = cv2.cvtColor(original, cv2.COLOR_BGR2GRAY)
blurred = cv2.GaussianBlur(gray, (5, 5), 0)
ret3, image = cv2.threshold(blurred, 50, 255, cv2.THRESH_BINARY)

el = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
image = cv2.dilate(image, el, iterations=6)

cv2.imwrite("dilated.png", image)

image, contours, hierarchy = cv2.findContours(
    image.copy(),
    cv2.RETR_LIST,
    cv2.CHAIN_APPROX_SIMPLE)

drawing = cv2.imread("table_wMarkers.jpg")

centers = []
radii = []
for contour in contours:
    area = cv2.contourArea(contour)

    # there is one contour that contains all others, filter it out
    if area > 500:
        continue

    br = cv2.boundingRect(contour)
    radii.append(br[2])

    m = cv2.moments(contour)
    center = (int(m['m10'] / m['m00']), int(m['m01'] / m['m00']))
    centers.append(center)

print("There are {} circles".format(len(centers)))

radius = int(np.average(radii)) + 5

for center in centers:
    cv2.circle(drawing, center, 3, (255, 0, 0), -1)
    cv2.circle(drawing, center, radius, (0, 255, 0), 1)

cv2.imwrite("drawing.png", drawing)