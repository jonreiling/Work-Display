#include "TagWatcher.h"
#include "CommandWatcher.h"
#include "Timer.h"
#include "OneButton.h"
#include "CapButton.h"
#include "PresenceDetector.h"
#include "StripManager.h"
#include <Tweener.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

Adafruit_DotStar button_strip = Adafruit_DotStar( 1, 7, 8, DOTSTAR_BRG);

PresenceDetector detector(3);
TagWatcher tagWatcher(2);
CommandWatcher commandWatcher;
CapButton button = CapButton();
Tweener tweener2;
StripManager stripManager;
Timer keepAliveTimer;

boolean buttonEnabled = false;

void setup(){
  
  Serial.begin(9600);

  button.connect();
  
  pinMode(13, OUTPUT);
  digitalWrite(13,HIGH);
  
  button_strip.begin();
  button_strip.setPixelColor(0,0xFFFFFF);
  button_strip.show();

  tagWatcher.init();
  tagWatcher.onTagChange(handleTagChange);
  commandWatcher.onCommand(handleCommand);

  button.setClickTicks(350);
  button.attachClick(handleButtonClick);
  button.attachPress(handleButtonPress);
  button.attachRelease(handleButtonRelease);
  button.attachDoubleClick(handleButtonDoubleClick);

  detector.onPresenceDetected(handlePresenceDetected);
  detector.onPresenceLost(handlePresenceLost);  

  stripManager = StripManager();
//     stripManager.setToPresent();

  tweener2.multiplier = .1;

  //keepAliveTimer.every(2000,keepAlive,0);
}

void loop(){
  
  tagWatcher.tick();
  commandWatcher.tick();
  tweener2.tick();
  detector.tick();
  stripManager.tick();
  keepAliveTimer.update();

  
  if (buttonEnabled) button.tick();


   
  button_strip.setBrightness(tweener2.value);
  button_strip.show();
}

void keepAlive(void *context) {
  Serial.print("(k,tick)");
  
}


void handleTagChange( String tag ) {
  
  if ( tag == "NONE" ) {
     detector.enableTimeout();
     stripManager.setToPresent();
  } else {
     detector.stopTimeout();
     stripManager.setToOn();
  }
  //String jsonOutput = "{\"e\":\"t\",\"v\":\"" + tag + "\"}";
//  Serial.println( jsonOutput ); 
  Serial.print("(t,"+tag+")");

}


void handleCommand( String command ) {
  
  if ( command == "eb" ) {
    enableButton();
  } else if ( command == "db" ) {
    disableButton();
  }
  
}

//------------------------------------------------------------------------
// Presence functions
//------------------------------------------------------------------------

//TO DO: Write precense functions

void handlePresenceDetected() {

//    String jsonOutput = "{\"e\":\"p\",\"v\":\"t\"}";
//    Serial.println( jsonOutput ); 
    Serial.print("(p,t)");


     stripManager.setToPresent();
}

void handlePresenceLost() {

//    String jsonOutput = "{\"e\":\"p\",\"v\":\"f\"}";
//    Serial.println( jsonOutput ); 

    Serial.print("(p,f)");

    stripManager.setToOff();

}

//------------------------------------------------------------------------
// Button functions
//------------------------------------------------------------------------

void handleButtonClick() {
  
//  String jsonOutput = "{\"e\":\"b\",\"v\":\"click\"}";
//  Serial.println( jsonOutput ); 
   Serial.print("(b,c)");
  
}

void handleButtonDoubleClick() {

//  String jsonOutput = "{\"e\":\"b\",\"v\":\"double\"}";
//  Serial.println( jsonOutput ); 
   Serial.print("(b,dc)");
}

void handleButtonPress() {
    tweener2.targetValue = 60;
    tweener2.value = 60;
}

void handleButtonRelease() {
  tweener2.targetValue = 255;  
}

void enableButton() {
  tweener2.targetValue = 255;
  tweener2.value = 255;
  buttonEnabled = true;
}

void disableButton() {
  tweener2.targetValue = 0;
  buttonEnabled = false;
}

