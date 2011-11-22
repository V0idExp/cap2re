import cap2re
from time import time

mgr = cap2re.Gphoto2CameraManager.instance()


mgr.detectCameras()

print "Testing shoot"
t1 = time()
mgr.captureFromAll("/tmp")
t2 = time() - t1
print "Time: ",t2
print

print "Single-thread shooting"
t1 = time()
mgr.captureFromAll("/tmp")
t2 = time() - t1
print "Time: ",t2
print

print "Multi-threaded shooting"
t1 = time()
mgr.captureFromAll("/tmp", True)
t2 = time() - t1
print "Time: ",t2
