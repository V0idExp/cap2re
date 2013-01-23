#include "Cap2re.h"

#include <iostream>
using namespace std;

int main()
{
    cap2re::initialize();

    CameraManager *mgr;
    StringList images;

    mgr->detectCameras();

    cout << "Capture in single-threaded mode" << endl;
    images = mgr->captureFromAll("/tmp", false);

    cout << "Image files:" << endl;

    for(StringList::iterator img = images.begin(); img != images.end(); img++)
        cout << "   " << *img << endl;

    cout << endl;

    cout << "Capture in multi-threaded mode" << endl;
    images = mgr->captureFromAll("/tmp", true);

    cout << "Image files:" << endl;
    for(StringList::iterator img = images.begin(); img != images.end(); img++)
        cout << "   " << *img << endl;


    cap2re::shutdown();
    return 0;
}
