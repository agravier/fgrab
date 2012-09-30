#ifndef FGRAB_H
  #define FGRAB_H

#include <alcommon/almodule.h>
#include <boost/shared_ptr.hpp>
#include <exception>
#include <iostream>


namespace AL {
  // This is a forward declaration of AL:ALBroker which
  // avoids including <alcommon/albroker.h> in this header
  class ALBroker;
}


/**
 * This class inherits AL::ALModule. This allows it to bind methods
 * and be run as a remote executable within NAOqi
 */
class FGrab : public AL::ALModule {
public:
  FGrab(boost::shared_ptr<AL::ALBroker> broker,
        const std::string &name);

  ~FGrab();

  /**
   * Overloading ALModule::init().
   * This is called right after the module has been loaded.
   */
  virtual void init();

  /**
   * Overloading ALModule::exit().
   * This is called to unload the module.
   */
  virtual void exit();
  
  // Writes a raw picture from the active camera to the file
  void grab(const std::string &file);

  /* no copy
  FGrab & operator=(const FGrab&) = delete;
  FGrab(const FGrab&) = delete;
  FGrab() = default;
  */
private:
  struct Impl;
  boost::shared_ptr<Impl> impl;
};


class FGrabError : public std::exception {
public:
  FGrabError(const char* message);
  FGrabError(const std::string message);
  virtual const char* what();
private:
  const char* msg;
};


#endif // FGRAB_H
