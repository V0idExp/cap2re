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

#include "config.h"
#include "BaslerCameraManager.h"
#include "BaslerCamera.h"
#include "Error.h"

#include <cstdlib>

using namespace std;

BaslerCameraManager::BaslerCameraManager() {
	// very important piece of code - environment variables setting
	if(!getenv("PYLON_ROOT") || !getenv("GENICAM_ROOT_V2_1")) {
		setenv("PYLON_ROOT", PYLON_ROOT, 1);
		setenv("GENICAM_ROOT_V2_1", GENICAM_ROOT, 1);
	}

	Pylon::PylonInitialize();
}

BaslerCameraManager::~BaslerCameraManager() {
	Pylon::PylonTerminate(false);
}

CameraPtrList
BaslerCameraManager::detectCameras() {
	// wipe the list of already detected cameras
	_cameras = CameraPtrList();

	// Get the transport layer factory
	Pylon::CTlFactory& TlFactory = Pylon::CTlFactory::GetInstance();

	// Create the transport layer object needed to enumerate or
	// create a camera object of type Camera_t::DeviceClass()
	Pylon::ITransportLayer *pTl = TlFactory.CreateTl(BaslerCamera_t::DeviceClass());

	// Exit the application if the specific transport layer is not available
	if (!pTl)
		throw RuntimeError("BaslerCameraManager: Failed to create transport layer!");

	// Get all attached cameras and exit the application if no camera is found
	Pylon::DeviceInfoList_t devices;
	if (0 == pTl->EnumerateDevices(devices))
		return _cameras; // briefly, an empty list

	// Create the BaslerCamera instance for each attached camera
	for(Pylon::DeviceInfoList_t::iterator it = devices.begin(); it != devices.end(); it++)
	{
		// Create the camera object of the first available camera.
		// The camera object is used to set and get all available
		// camera features.
		try {
			BaslerCamera_t* cam = new BaslerCamera_t(pTl->CreateDevice(*it));
			_cameras.push_back(new BaslerCamera(cam->GetDeviceInfo().GetSerialNumber().c_str(), cam));
		} catch(GenICam::GenericException e) {
			throw RuntimeError(String("BaslerCameraManager: Error occurred while trying to create Camera object:\n") + e.what());
		}
	}

	return _cameras;
}