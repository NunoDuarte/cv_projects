from mesh import meshingAlg
import numpy as np
import cv2
import csv

# cap = cv2.VideoCapture('world_viz.mp4')

# mesh = meshingAlg()

timestamps_gaze = list()
norm_pos_x = list()
norm_pos_y = list()

with open('gaze_positions_18-12-2017.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        timestamps_gaze.append(row['timestamp'])
        norm_pos_x.append(row['norm_pos_x'])
        norm_pos_y.append(row['norm_pos_y'])
        # print(row['timestamp'], row['norm_pos_x'], row['norm_pos_y'])

# print(timestamps_gaze[2])
# print(norm_pos_y[2])      # dont forget it starts with 0
# print(norm_pos_x[2])

timestamps = np.load('world_viz_timestamps.npy')
cv2.waitKey(0)

while (True):
    ret, frame = cap.read()
    # print(frame)
    # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    mesh.mesh(frame)

    cv2.imshow('frame', frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

    cv2.waitKey(0)

cap.release()
cv2.destroyAllWindows()