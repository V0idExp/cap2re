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

#ifndef GPHOTO2CAMERAIMP_H
#define GPHOTO2CAMERAIMP_H

#include "Camera.h"

namespace gphoto2
{
    #include <gphoto2/gphoto2.h>
}

class Gphoto2Camera : public Camera
{
public:
    Gphoto2Camera(gphoto2::Camera *gp2Cam, gphoto2::GPContext *gp2Context);
    ~Gphoto2Camera();

    String
    capture(const String &outDir);
    String
    getSerialNo() const;

    void
    setOption(const String &path, const String &value);
    String
    getOption(const String &path) const;
    StringList
    getOptionValues(const String &path) const;

    // Some convenience properties
    void
    setExposureTime(const String &value);
    String
    getExposureTime() const;
    StringList
    getApertureSizeValues() const;

    void
    setApertureSize(const String &value);
    String
    getApertureSize() const;
    StringList
    getExposureTimeValues() const;

private:
    gphoto2::CameraAbilities    _abilities;
    gphoto2::Camera*            _camera;
    gphoto2::GPContext*         _context;
};

#endif // GPHOTO2CAMERAIMP_H
