%module cap2re
%{
#include "Common.h"
#include "Cap2re.h"
#include "Camera.h"
#include "CameraManager.h"
%}

%include "std_string.i"
typedef std::string String;

%include "std_vector.i"
%template(CameraPtrList) std::vector<Camera*>;
%template(StringList) std::vector<String>;

%include "Common.h"
%include "Cap2re.h"
%include "Camera.h"
%include "CameraManager.h"