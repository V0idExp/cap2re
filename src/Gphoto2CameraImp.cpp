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

#include "Gphoto2CameraImp.h"
#include "Error.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

void
buildConfigTree(Node *node, gphoto2::CameraWidget *w)
{
    const char *name;
    gp_widget_get_name(w, &name);

    int childrenCount = gp_widget_count_children(w);

    if(!childrenCount)
        node->addChild(new Node(name));
    else
        for(int c = 0; c < childrenCount; c++)
        {
            gphoto2::CameraWidget *child;
            gp_widget_get_child(w, c, &child);
            buildConfigTree(node, child);
        }
}

ConfigOptsTree*
loadCameraConfiguration(gphoto2::Camera *gp2cam, gphoto2::GPContext *gp2context)
{
    ConfigOptsTree *cfg = new ConfigOptsTree();
    gphoto2::CameraWidget *root;
    gp_camera_get_config(gp2cam, &root, gp2context);

    buildConfigTree(cfg->getEntry(""), root);

    return cfg;
}

Gphoto2CameraImp::Gphoto2CameraImp(gphoto2::Camera *gp2Cam, gphoto2::GPContext *gp2Context):
    _camera(gp2Cam),
    _context(gp2Context)
{
    // Build and store camera configuration
    _config = loadCameraConfiguration(gp2Cam, gp2Context);

    // Retrieve a copy of camera abilities
    gp_camera_get_abilities(_camera, &_abilities);

    // Initialize the camera
    gp_camera_init(_camera, _context);

}

Gphoto2CameraImp::~Gphoto2CameraImp()
{
    gp_camera_exit(_camera, _context);
    gp_camera_unref(_camera);

    delete _config;
}

String
Gphoto2CameraImp::capture(const String &outDir)
{
    // Path on camera where the image is stored
    gphoto2::CameraFilePath  out;

    // gphoto2 file management data
    gphoto2::CameraFile *file;
    int fd;

    // Path to file
    String filename = outDir + "/imgXXXXXX";
    char tmpfilename[filename.length()+1];
    strcpy(tmpfilename, filename.c_str());

    // Capture the image. For now, the result is stored on camera's memory (RAM or SD)
    if(gp_camera_capture(_camera, gphoto2::GP_CAPTURE_IMAGE, &out, _context) < GP_OK)
        throw RuntimeError("something gone wrong when trying to capture image from gphoto2 camera");

    // Create a temporary file with a random name
    fd = mkstemp(tmpfilename);

    // Create a gphoto2 file struct from file descripter obtained by mkstemp()
    if(gp_file_new_from_fd(&file, fd) < GP_OK)
        throw RuntimeError("can't create temporary file");

    // Save the image into the temporary file
    if(gp_camera_file_get(_camera, out.folder, out.name, gphoto2::GP_FILE_TYPE_NORMAL, file, _context) < GP_OK)
        throw RuntimeError("can't download the image from camera to temporary file");

    return tmpfilename;
}

void
Gphoto2CameraImp::setOption(const String &path, const String &value)
{
    //TODO
}

String
Gphoto2CameraImp::getOption(const String &path) const
{
    //TDOO
}

StringList
Gphoto2CameraImp::getOptionValues(const String &path) const
{
    //TODO
}
