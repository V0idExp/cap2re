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
#include "Cap2re.h"

// Determine which backends are available at build time
#if HAVE_GPHOTO2
	#include "Gphoto2CameraManager.h"
#endif

#if HAVE_PYLON
	#include "BaslerCameraManager.h"
#endif

#include <map>

typedef std::map< String, CameraManager* > BackendsMap;
static BackendsMap gBackends;

void
cap2re::initialize() {
#if HAVE_GPHOTO2
	gBackends["libgphoto2"] = new Gphoto2CameraManager;
#endif

#if HAVE_PYLON
	gBackends["basler"] = new BaslerCameraManager;
#endif
}

void
cap2re::shutdown() {
	// Destroy camera managers
	for(BackendsMap::iterator pair = gBackends.begin(); pair != gBackends.end(); pair++)
		delete pair->second;

	gBackends.clear();
}

StringList
cap2re::getAvailableBackends() {
	StringList names;
	for(BackendsMap::iterator pair = gBackends.begin(); pair != gBackends.end(); pair++)
		names.push_back(pair->first);

	return names;
}

CameraManager*
cap2re::getBackend(const String& name) {
	try {
		return gBackends[name];
	}
	catch(Error) {
		return NULL;
	}
}
