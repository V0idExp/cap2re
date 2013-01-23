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

#ifndef CAP2RE_H
#define CAP2RE_H

#include "CameraManager.h"
#include "Common.h"

namespace cap2re {

/**
 * Initialize the library.
 *
 * ALWAYS call this before doing anything.
 */
extern void
initialize();

/**
 * Cleanup function.
 *
 * ALWAYS call this when you finished using the library, memleaks are granted
 * otherwise.
 */
extern void
shutdown();

/**
 * Get the list of available backend implementations.
 *
 * This function returns a list of backend names, which can be used to obatin
 * a instance of CameraManager.
 */
extern StringList
getAvailableBackends();

/**
 * Get the instance of CameraManager for given backend. Returns NULL if a
 * backend with given name does not exist.
 */
extern CameraManager*
getBackend(const String& name);

}

// CAP2RE_H
#endif