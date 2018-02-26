import cv2
import numpy as np

lim=40
d=cv2.imread("bgbk2.jpg")
f=np.zeros([d.shape[0],d.shape[1],4])
for i in range(d.shape[0]):
    print(d[i,:,0])
    for j in range(d.shape[1]):
        if d[i,j,0]<=lim and d[i,j,1]<=lim and d[i,j,2]<=lim:# and ((i-40)**2+(j-40)**2)**.5>30:
            f[i,j,:]=0
        else:
            f[i,j,:3]=d[i,j,:]
            f[i,j,3]=255
    print()

cv2.imwrite("bgbk.png",f)
