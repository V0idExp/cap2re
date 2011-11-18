%module cap2re
%{
#include "Camera.h"
#include "CameraManager.h"
#include "Gphoto2CameraManager.h"
%}

%include "std_string.i"
typedef std::string String;

%include "std_vector.i"
%template(CameraPtrList) std::vector<Camera*>;
%template(StringList) std::vector<String>;

%include "Common.h"
%include "Camera.h"
%include "CameraManager.h"
%include "Gphoto2CameraManager.h"
