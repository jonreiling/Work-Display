//------------------------------------------------------------------------
// TagWatcher.cpp - Library for detecting changes in the ID-* line of RFID readers
//------------------------------------------------------------------------

#include "StripManager.h"

//StripManager StripManager::_instance;

Adafruit_DotStar StripManager::_strip = Adafruit_DotStar(37, 5, 4, DOTSTAR_BRG);
uint32_t StripManager::_topColor;
uint32_t StripManager::_bottomColor;
int StripManager::_currentPixel;
int StripManager::_previousPixel;
// ----- Initialization and Default Values -----

StripManager::StripManager(){

//  _strip = Adafruit_DotStar(37, 4, 5, DOTSTAR_BRG);
  _topColor = 0x000000;
  _bottomColor = 0x000000;
  _currentPixel = 0;
  _previousPixel = 0;
  _strip.begin(); // Initialize pins for output
  _strip.show();
  
  _timer.every(15,updatePixels,0);

  _tweener.multiplier = .1;
} 

void StripManager::setToPresent() {
    _topColor = 0xFFFFFF;
    _bottomColor = 0x000000;
    _tweener.targetValue = 255;
}

void StripManager::setToOn() {
    _topColor = 0xFFFFFF;
    _bottomColor = 0xFFFFFF;
    _tweener.targetValue = 255;


}



void StripManager::setToOff() {

//    _topColor = 0x000000;
  //  _bottomColor = 0x000000;

    _tweener.targetValue = 0;
}

void StripManager::updatePixels(void *context) {
    
    _strip.setPixelColor(_previousPixel,_bottomColor); // 'Off' pixel at head

      _previousPixel = _currentPixel;
      _currentPixel ++;
      if ( _currentPixel == 37 ) {
        _currentPixel = 2;  
      }

    _strip.setPixelColor(_currentPixel,_topColor); // 'On' pixel at head


}

void StripManager::tick(void){

  _timer.update();
  _tweener.tick();
  _strip.setBrightness(_tweener.value);
    _strip.show();

}
