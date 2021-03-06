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

#ifndef CAP2RE_BASLER_CAMERA_MANAGER_H
#define CAP2RE_BASLER_CAMERA_MANAGER_H

#include "CameraManager.h"

class BaslerCameraManager : public CameraManager
{
public:
	BaslerCameraManager();
	~BaslerCameraManager();

	CameraPtrList
	detectCameras();
};

// BASLER_CAMERA_MANAGER_H
#endif
