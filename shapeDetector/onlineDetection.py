from mesh import meshingAlg
from find_nearest import find_nearest
import numpy as np
import cv2
import csv

cap = cv2.VideoCapture('world_viz.mp4')
width = cap.get(3)
height = cap.get(4)

mesh = meshingAlg()

timestamps_gaze = list()
norm_pos_x = list()
norm_pos_y = list()

with open('gaze_positions_18-12-2017.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        timestamps_gaze.append(float(row['timestamp']))
        norm_pos_x.append(row['norm_pos_x'])
        norm_pos_y.append(row['norm_pos_y'])
        # print(row['timestamp'], row['norm_pos_x'], row['norm_pos_y'])

# print(timestamps_gaze[2])
# print(norm_pos_y[2])      # dont forget it starts with 0
# print(norm_pos_x[2])

timestamps = np.load('world_viz_timestamps.npy')
cv2.waitKey(0)

i = 0

while (True):
    ret, frame = cap.read()
    # print(frame)
    # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    mesh.mesh(frame)

    # calculate the nearest timestamp for the current frame
    time = timestamps[i]
    time_close, ind = find_nearest(timestamps_gaze, float(time))
    print('time ', time)
    print('ind', ind)
    print('time_close', time_close)

    # use the x, y position of the closest timestamp norm_pos_*
    pos_x = norm_pos_x[ind+20]
    pos_y = norm_pos_y[ind+20]

    print(int(float(pos_x)*width))
    print(int(height - int(float(pos_y)*height)))
    cv2.circle(frame, (int(float(pos_x)*width), int(height - int(float(pos_y)*height))), 10, (0, 255, 1), thickness=1, lineType=8, shift=0)  # draw circle

    cv2.imshow('frame', frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

    i = i + 1
    cv2.waitKey(0)

cap.release()
cv2.destroyAllWindows()