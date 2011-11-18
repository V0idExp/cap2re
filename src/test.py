#!/usr/bin/python2

import cap2re

images = []

camMgr = cap2re.Gphoto2CameraManager()
cameras = camMgr.detectCameras()

for c in range(len(cameras)):
    print "-- Camera n." + str(c+1) + " --"

    print "Serial number:\t", cameras[c].getSerialNo()

    print "Aperture size:\t", cameras[c].getApertureSize()
    #for size in cameras[c].getApertureSizeValues():
    #    print '\t',size

    print "Exposure time:\t", cameras[c].getExposureTime()
    #for time in cameras[c].getExposureTimeValues():
    #    print '\t',time

    print    

for c in range(len(cameras)):
    print "Shooting a pic with Cam"+str(c+1)+"..."
    images.append(cameras[c].capture('/tmp'))


if images:
    print
    print "Picture files:"
    
    for img in images:
        print '  ',img
    
