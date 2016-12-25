import urllib
import cv2
import numpy as np
import os

def store_raw_images():
    neg_images_link = 'http://image-net.org/api/text/imagenet.synset.geturls?wnid=n00523513'
    neg_images_urls = urllib.urlopen(neg_images_link).read().decode()

    if not os.path.exists('neg'):
        os.makedirs('neg')


    pic_num = 1
    for i in neg_images_urls.split('\n'):
        try:
            print(i)
            urllib.urlretrieve(i, "neg/" + str(pic_num)+'.jpg')
            img = cv2.imread("neg/" + str(pic_num)+'.jpg', cv2.IMREAD_GRAYSCALE)
            resized_image = cv2.resize(img, (100,100))
            cv2.imwrite("neg/" + str(pic_num)+'.jpg', resized_image)
            pic_num += 1

        except Exception as e:
            print(str(e))


store_raw_images()