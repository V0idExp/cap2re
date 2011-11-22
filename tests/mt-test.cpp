#include "Common.h"
#include "Gphoto2CameraManager.h"
#include "Camera.h"

#include <iostream>
using namespace std;

int main()
{
    CameraManager *mgr = Gphoto2CameraManager::instance();
    StringList images;

    mgr->detectCameras();

    cout << "Capture in single-threaded mode" << endl;
    images = mgr->captureFromAll("/tmp", false);

    cout << "Image files:" << endl;
    for(auto img: images)
        cout << "   " << img << endl;

    cout << endl;

    cout << "Capture in multi-threaded mode" << endl;
    images = mgr->captureFromAll("/tmp", true);

    cout << "Image files:" << endl;
    for(auto img: images)
        cout << "   " << img << endl;


    delete mgr;
    return 0;
}
