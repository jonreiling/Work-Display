//------------------------------------------------------------------------
// TagWatcher.cpp - Library for detecting changes in the ID-* line of RFID readers
//------------------------------------------------------------------------

#include "TagWatcher.h"

boolean TagWatcher::_running = true;


// ----- Initialization and Default Values -----

TagWatcher::TagWatcher(int resetPin){
 
  _resetPin = resetPin;
  pinMode(_resetPin, OUTPUT);
  digitalWrite(_resetPin, HIGH);

  Serial1.begin(9600);

  _previousContent = "";
  _tagChangeFunc = NULL;
  
} 

void TagWatcher::init() {

  Serial1.begin(9600);

  _previousContent = "";
  _tagChangeFunc = NULL;  
}


void TagWatcher::onTagChange(tagCallbackFunction tagChangeFunc){
  _tagChangeFunc = tagChangeFunc;
}

void TagWatcher::tick(void){

  if ( _running ) {
    
    checkTag();
  } 
  
  _timer.update();
}

void TagWatcher::checkTag(){
   String content = "";
  while(Serial1.available() > 0){
    
    char incomingByte = Serial1.read();
    //Serial.print(incomingByte);
    if ( incomingByte > 47) { 
      //Exclude everything before the numbers start. This will filter out irrelevant data coming from the reader.
      //See http://arduino.cc/en/Reference/ASCIIchart for reference.
      
      content.concat(incomingByte);
    }
  }  
  if ( content != _previousContent ) {
    //See if the value we've read this time is different.
    //If it is, that means there was a change.
    
    int contentLength = content.length();
    
    if ( contentLength == 12 ) {
      //Make sure we have a proper length tag. If so, broadcast it!
       if (_tagChangeFunc) _tagChangeFunc( content );
      
    } else if (contentLength == 0 ) {
      //If the length is different, that means there is no longer a tag present.
       if (_tagChangeFunc) _tagChangeFunc( "NONE" );
       
    } else {
      Serial.println(content);
       //Do nothing, as we don't have a proper tag value. 
    }

  }
  
  _previousContent = content;
  
  resetReader();
} // OneButton.tick()

void TagWatcher::resetReader(){
///////////////////////////////////
//Reset the RFID reader to read again.
///////////////////////////////////
  digitalWrite(_resetPin, LOW);
  digitalWrite(_resetPin, HIGH);
  
  _running = false;
  _timer.after(150,startReader,0);
//  delay(150);
//  startReader(0);
}

void TagWatcher::startReader(void *context) {
    _running = true;
}


// end.

