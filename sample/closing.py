import cv

ob = cv.LoadImage("../ref/Level1/level1_sample3.png",cv.CV_LOAD_IMAGE_GRAYSCALE)
size = cv.GetSize(ob)
obb = cv.CreateImage(size,8,1)
cv.Threshold(ob,obb,128,255,cv.CV_THRESH_BINARY)

obd = cv.CreateImage(size,8,1)
obe = cv.CreateImage(size,8,1)

cv.Dilate(obb,obd,None,4)
cv.Erode(obd,obe,None,4)

cv.NamedWindow("src")
cv.ShowImage("src",ob)
cv.NamedWindow("Closing")
cv.ShowImage("Closing",obe)

cv.WaitKey (0)
cv.DestroyWindow("Closing")
