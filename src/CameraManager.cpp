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

#include "CameraManager.h"

#if HAVE_BOOST_THREAD
	#include <boost/thread.hpp>
	using namespace boost;
#endif

// Function invoked in each thread. The file argument must be the pointer to
// an already allocated String object
void captureThread(Camera *cam, const String &outdir, String *file)
{
	*file = cam->capture(outdir);
}

CameraPtrList
CameraManager::getCameras() const
{
	return _cameras;
}

Camera*
CameraManager::getCamera(const String &serialNo) const
{
	for(CameraPtrList::const_iterator cam = _cameras.begin(); cam != _cameras.end(); cam++)
		if((*cam)->getSerialNo() == serialNo)
			return *cam;

	return NULL;
}

StringList
CameraManager::captureFromAll(const String &outdir, bool parallelize)
{
	StringList imageFiles(_cameras.size());

#if HAVE_BOOST_THREAD == 0
	parallelize = false;
#endif

	if(!parallelize)
	{
		for(int c = 0; c < _cameras.size(); c++)
			imageFiles[c] = _cameras[c]->capture(outdir);
	}
#if HAVE_BOOST_THREAD
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
#endif

	return imageFiles;
}
