#include "Common.h"
#include "Gphoto2CameraManager.h"
#include "Camera.h"

int main()
{
    CameraManager *mgr = Gphoto2CameraManager::instance();

    mgr->detectCameras();
    mgr->captureFromAll("/tmp", true);

    delete mgr;
    return 0;
}
