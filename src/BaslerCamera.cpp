#include "BaslerCamera.h"
#include "Error.h"

#include <pylon/PylonIncludes.h>
using namespace Pylon;
using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;

#include <jpeglib.h>

#include <cstdlib>
#include <istream>
#include <ostream>
#include <iostream>
using namespace std;

BaslerCamera::BaslerCamera(const String& name, BaslerCamera_t* cam): Camera(name), _cam(cam) {
	// Open the camera
	cam->Open();

	// Set the image format and AOI
	cam->PixelFormat.SetValue(PixelFormat_Mono8);
	cam->OffsetX.SetValue(0);
	cam->OffsetY.SetValue(0);
	cam->Width.SetValue(cam->Width.GetMax());
	cam->Height.SetValue(cam->Height.GetMax());

	// We want to acquire one frame each time
	cam->AcquisitionMode.SetValue(AcquisitionMode_SingleFrame);

	// Set the exposure mode to timed
	cam->ExposureMode.SetValue(ExposureMode_Timed);

	//Disable acquisition start trigger if available
	{
		GenApi::IEnumEntry* acquisitionStart = cam->TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
		if ( acquisitionStart && GenApi::IsAvailable( acquisitionStart))
		{
			cam->TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
			cam->TriggerMode.SetValue( TriggerMode_Off);
		}
	}

	//Disable frame start trigger if available
	{
		GenApi::IEnumEntry* frameStart = cam->TriggerSelector.GetEntry( TriggerSelector_FrameStart);
		if ( frameStart && GenApi::IsAvailable( frameStart))
		{
			cam->TriggerSelector.SetValue( TriggerSelector_FrameStart);
			cam->TriggerMode.SetValue( TriggerMode_Off);
		}
	}

	// Get the first stream grabber object of the selected camera
	_streamGrabber = new BaslerCamera_t::StreamGrabber_t(cam->GetStreamGrabber(0));

	// Open the stream grabber
	_streamGrabber->Open();

	// Create an image buffer
	const size_t imgSize = (size_t)(cam->PayloadSize.GetValue());
	_pBuffer = new uint8_t[ imgSize ];

	// We won't use image buffers greater than imgSize
	_streamGrabber->MaxBufferSize.SetValue(imgSize);

	// We won't queue more than one image buffer at a time
	_streamGrabber->MaxNumBuffer.SetValue(1);

	// Allocate all resources for grabbing. Critical parameters like image
	// size now must not be changed until FinishGrab() is called.
	_streamGrabber->PrepareGrab();

	// Buffers used for grabbing must be registered at the stream grabber.
	// The registration returns a handle to be used for queuing the buffer.
	_hBuffer = new StreamBufferHandle(_streamGrabber->RegisterBuffer(_pBuffer, imgSize));

	// Put the buffer into the grab queue for grabbing
	_streamGrabber->QueueBuffer(*_hBuffer, NULL);
}

BaslerCamera::~BaslerCamera() {
	if(_cam) {
		_cam->AcquisitionStop.Execute();
		_streamGrabber->DeregisterBuffer(*_hBuffer);
		_streamGrabber->FinishGrab();
		_streamGrabber->Close();

		_cam->Close();

		delete _hBuffer;
		delete[] _pBuffer;
		delete _streamGrabber;
		delete _cam;
		_cam = NULL;
	}
}

String
BaslerCamera::capture(const String& outdir) {
	String outfile;

	// Let the camera acquire one single image ( Acquisiton mode equals
	// SingleFrame! )
	_cam->AcquisitionStart.Execute();

	// Wait for the grabbed image with a timeout of 3 seconds
	if (_streamGrabber->GetWaitObject().Wait(3000)) {
		// Get the grab result from the grabber's result queue
		GrabResult res;
		_streamGrabber->RetrieveResult(res);

		if (res.Succeeded()) {
			// Get the pointer to the image buffer
			const uint8_t *pImageBuffer = (uint8_t *) res.Buffer();

			// Save the buffer as a JPEG-compressed image in a unique filename inside outdir and return its name
			outfile =  saveJpegImage(outdir, pImageBuffer, res.GetSizeX(), res.GetSizeY());

			// Prepare for next frame
			_streamGrabber->QueueBuffer(res.Handle(), res.Context());
		}
		else
			throw RuntimeError("An error occurred when trying to capture the image from camera " + _name);
	}
	else {
		// Get the pending buffer back (You are not allowed to deregister
		// buffers when they are still queued)
		_streamGrabber->CancelGrab();

		// Get all buffers back
		for (GrabResult r; _streamGrabber->RetrieveResult(r););

		throw RuntimeError("Acquisition from camera " + this->getName() + " timed out!");
	}

	return outfile;
}

String
BaslerCamera::getSerialNo() const {
	return _cam->GetDeviceInfo().GetSerialNumber().c_str();
}

void
BaslerCamera::setExposureTime(const String& value) {
	istringstream s(value);
	int i;

	s >> i;
	_cam->ExposureTimeRaw.SetValue(i);
}

String
BaslerCamera::getExposureTime() const {
	ostringstream s;

	s << _cam->ExposureTimeRaw.GetValue();
	return s.str();
}

StringList
BaslerCamera::getExposureTimeValues() const {
	ostringstream min;
	ostringstream max;
	min << _cam->ExposureTimeRaw.GetMin();
	max << _cam->ExposureTimeRaw.GetMax();

	StringList res;
	res.push_back(min.str());
	res.push_back(max.str());
	return res;
}

String
BaslerCamera::getApertureSize() const {
	cerr << "BaslerCamera " << _name << ": getApertureSize() is unavailable on this device" << endl;
	return String();
}

void
BaslerCamera::setApertureSize(const String& value) {
	cerr << "BaslerCamera " << _name << ": setApertureSize() is unavailable on this device" << endl;
}

StringList
BaslerCamera::getApertureSizeValues() const {
	cerr << "BaslerCamera " << _name << ": getApertureSizeValues() is unavailable on this device" << endl;

	return StringList();
}

String
BaslerCamera::saveJpegImage(const String& outdir, const unsigned char* buffer, int width, int height) {
	// generate the image filename
	String filename = outdir + "/" + _name + "_XXXXXX";
	char tmpfilename[filename.length()+1];
	strcpy(tmpfilename, filename.c_str());
	FILE* outfile;

	// Create a temporary file with a random name
	int fd = mkstemp(tmpfilename);

	// open destination file
	if(fd == -1 || (outfile = fdopen(fd, "wb")) == NULL)
		throw RuntimeError("Can't open destination file for writing!");

	jpeg_compress_struct cinfo;
	jpeg_error_mgr jerr;

	JSAMPROW image[height];

	// Prepare the libjpeg-like buffer
	for(int row = 0, offset = 0; row < height; row++, offset++) {
		image[row] = (JSAMPLE*)(buffer + offset * width);
	}

	// setup compressor
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 1;
	cinfo.in_color_space = JCS_GRAYSCALE;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 100, TRUE);

	// compress!
	jpeg_start_compress(&cinfo, TRUE);
	jpeg_write_scanlines(&cinfo, image, height);
	jpeg_finish_compress(&cinfo);

	fclose(outfile);

	jpeg_destroy_compress(&cinfo);

	return tmpfilename;
}