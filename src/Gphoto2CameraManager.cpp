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
#include "Gphoto2CameraImp.h"
#include "Error.h"

Gphoto2CameraManager::Gphoto2CameraManager():
    _context(gphoto2::gp_context_new())
{
    if(!_context)
        throw RuntimeError("Gphoto2CameraManager: can't initialize gPhoto2 context");
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
    gp_abilities_list_load(caList, _context); // populate

    // Get information about system ports (USB, serial, etc.)
    gphoto2::GPPortInfoList* piList;
    gp_port_info_list_new(&piList); // init
    gp_port_info_list_load(piList); // populate

    // Prepare the camera list
    gphoto2::CameraList* cl;
    gp_list_new(&cl);

    // Autodetect attached cameras
    gp_abilities_list_detect(caList, piList, cl, _context);

    for(int i = 0; i < gp_list_count(cl); i++)
    {
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
        _cameras.push_back(new Camera(new Gphoto2CameraImp(cam, _context)));
    }

    return _cameras;
}

