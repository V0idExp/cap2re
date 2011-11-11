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

#ifndef CAMERAIMP_H
#define CAMERAIMP_H

#include "ConfigOptsTree.h"
#include "Common.h"

class CameraImp
{
public:
    virtual String          capture() = 0;

    virtual void            setOption(const String &path, const String &value) = 0;
    virtual String          getOption(const String &path) const = 0;
    virtual StringList      getOptionValues(const String &path) const = 0;
    virtual ConfigOptsTree  getConfiguration() const;

protected:
    ConfigOptsTree  _config;
};

#endif // CAMERAIMP_H
