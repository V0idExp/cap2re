#ifndef GPHOTO2CAMERAWRAPPER_H
#define GPHOTO2CAMERAWRAPPER_H

#include "Gphoto2Camera.h"


class Gphoto2CameraWrapper : public Gphoto2CameraWrapper
{
public:
    String      capture(const String &outDir)
    {

    }

    String      getSerialNo() const;

    void        setOption(const String &path, const String &value);
    String      getOption(const String &path) const;
    StringList  getOptionValues(const String &path) const;

    // Some convenience properties
    void        setExposureTime(const String &value);
    String      getExposureTime() const;

    void        setApertureSize(const String &value);
    String      getApertureSize() const;
#endif // GPHOTO2CAMERAWRAPPER_H
