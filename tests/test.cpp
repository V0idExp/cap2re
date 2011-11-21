/* 
 * Copyright (C) 2011 Ivan Nikolaev
 *
 * This file is part of cap2re.
 *
 * cap2re is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * cap2re is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with cap2re. 
 * If not, see http://www.gnu.org/licenses
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <boost/regex.hpp>
using namespace std;

#include "ConfigOptsTree.h"
#include "CameraManager.h"
#include "Gphoto2CameraManager.h"


int main()
{
    CameraManager *mgr = Gphoto2CameraManager::instance();

    CameraPtrList cams = mgr->detectCameras();
    cout << "Camere rilevate: " << cams.size() << endl;

    for(int i = 0; i < cams.size(); i++)
    {
        cout << "-- Camera n." << i+1 << " --" << endl;

        cout << "Aperture size:\t" << cams[i]->getApertureSize() << endl;
        cout << "Exposure time:\t" << cams[i]->getExposureTime() << endl;
        cout << "Serial number:\t" << cams[i]->getSerialNo() << endl;
        cams[i]->capture("/tmp");
    }

    delete mgr;
    return 0;
}
