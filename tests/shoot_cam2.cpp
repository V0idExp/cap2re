#include "Camera.h"
#include "Gphoto2CameraManager.h"

#include <iostream>
using namespace std;

int main()
{
    CameraManager *mgr = Gphoto2CameraManager::instance();
    cout << "CameraManager: " << mgr << endl;

    CameraPtrList cameras = mgr->detectCameras();

    if(cameras.size() >= 2)
        cameras[1]->capture("/tmp/cam2");

    delete mgr;
}

