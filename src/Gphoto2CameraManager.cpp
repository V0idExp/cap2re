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

#include "Gphoto2CameraManager.h"
#include "Gphoto2Camera.h"
#include "Error.h"

#include <sstream>

#include <boost/thread.hpp>
using namespace boost;


CameraManager*
Gphoto2CameraManager::instance()
{
    static CameraManager *_inst = NULL;
    if(!_inst)
        _inst = new Gphoto2CameraManager;

    return _inst;
}

Gphoto2CameraManager::Gphoto2CameraManager():
    _context(gphoto2::gp_context_new())
{
    if(!_context)
        throw RuntimeError("Gphoto2CameraManager: can't initialize GPContext");
}

Gphoto2CameraManager::~Gphoto2CameraManager()
{
    // Unreference the context, when the reference count goes to 0, gphoto2 frees the object
    gphoto2::gp_context_unref(_context);
}

CameraPtrList
Gphoto2CameraManager::detectCameras()
{
    // Get the abilities list for current GP implementation
    gphoto2::CameraAbilitiesList* caList;
    gp_abilities_list_new(&caList); // init

    if(gp_abilities_list_load(caList, _context) < GP_OK)
        throw RuntimeError("Gphoto2CameraManager: can't load abilities list");

    // Get information about system ports (USB, serial, etc.)
    gphoto2::GPPortInfoList* piList;
    gp_port_info_list_new(&piList); // init

    if(gp_port_info_list_load(piList) < GP_OK)
        throw RuntimeError("Gphoto2CameraManager: can't load port info list");

    // Prepare the camera list
    gphoto2::CameraList* cl;
    gp_list_new(&cl);

    // Autodetect attached cameras
    if(gp_abilities_list_detect(caList, piList, cl, _context) < GP_OK)
        throw RuntimeError("Gphoto2CameraManager: can't detect attached cameras");

    for(int i = 0; i < gp_list_count(cl); i++)
    {
        std::ostringstream camName;
        camName << "Camera" << i+1;
        const char *model, *portName;
        gphoto2::CameraAbilities ca;
        gphoto2::GPPortInfo pi;

        // Retrieve camera model
        gp_list_get_name(cl, i, &model);
        gp_list_get_value(cl, i, &portName);

        // Retrieve the abilities of the given camera model
        int a = gp_abilities_list_lookup_model(caList, model);
        gp_abilities_list_get_abilities(caList, a, &ca);

        // Retrieve the port at which the camera is connected
        int p = gp_port_info_list_lookup_path(piList, portName);
        gp_port_info_list_get_info(piList, p, &pi);

        // Create and init new Camera object
        gphoto2::Camera *cam;

        gp_camera_new(&cam);
        gp_camera_set_abilities(cam, ca);
        gp_camera_set_port_info(cam, pi);

        // Add the camera object to camera list
        _cameras.push_back(new Gphoto2Camera(camName.str(), cam, gphoto2::gp_context_new()));
    }

    gp_list_free(cl);
    return _cameras;
}

void captureThread(Camera *cam, const String &outdir, String *file)
{
    *file = cam->capture(outdir);
}

StringList
Gphoto2CameraManager::captureFromAll(const String &outdir, bool parallelize)
{
    StringList imageFiles(_cameras.size());

    if(!parallelize)
    {
        for(int c = 0; c < _cameras.size(); c++)
            imageFiles[c] = _cameras[c]->capture(outdir);
    }
    else
    {
        boost::thread threads[_cameras.size()];

        for(int c = 0; c < _cameras.size(); c++)
        {
            threads[c] = thread(captureThread, _cameras[c], outdir, &imageFiles[c]);
        }

        for(int t = 0; t < _cameras.size(); t++)
            threads[t].join();

    }

    return imageFiles;
}
