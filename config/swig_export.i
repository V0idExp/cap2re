%module cap2re
%{
#include "config.h"
#include "Common.h"
#include "Camera.h"
#include "CameraManager.h"

#ifdef HAVE_GPHOTO2
#include "Gphoto2CameraManager.h"
#endif
%}

%include "std_string.i"
typedef std::string String;

%include "std_vector.i"
%template(CameraPtrList) std::vector<Camera*>;
%template(StringList) std::vector<String>;

%include "config.h"
%include "Common.h"
%include "Camera.h"
%include "CameraManager.h"

#ifdef HAVE_GPHOTO2
%include "Gphoto2CameraManager.h"
#endif
