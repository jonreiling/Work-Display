
#include "TagWatcher.h"
#include "CommandWatcher.h"
#include "Timer.h"
#include "OneButton.h"
#include "PresenceDetector.h"
#include "StripManager.h"
#include <Tweener.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET

Adafruit_DotStar button_strip = Adafruit_DotStar( 1, 7, 8, DOTSTAR_BRG);

PresenceDetector detector(3);
TagWatcher tagWatcher(2);
CommandWatcher commandWatcher;
OneButton button(6, true);
Tweener tweener2;
StripManager stripManager;

boolean buttonEnabled = false;

void setup(){
  
  Serial.begin(115200);
  Serial.print("BEGIN");

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
  button.attachDoubleClick(handleButtonDoubleClick);

  detector.onPresenceDetected(handlePresenceDetected);
  detector.onPresenceLost(handlePresenceLost);  

  stripManager = StripManager();
//     stripManager.setToPresent();

  tweener2.multiplier = .1;


}

void loop(){
  
  tagWatcher.tick();
  commandWatcher.tick();
  tweener2.tick();
  detector.tick();
  stripManager.tick();
  
  if ( buttonEnabled ) button.tick();


   
  button_strip.setBrightness(tweener2.value);
  button_strip.show();
   
}



void handleTagChange( String tag ) {
  
  if ( tag == "NONE" ) {
     detector.resetTimeout();
     stripManager.setToPresent();
  } else {
     detector.stopTimeout();
     stripManager.setToOn();
  }
  String jsonOutput = "{\"e\":\"t\",\"v\":\"" + tag + "\"}";
  
  Serial.println( jsonOutput ); 
}


void handleCommand( String command ) {
  
  if ( command == "enableButton" ) {
    enableButton();
  } else if ( command == "disableButton" ) {
    disableButton();
  }
  
}

//------------------------------------------------------------------------
// Presence functions
//------------------------------------------------------------------------

//TO DO: Write precense functions

void handlePresenceDetected() {

    String jsonOutput = "{\"e\":\"p\",\"v\":\"t\"}";
    Serial.println( jsonOutput ); 

     stripManager.setToPresent();
}

void handlePresenceLost() {

    String jsonOutput = "{\"e\":\"p\",\"v\":\"f\"}";

    Serial.println( jsonOutput ); 
    stripManager.setToOff();

}

//------------------------------------------------------------------------
// Button functions
//------------------------------------------------------------------------

void handleButtonClick() {
  
  String jsonOutput = "{\"event\":\"b\",\"v\":\"click\"}";
  Serial.println( jsonOutput ); 
  
  
}

void handleButtonDoubleClick() {

  String jsonOutput = "{\"event\":\"b\",\"v\":\"double\"}";
  Serial.println( jsonOutput ); 
}

void enableButton() {
  tweener2.targetValue = 255;
  buttonEnabled = true;
}

void disableButton() {
  tweener2.targetValue = 0;
  buttonEnabled = false;
}

