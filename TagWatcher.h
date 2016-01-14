//------------------------------------------------------------------------
// TagWatcher.h - Library for detecting changes in the ID-* line of RFID readers
//------------------------------------------------------------------------

#ifndef TagWatcher_h
#define TagWatcher_h

#include "Arduino.h"
#include "Timer.h"                     //http://github.com/JChristensen/Timer

extern "C" {
  typedef void (*tagCallbackFunction)(String tag);
}


class TagWatcher{
  
public:

  TagWatcher(int resetPin);
  void onTagChange(tagCallbackFunction newFunction);
  void tick(void);
  void init(void);
  
private:

  //Because we use the Timer class, we have to make this static.
  static boolean _running;
  
  String _previousContent; 
  int _resetPin;
  tagCallbackFunction _tagChangeFunc;
  Timer _timer;

  void checkTag();
  void resetReader();

  //Because we use the Timer class, we have to make this static.
  static void startReader(void *context);
  
};

#endif


