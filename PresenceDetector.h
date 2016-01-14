//------------------------------------------------------------------------
// TagWatcher.h - Library for detecting changes in the ID-* line of RFID readers
//------------------------------------------------------------------------

#ifndef PresenceDetector_h
#define PresenceDetector_h

#include "Arduino.h"
#include "Timer.h"                     //http://github.com/JChristensen/Timer

extern "C" {
  typedef void (*presenceDetectedCallbackFunction)();
  typedef void (*presenceLostCallbackFunction)();
}


class PresenceDetector{
  
public:

  PresenceDetector(int sensorPin);
  void onPresenceDetected(presenceDetectedCallbackFunction newFunction);
  void onPresenceLost(presenceLostCallbackFunction newFunction);
  void tick(void);
  void resetTimeout();
  void stopTimeout();
	  
	  
  
private:

  //Because we use the Timer class, we have to make this static.
  static boolean _detected;
  static boolean _enabled;
  int _sensorPin;
  
  String _previousContent; 
  presenceDetectedCallbackFunction _presenceDetectedCallbackFunc;
  static presenceLostCallbackFunction _presenceLostCallbackFunc;
  Timer _timer;
  int8_t _timerId;


  //Because we use the Timer class, we have to make this static.
  static void losePresence(void *context);
  
};

#endif


