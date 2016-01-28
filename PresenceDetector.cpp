//------------------------------------------------------------------------
// TagWatcher.cpp - Library for detecting changes in the ID-* line of RFID readers
//------------------------------------------------------------------------

#include "PresenceDetector.h"

boolean PresenceDetector::_detected = false;
boolean PresenceDetector::_enabled = true;
presenceLostCallbackFunction PresenceDetector::_presenceLostCallbackFunc = NULL;


// ----- Initialization and Default Values -----

PresenceDetector::PresenceDetector(int sensorPin){
 
  _sensorPin = sensorPin;
  _detected = false;
  _enabled = true;
  _presenceDetectedCallbackFunc = NULL;
  _presenceDetectedCallbackFunc = NULL;
  
} 


void PresenceDetector::onPresenceDetected(presenceDetectedCallbackFunction newFunction){
  _presenceDetectedCallbackFunc = newFunction;
}

void PresenceDetector::onPresenceLost(presenceLostCallbackFunction newFunction){
  _presenceLostCallbackFunc = newFunction;
}

void PresenceDetector::enableTimeout() {
  _enabled = true;
  resetTimeout();
}

void PresenceDetector::resetTimeout() {
  
  _timer.stop(_timerId);

  if ( _enabled ) {
  	_timerId = _timer.after(1000 * 30, losePresence,0);	
  }
}

void PresenceDetector::stopTimeout() {
  _enabled = false;
	_timer.stop(_timerId);
}

void PresenceDetector::losePresence(void *context) {
	
	_detected = false;
  if (_presenceLostCallbackFunc) _presenceLostCallbackFunc();
}

void PresenceDetector::tick(void){

  if (digitalRead(_sensorPin) == HIGH) {
	  
	  if ( !_detected ) {
	  	_detected = true;
	    if (_presenceDetectedCallbackFunc) _presenceDetectedCallbackFunc();
	  }
   
	  resetTimeout();
  }

  _timer.update();
}
