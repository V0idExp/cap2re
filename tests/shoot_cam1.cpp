#include "Camera.h"
#include "Gphoto2CameraManager.h"

#include <iostream>
using namespace std;

int main()
{
    CameraManager *mgr = new Gphoto2CameraManager;
    cout << "CameraManager: " << mgr << endl;

    CameraPtrList cameras = mgr->detectCameras();

    if(cameras.size() >= 1)
        cameras[0]->capture("/tmp/cam1");

    delete mgr;
}
