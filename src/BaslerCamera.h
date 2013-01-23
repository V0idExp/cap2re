/*
 * Copyright (C) 2012 Ivan Nikolaev
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

#ifndef CAP2RE_BASLER_CAMERA_H
#define CAP2RE_BASLER_CAMERA_H

#include "Camera.h"

#include <pylon/gige/BaslerGigECamera.h>
typedef Pylon::CBaslerGigECamera BaslerCamera_t;

class BaslerCamera : public Camera
{
public:
	BaslerCamera(const String& name, BaslerCamera_t* cam);
	~BaslerCamera();

	String
	capture(const String &outDir);

	String
	getSerialNo() const;

	/**
	  * Set camera's exposure time in microseconds. Must be convertable to int value.
	  */
	void
	setExposureTime(const String& value);

	String
	getExposureTime() const;

	StringList
	getExposureTimeValues() const;

	void
	setApertureSize(const String& value);

	String
	getApertureSize() const;

	StringList
	getApertureSizeValues() const;

private:
	String
	saveJpegImage(const String& outdir, const unsigned char* buffer, int width, int height);

	BaslerCamera_t*                  _cam;
	Pylon::StreamBufferHandle*       _hBuffer;
	uint8_t*                         _pBuffer;
	BaslerCamera_t::StreamGrabber_t* _streamGrabber;
};

// CAP2RE_BASLER_CAMERA_H
#endif