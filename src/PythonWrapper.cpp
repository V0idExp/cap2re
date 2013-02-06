#include <Python.h>
#include "Cap2re.h"
#include "Error.h"
#include <iostream>
using namespace std;

typedef struct {
	PyObject_HEAD
	Camera* camObj;
} CameraWrap;

static PyTypeObject cameraType = {
	PyObject_HEAD_INIT(NULL)
	0,
	"cap2re.Camera",
	sizeof(CameraWrap),
	0,                         /*tp_itemsize*/
	0,                         /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT,        /*tp_flags*/
	"Camera object.	Provides a generic interface to real devices.",
};

static void
Camera_del(CameraWrap* self) {
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject*
Camera_new(Camera* obj) {
	CameraWrap* self;

	self = (CameraWrap*)cameraType.tp_alloc(&cameraType, 0);
	self->camObj = obj;

	return (PyObject*)self;
}

static PyObject*
Camera_capture(CameraWrap* self, PyObject* args) {
	// extract the output directory from arguments
	const char* destdir;
	PyArg_ParseTuple(args, "s", &destdir);

	PyObject* filename;

	if(self->camObj) {
		try {
			filename = PyString_FromString(self->camObj->capture(destdir).c_str());
		} catch(RuntimeError &e) {
			PyErr_SetString(PyExc_RuntimeError, e.what());
			return NULL;
		}
	}

	return filename;
}

static PyObject*
Camera_getSerialNo(CameraWrap* self, PyObject* args) {
	return PyString_FromString(self->camObj->getSerialNo().c_str());
}

static PyObject*
Camera_getName(CameraWrap* self, PyObject* args) {
	return PyString_FromString(self->camObj->getName().c_str());
}

static PyObject*
Camera_setName(CameraWrap* self, PyObject* args) {
	const char* value;
	PyArg_ParseTuple(args, "s", &value);

	self->camObj->setName(value);

	Py_RETURN_NONE;
}


static PyObject*
Camera_getExposureTime(CameraWrap* self, PyObject* args) {
	return PyString_FromString(self->camObj->getExposureTime().c_str());
}

static PyObject*
Camera_setExposureTime(CameraWrap* self, PyObject* args) {
	const char* value;
	PyArg_ParseTuple(args, "s", &value);

	self->camObj->setExposureTime(value);

	Py_RETURN_NONE;
}

static PyObject*
Camera_getExposureTimeValues(CameraWrap* self, PyObject* args) {
	StringList values;
	PyObject* list;

	values = self->camObj->getExposureTimeValues();
	list = PyList_New(values.size());

	for(int v = 0; v < values.size(); v++) {
		PyList_SetItem(list, v, PyString_FromString(values[v].c_str()));
	}

	return list;
}

static PyObject*
Camera_getApertureSize(CameraWrap* self, PyObject* args) {
	return PyString_FromString(self->camObj->getApertureSize().c_str());
}

static PyObject*
Camera_setApertureSize(CameraWrap* self, PyObject* args) {
	const char* value;
	PyArg_ParseTuple(args, "s", &value);

	self->camObj->setApertureSize(value);

	Py_RETURN_NONE;
}

static PyObject*
Camera_getApertureSizeValues(CameraWrap* self, PyObject* args) {
	StringList values;
	PyObject* list;

	values = self->camObj->getApertureSizeValues();
	list = PyList_New(values.size());

	for(int v = 0; v < values.size(); v++) {
		PyList_SetItem(list, v, PyString_FromString(values[v].c_str()));
	}

	return list;
}

static PyMethodDef cameraMethods[] = {
	{"capture", (PyCFunction)Camera_capture, METH_VARARGS, "Acquire an image and save it into destination dir."},
	{"getSerialNo", (PyCFunction)Camera_getSerialNo, METH_NOARGS, "Get camera's serial number."},
	{"getName", (PyCFunction)Camera_getName, METH_NOARGS, "Get the name of device."},
	{"setName", (PyCFunction)Camera_setName, METH_VARARGS, "Change the camera name."},
	{"getExposureTime", (PyCFunction)Camera_getExposureTime, METH_NOARGS, "Get currently set exposure time."},
	{"setExposureTime", (PyCFunction)Camera_setExposureTime, METH_VARARGS, "Change camera's exposure time."},
	{"getExposureTimeValues", (PyCFunction)Camera_getExposureTimeValues, METH_NOARGS, "Get the list of valid exposure time values. Can be a list of discrete values or a range."},
	{"getApertureSize", (PyCFunction)Camera_getApertureSize, METH_NOARGS, "Get the size of lens aperture. May be unavailable on some devices."},
	{"setApertureSize", (PyCFunction)Camera_setApertureSize, METH_VARARGS, "Set the lens aperture. May be unavailable on some devices."},
	{"getApertureSizeValues", (PyCFunction)Camera_getApertureSizeValues, METH_NOARGS, "Get the list of valid values for aperture size setting. If the device does not provide such facility, will be empty."},
	{NULL}
};

typedef struct {
	PyObject_HEAD
	CameraManager* mgrObj;
} CameraManagerWrap;

static PyTypeObject cameraMgrType = {
	PyObject_HEAD_INIT(NULL)
	0,
	"cap2re.CameraManager",
	sizeof(CameraManagerWrap),
	0,                         /*tp_itemsize*/
	0,                         /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT,        /*tp_flags*/
	"The main interface for backend's facilities. The manager is responsible\n\
	of detecting and handling the devices which it supports.",
};

static void
CameraManager_del(CameraManagerWrap* self) {
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject*
CameraManager_new(CameraManager* obj) {
	CameraManagerWrap* self;

	self = (CameraManagerWrap*)cameraMgrType.tp_alloc(&cameraMgrType, 0);
	self->mgrObj = obj;

	return (PyObject*)self;
}

static PyObject*
CameraManager_detectCameras(CameraManagerWrap* self, PyObject* args) {
	CameraPtrList cameras;
	PyObject* list;

	try {
		cameras = self->mgrObj->detectCameras();
		list = PyList_New(cameras.size());

		for(int c = 0; c < cameras.size(); c++) {
			PyObject* pycam = Camera_new(cameras[c]);
			PyList_SetItem(list, c, pycam);
		}
	} catch(RuntimeError &e) {
		PyErr_SetString(PyExc_RuntimeError, e.what());
		return NULL;
	}

	return list;
}

static PyObject*
CameraManager_captureFromAll(CameraManagerWrap* self, PyObject* args) {
	StringList imageFiles;
	PyObject* list;
	bool parallelize = false;
	char *outdir;

	// get the output directory path and parallelization flag
	PyArg_ParseTuple(args, "s|i", &outdir, &parallelize);

	try {
		imageFiles = self->mgrObj->captureFromAll(outdir, parallelize);
		list = PyList_New(imageFiles.size());

		for(int i = 0; i < imageFiles.size(); i++) {
			PyList_SetItem(list, i, PyString_FromString(imageFiles[i].c_str()));
		}
	} catch(RuntimeError &e) {
		PyErr_SetString(PyExc_RuntimeError, e.what());
		return NULL;
	}

	return list;
}

static PyObject*
CameraManager_getCameras(CameraManagerWrap* self, PyObject* args) {
	CameraPtrList cameras;
	PyObject* list;

	cameras = self->mgrObj->getCameras();
	list = PyList_New(cameras.size());

	for(int c = 0; c < cameras.size(); c++) {
		PyObject* pycam = Camera_new(cameras[c]);
		PyList_SetItem(list, c, pycam);
	}

	return list;
}

static PyObject*
CameraManager_getCamera(CameraManagerWrap* self, PyObject* args) {
	PyObject* pycam;
	Camera* cam;
	char *name;

	// get the camera name string
	PyArg_ParseTuple(args, "s", &name);

	cam = self->mgrObj->getCamera(name);
	if(cam)
		pycam = Camera_new(cam);
	else
		Py_RETURN_NONE;

	return pycam;
}

static PyMethodDef cameraMgrMethods[] = {
	{"detectCameras", (PyCFunction)CameraManager_detectCameras, METH_NOARGS, "Search for cameras supported by the backend."},
	{
		"captureFromAll",
		(PyCFunction)CameraManager_captureFromAll,
		METH_VARARGS,
		"Acquire images from all cameras currently known by the backend.\n\
		Images are saved in the given directory. If the parallelize argument\n\
		is True, the manager will try to capture images in parallel. Note,\n\
		that this feature may not be available on all backends or it may be\n\
		disabled at build time. "
	},
	{"getCameras", (PyCFunction)CameraManager_getCameras, METH_NOARGS, "Get the list of previously detected cameras."},
	{"getCamera", (PyCFunction)CameraManager_getCamera, METH_VARARGS, "Get a specific camera by name."},
	{NULL}
};

/**
  * Export of cap2re::initialize()
  */
static PyObject*
cap2re_init(PyObject* self, PyObject* args) {
	cap2re::initialize();

	Py_RETURN_NONE;
}

/**
  * Export of cap2re::shutdown()
  */
static PyObject*
cap2re_shutdown(PyObject* self, PyObject* args) {
	cap2re::shutdown();

	Py_RETURN_NONE;
}

/**
  * Export of cap2re::getAvailableBackends()
  */
static PyObject*
cap2re_getAvailableBackends(PyObject* self, PyObject* args) {
	StringList names = cap2re::getAvailableBackends();

	PyObject* list = PyTuple_New(names.size());
	for(int n = 0; n < names.size(); n++)
		PyTuple_SetItem(list, n, PyString_FromString(names[n].c_str()));

	return list;
}

/**
  * Export of cap2re::getBackend()
  */
static PyObject*
cap2re_getBackend(PyObject* self, PyObject* args) {
	CameraManager* backend;
	PyObject* pybackend;
	char* name;

	PyArg_ParseTuple(args, "s", &name);

	backend = cap2re::getBackend(name);
	if(backend)
		pybackend = CameraManager_new(backend);
	else
		Py_RETURN_NONE;

	return pybackend;
}

static PyMethodDef methods[] = {
	{
		"initialize",
		cap2re_init,
		METH_NOARGS,
		"Initialize cap2re.\n\
		Call this function before doing anything else with the library."
	},

	{
		"shutdown",
		cap2re_init,
		METH_NOARGS,
		"Shutdown cap2re.\n\
		Always call this function when you have done with the library."
	},

	{
		"getAvailableBackends",
		cap2re_getAvailableBackends,
		METH_NOARGS,
		"Get the list of available backends.\n\
		Use the getBackend() function with the name of interested backend, returned by\n\
		this func, to obtain the instance of CameraManager for that backend."
	},

	{
		"getBackend",
		cap2re_getBackend,
		METH_VARARGS,
		"Get the CameraManager instance for given backend."
	},

	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initcap2re() {
	// export the Camera wrapper
	cameraType.tp_methods = cameraMethods;
	cameraType.tp_dealloc = (destructor)Camera_del;

	if(PyType_Ready(&cameraType)< 0)
		return;

	// export the CameraManager wrapper
	cameraMgrType.tp_methods = cameraMgrMethods;
	cameraMgrType.tp_dealloc = (destructor)CameraManager_del;

	if(PyType_Ready(&cameraMgrType)< 0)
		return;

	PyObject* module = Py_InitModule("cap2re", methods);
	Py_INCREF(&cameraType);
	Py_INCREF(&cameraMgrType);
	PyModule_AddObject(module, "Camera", (PyObject*)&cameraType);
	PyModule_AddObject(module, "CameraManager", (PyObject*)&cameraMgrType);
}