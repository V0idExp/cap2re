import cap2re
from time import time

mgr = cap2re.Gphoto2CameraManager.instance()

images = []

mgr.detectCameras()

print "Initial shoot"
t1 = time()
mgr.captureFromAll("/tmp")
t2 = time() - t1
print "Time: ",t2
print

print "Single-threaded shooting"
t1 = time()
mgr.captureFromAll("/tmp")
t2 = time() - t1
print "Time: ",t2
print

for i in range(10):
    print "Multi-threaded shooting"
    t1 = time()
    for img in mgr.captureFromAll("/tmp", True):
        images.append(img)
    t2 = time() - t1
    print "Time: ",t2
    print
