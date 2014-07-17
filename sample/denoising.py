import numpy as np
import cv2
from matplotlib import pyplot as plt

def nothing(*arg):
    pass
img = cv2.imread("level1_sample1.png")


cv2.imshow("srcimg",img)
cv2.namedWindow("denoising")
cv2.createTrackbar('thrs1','denoising',3,100,nothing)
cv2.createTrackbar('thrs2','denoising',7,100,nothing)
cv2.createTrackbar('thrs3','denoising',21,100,nothing)
cv2.imshow("denoising",img)

#plt.subplot(121),plt.imshow(img)
while True:
    print 'ok'
    cv2.waitKey()
    thrs1 = cv2.getTrackbarPos('thrs1','denoising')
    thrs2 = cv2.getTrackbarPos('thrs2','denoising')
    thrs3 = cv2.getTrackbarPos('thrs3','denoising')
    print ' thrs1=' + str(thrs1) + ' thrs2=' + str(thrs2) + ' thrs3=' + str(thrs3)
    dst = cv2.fastNlMeansDenoising(img,None,thrs1,thrs2,thrs3)
    cv2.imshow("denoising",dst)
cv2.destroyAllWindows()

