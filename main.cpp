// Module library initialization code

#ifndef _WIN32
# include <signal.h>
#endif

#include <boost/shared_ptr.hpp>

#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>
#include "fgrab.h"

// export the entry point
#ifdef _WIN32
  #define ALCALL __declspec(dllexport)
#else
  #define ALCALL
#endif

extern "C"
{
  ALCALL int _createModule(boost::shared_ptr<AL::ALBroker> pBroker)
  {
    // init broker with the main broker instance
    // from the parent executable
    AL::ALBrokerManager::setInstance(pBroker->fBrokerManager.lock());
    AL::ALBrokerManager::getInstance()->addBroker(pBroker);
    // create module instances
    AL::ALModule::createModule<FGrab>(pBroker, "FGrab");
    return 0;
  }

  ALCALL int _closeModule(  )
  {
    return 0;
  }
}

