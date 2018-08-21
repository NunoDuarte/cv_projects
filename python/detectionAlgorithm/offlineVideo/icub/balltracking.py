import numpy as np
import cv2

class Ball:

    def __init__(self):
        pass

    def trackingCyan(self, frame, pts):
        ball = []

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # because hue wraps up and to extract as many "red objects" as possible,
        # I define lower and upper boundaries for brighter and for darker red shades
        bright_cyan_lower_bounds = (75, 100, 100)
        bright_cyan_upper_bounds = (80, 255, 255)
        bright_cyan_mask = cv2.inRange(hsv, bright_cyan_lower_bounds, bright_cyan_upper_bounds)

        dark_cyan_lower_bounds = (90, 100, 100)
        dark_cyan_upper_bounds = (105, 255, 255)
        dark_cyan_mask = cv2.inRange(hsv, dark_cyan_lower_bounds, dark_cyan_upper_bounds)

        # after masking the red shades out, I add the two images
        weighted_mask = cv2.addWeighted(bright_cyan_mask, 1.0, dark_cyan_mask, 1.0, 0.0)

        # then the result is blurred
        blurred = cv2.GaussianBlur(weighted_mask, (9, 9), 3, 3)

        # construct a mask for the color "red", then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        # mask = cv2.inRange(blurred, greenLower, greenUpper)
        mask = cv2.erode(blurred, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask and initialize the current
        # (x, y) center of the ball
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                                cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        # only proceed if at least one contour was found
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
            if radius > 1:
                # draw the circle and centroid on the frame,
                # then update the list of tracked points
                cv2.putText(frame, "iCub's hand", (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 255, 0), 2)
                cv2.circle(frame, (int(x), int(y)), int(radius),
                           (0, 255, 255), 2)
                cv2.circle(frame, center, 5, (0, 0, 255), -1)
                ball.append([int(x), int(y)])

        # update the points queue
        pts.appendleft(center)

        # show the frame to our screen
        # cv2.imshow("output1", np.hstack([frame, output1]))
        return frame, pts, ball

    def trackingYellow(self, frame, pts):
        ball = []

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # because hue wraps up and to extract as many "red objects" as possible,
        # I define lower and upper boundaries for brighter and for darker red shades
        bright_yellow_lower_bounds = (20, 100, 100)
        bright_yellow_upper_bounds = (30, 255, 255)
        bright_yellow_mask = cv2.inRange(hsv, bright_yellow_lower_bounds, bright_yellow_upper_bounds)

        dark_yellow_lower_bounds = (20, 100, 100)
        dark_yellow_upper_bounds = (30, 255, 255)
        dark_yellow_mask = cv2.inRange(hsv, dark_yellow_lower_bounds, dark_yellow_upper_bounds)

        # after masking the red shades out, I add the two images
        weighted_mask = cv2.addWeighted(bright_yellow_mask, 1.0, dark_yellow_mask, 1.0, 0.0)

        # then the result is blurred
        blurred = cv2.GaussianBlur(weighted_mask, (9, 9), 3, 3)

        # construct a mask for the color "red", then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        # mask = cv2.inRange(blurred, greenLower, greenUpper)
        mask = cv2.erode(blurred, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask and initialize the current
        # (x, y) center of the ball
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                                cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        # only proceed if at least one contour was found
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
            if radius > 1:
                # draw the circle and centroid on the frame,
                # then update the list of tracked points
                cv2.putText(frame, "Human's hand", (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 255, 0), 2)
                cv2.circle(frame, (int(x), int(y)), int(radius),
                           (0, 255, 255), 2)
                cv2.circle(frame, center, 5, (0, 0, 255), -1)
                ball.append([int(x), int(y)])

        # update the points queue
        pts.appendleft(center)

        # show the frame to our screen
        # cv2.imshow("output1", np.hstack([frame, output1]))
        return frame, pts, ball

    def trackingRed(self, frame, pts):
        ball = []

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # because hue wraps up and to extract as many "red objects" as possible,
        # I define lower and upper boundaries for brighter and for darker red shades
        bright_red_lower_bounds = (0, 100, 100)
        bright_red_upper_bounds = (10, 255, 255)
        bright_red_mask = cv2.inRange(hsv, bright_red_lower_bounds, bright_red_upper_bounds)

        dark_red_lower_bounds = (160, 100, 100)
        dark_red_upper_bounds = (179, 255, 255)
        dark_red_mask = cv2.inRange(hsv, dark_red_lower_bounds, dark_red_upper_bounds)

        # after masking the red shades out, I add the two images
        weighted_mask = cv2.addWeighted(bright_red_mask, 1.0, dark_red_mask, 1.0, 0.0)

        # then the result is blurred
        blurred = cv2.GaussianBlur(weighted_mask, (9, 9), 3, 3)

        # construct a mask for the color "red", then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        # mask = cv2.inRange(blurred, greenLower, greenUpper)
        mask = cv2.erode(blurred, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask and initialize the current
        # (x, y) center of the ball
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                                cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        # only proceed if at least one contour was found
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
            if radius > 1:
                # draw the circle and centroid on the frame,
                # then update the list of tracked points
                cv2.putText(frame, "iCub's workspace", (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 255, 0), 2)
                cv2.circle(frame, (int(x), int(y)), int(radius),
                           (0, 255, 255), 2)
                cv2.circle(frame, center, 5, (0, 0, 255), -1)
                ball.append([int(x), int(y)])

        # update the points queue
        pts.appendleft(center)

        # show the frame to our screen
        # cv2.imshow("output1", np.hstack([frame, output1]))
        return frame, pts, ball

    def trackingBlue(self, frame, pts):
        ball = []

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # because hue wraps up and to extract as many "red objects" as possible,
        # I define lower and upper boundaries for brighter and for darker red shades
        bright_blue_lower_bounds = (110, 150, 150) #(65, 60, 60)
        bright_blue_upper_bounds = (140, 255, 255) #(80, 255, 100)
        bright_blue_mask = cv2.inRange(hsv, bright_blue_lower_bounds, bright_blue_upper_bounds)

        dark_blue_lower_bounds = (130, 255, 255) #(65, 60, 160)
        dark_blue_upper_bounds = (140, 255, 255) #(80, 255, 179)
        dark_blue_mask = cv2.inRange(hsv, dark_blue_lower_bounds, dark_blue_upper_bounds)

        # after masking the red shades out, I add the two images
        weighted_mask = cv2.addWeighted(bright_blue_mask, 1.0, dark_blue_mask, 1.0, 0.0)

        # then the result is blurred
        blurred = cv2.GaussianBlur(weighted_mask, (9, 9), 3, 3)

        # construct a mask for the color "red", then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        # mask = cv2.inRange(blurred, greenLower, greenUpper)
        mask = cv2.erode(blurred, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask and initialize the current
        # (x, y) center of the ball
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                                cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        # only proceed if at least one contour was found
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
            if radius > 1:
                # draw the circle and centroid on the frame,
                # then update the list of tracked points
                cv2.putText(frame, "Brick", (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 255, 0), 2)
                cv2.circle(frame, (int(x), int(y)), int(radius),
                           (0, 255, 255), 2)
                cv2.circle(frame, center, 5, (0, 0, 255), -1)
                ball.append([int(x), int(y)])

        # update the points queue
        pts.appendleft(center)

        # show the frame to our screen
        # cv2.imshow("output1", np.hstack([frame, output1]))
        return frame, pts, ball

    def trackingGreen(self, frame, pts):
        ball = []

        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # because hue wraps up and to extract as many "red objects" as possible,
        # I define lower and upper boundaries for brighter and for darker red shades
        bright_green_lower_bounds = (65, 60, 60)
        bright_green_upper_bounds = (80, 255, 100)
        bright_green_mask = cv2.inRange(hsv, bright_green_lower_bounds, bright_green_upper_bounds)

        dark_green_lower_bounds = (65, 60, 160)
        dark_green_upper_bounds = (80, 255, 179)
        dark_green_mask = cv2.inRange(hsv, dark_green_lower_bounds, dark_green_upper_bounds)

        # after masking the red shades out, I add the two images
        weighted_mask = cv2.addWeighted(bright_green_mask, 1.0, dark_green_mask, 1.0, 0.0)

        # then the result is blurred
        blurred = cv2.GaussianBlur(weighted_mask, (9, 9), 3, 3)

        # construct a mask for the color "red", then perform
        # a series of dilations and erosions to remove any small
        # blobs left in the mask
        # mask = cv2.inRange(blurred, greenLower, greenUpper)
        mask = cv2.erode(blurred, None, iterations=2)
        mask = cv2.dilate(mask, None, iterations=2)

        # find contours in the mask and initialize the current
        # (x, y) center of the ball
        cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
                                cv2.CHAIN_APPROX_SIMPLE)[-2]
        center = None

        # only proceed if at least one contour was found
        if len(cnts) > 0:
            # find the largest contour in the mask, then use
            # it to compute the minimum enclosing circle and
            # centroid
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

            # only proceed if the radius meets a minimum size
            if radius > 1:
                # draw the circle and centroid on the frame,
                # then update the list of tracked points
                cv2.putText(frame, "Human's workspace", (int(x), int(y)), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 255, 0), 2)
                cv2.circle(frame, (int(x), int(y)), int(radius),
                          (0, 255, 255), 2)
                cv2.circle(frame, center, 5, (0, 0, 255), -1)
                ball.append([int(x), int(y)])

        # update the points queue
        pts.appendleft(center)

        # show the frame to our screen
        # cv2.imshow("output1", np.hstack([frame, output1]))

        return frame, pts, ball

