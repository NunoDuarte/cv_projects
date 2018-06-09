# import files
import cv2

# add path to directories
cascadeDir = 'haarCascades'
icubDir = cascadeDir + '/icub'
# originalsDir = cascadeDir + '/original'

# face_cascade = cv2.CascadeClassifier(originalsDir + '/haarcascade_frontalface_default.xml')
# eye_cascade = cv2.CascadeClassifier(originalsDir + '/haarcascade_eye.xml')
icub_cascade = cv2.CascadeClassifier(icubDir + '/cascade-icub-30v30.xml')


if cv2.waitKey() == ord('q'):
    cv2.destroyAllWindows()

img = cv2.imread('icub_face')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
blurred = cv2.GaussianBlur(gray, (31, 31), 0)
thresh = cv2.threshold(img,127,255,cv.THRESH_TOZERO)[1]
faces = icub_cascade.detectMultiScale(
    thresh,
    scaleFactor=1.2,
    minNeighbors=3,
    minSize=(100, 100)
)

for (x,y,w,h) in faces:
    cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
    roi_gray = gray[y:y+h, x:x+w]
    roi_color = img[y:y+h, x:x+w]
    # eyes = eye_cascade.detectMultiScale(roi_gray)
    # for (ex,ey,ew,eh) in eyes:
    #     cv.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
cv2.imshow('img',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
