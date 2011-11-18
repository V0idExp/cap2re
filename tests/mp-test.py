from multiprocessing import Pipe, Process, Manager
from cap2re import Gphoto2CameraManager

mgr = Gphoto2CameraManager()

cams = mgr.detectCameras()
procs = []

print "Found " + str(len(cams)) + " cameras"

def capture(camera, outdir, shoots = 10):
    for s in range(shoots):
        camera.capture(outdir)

def parallelShooting():
    outdir = "/tmp/cam"

    for c in range(len(cams)):
        print "Initiating p" + str(c) + "..."
        print "Outdir: " + outdir+str(c+1)
        procs.append( Process(None, capture, None, (cams[c], outdir + str(c+1)) ) )

    for p in range(len(procs)):
        print "Starting p" + str(p) + "..."
        procs[p].start()

    for p in procs:
        p.join()

parallelShooting()
