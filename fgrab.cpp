#include "fgrab.h"
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alvision/alimage.h>
#include <alvision/alvisiondefinitions.h>
#include <alproxies/alvideodeviceproxy.h>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <qi/log.hpp>
#include <qi/os.hpp>



FGrabError::FGrabError(const char* message) : msg(message) {}

FGrabError::FGrabError(const std::string message) : msg(message.c_str()) {}

const char* FGrabError::what() {
  return msg;
}


struct FGrab::Impl {
  std::string nameId;
  boost::shared_ptr<AL::ALVideoDeviceProxy> cameraProxy;
  FGrab &module;

  Impl(FGrab &mod) : nameId("FGrab_VM"), module(mod) {
    try {
      qiLogVerbose("FGrab") << "Constructing FGrab implementation" << std::endl;
      cameraProxy = boost::shared_ptr<AL::ALVideoDeviceProxy>
          (new AL::ALVideoDeviceProxy(mod.getParentBroker()));
    } catch (const AL::ALError& e) {
      qiLogError("FGrab") << "Error while getting proxy of ALVideoDevice: "
                          << e.toString() << std::endl;
      throw FGrabError("Could not get a proxy to the video device module");
    }
    if (!cameraProxy) {
      qiLogError("FGrab") << "Failed to get a proxy of ALVideoDevice" << std::endl;
      throw FGrabError("Could not get a proxy to the video device module");
    }
    nameId = cameraProxy->subscribe(nameId, AL::k4VGA, AL::kYUV422ColorSpace, 30);
    // The video module needs an explicit call to setFrameRate to get
    // higher framerates than 5 fps in high def.
    cameraProxy->setFrameRate(nameId, 30);
    cameraProxy->setParam(AL::kCameraAutoExpositionID, 1);
    // if (nameId == NULL) {
    //   throw FGrabError("Could not subscribe to camera");
    // }
    qiLogVerbose("FGrab") << "Successfully registered FGrab to the camera proxy"
                          << std::endl;
  }

  ~Impl() {
    releaseFrameBuffer();
    try {
      if(cameraProxy) {
        // Unregister the video module.
        cameraProxy->unsubscribe(nameId);
      }
      cameraProxy.reset(); // explicit shared_ptr ref count decr
    } catch(const AL::ALError& e) {
      qiLogError("FGrab: Error in Impl desctructor: ") <<  e.toString() << std::endl;
    }
  }

  /*  This implementation uses DIRECT ACCESS to the latest raw (YUV422)
   *  buffer of the camera driver. hence,
   *
   *    NO CAMERA SWITCH, MODE/RESOLUTION CHANGE, ETC... SHOULD BE
   *    PERFORMED WHEN THE IMAGE BUFFER IS BEING ACCESSED.
   */

  int getActiveCamera() {
    // TODO
    return 0;
  }

  int switchCamera(int id) {
    // TODO
    // Wait for mutex to prevent switching during access.
    return 0;
  }

  AL::ALImage * getFrameBuffer() {
    // TODO: Mark access using mutex    
    AL::ALImage *img = static_cast<AL::ALImage *>(cameraProxy->getDirectRawImageLocal(nameId));
    return img;
  }

  void releaseFrameBuffer() {
    cameraProxy->releaseDirectRawImage(nameId);
    // TODO: Release mutex
  }
};

FGrab::FGrab(boost::shared_ptr<AL::ALBroker> broker,
             const std::string& name)
    : AL::ALModule(broker, name) {
  // Describe the module here. This will appear on the webpage
  setModuleDescription("Grab raw frames from the active camera.");

  functionName("grab", getName(), "Grab a frame and write it in a file.");
  addParam("file", "The path and name of the file to be written.");
  BIND_METHOD(FGrab::grab);
}

FGrab::~FGrab() {}

void FGrab::init() {
  /* Init is called just after construction. */
  try {
    impl = boost::shared_ptr<Impl>(new Impl(*this));
  } catch (std::exception& e) {
    qiLogError("FGrab") << "Failed to initialize FGrab module: " << e.what() << std::endl;
    exit();
  }
}

void FGrab::exit() {
  // I think that no manual cleanup is needed.
  AL::ALModule::exit();
}
  
void FGrab::grab(const std::string &file) {
  qiLogVerbose("FGrab") << "Trying to save one frame to " << file << std::endl;
  // ALImage ref doc at
  // http://www.aldebaran-robotics.com/documentation/ref/libalvision/classAL_1_1ALImage.html
  // Should not be a boost::scoped_ptr because freed by the vision module.
  AL::ALImage *img = impl->getFrameBuffer();
  if (!img) {
    qiLogError("FGrab") << "Could not get the latest camera image" << std::endl;    
    impl->releaseFrameBuffer();
    return;
  }
  
  int width = img->getWidth();
  int height = img->getHeight();
  int nbLayers = img->getNbLayers();
  int colorSpace = img->getColorSpace();
  long long timeStamp = img->getTimeStamp();
  unsigned int size = img->getSize(); // data filed size in bytes
  qiLogVerbose("FGrab") << "Image: " << width << "x" << height
                        << ", nbLayers: " << nbLayers
                        << ", colorspace: " << colorSpace
                        << ", timestamp: " << timeStamp
                        << ", size: " << size
                        << std::endl;

  FILE *f = fopen(file.c_str(), "wb"); // "wb" for pics
  if (!f) {
    qiLogError("FGrab", "Error opening the file %s for writing", file.c_str());
  } else {
    try {
      fwrite(img->getData(), 1, img->getSize(), f);
    } catch (std::bad_alloc& e) {
      qiLogError("FGrab") << "Could not write image: out of memory" << std::endl;
    } catch (std::exception& e) {
      qiLogError("FGrab") << "File write failed" << std::endl;
    }
    fclose(f);
  }
  impl->releaseFrameBuffer();
}
