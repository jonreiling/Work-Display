//------------------------------------------------------------------------
// TagWatcher.h - Library for detecting changes in the ID-* line of RFID readers
//------------------------------------------------------------------------

#ifndef StripManager_h
#define StripManager_h

#include "Arduino.h"
#include <SPI.h>
#include "Timer.h"                     //http://github.com/JChristensen/Timer
#include <Adafruit_DotStar.h>
#include "Tweener.h"




class StripManager{
  
public:

  StripManager();
  void tick(void);
  void setToPresent();
  void setToOn();
  void setToOff();
	  
	  
  
private:

  //Because we use the Timer class, we have to make this static.
  static uint32_t _topColor;
  static uint32_t _bottomColor;
  static int _currentPixel;
  static Adafruit_DotStar _strip;
  static int _previousPixel;
  
  Timer _timer;
  int8_t _timerId;
  Tweener _tweener;
  
  static void updatePixels(void *context);
};

#endif


