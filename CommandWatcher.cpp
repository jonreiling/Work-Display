// -----
// CommandWatcher.cpp - Library for recieving commands from another serial source.

// -----

#include "CommandWatcher.h"

// ----- Initialization and Default Values -----

CommandWatcher::CommandWatcher(){
  Serial.begin(115200);
  _commandParts = "";
  _reading = false;
} 


void CommandWatcher::onCommand(commandCallbackFunction newFunction){
  _commandCallback = newFunction;
}

void CommandWatcher::tick(void){
  
  while(Serial.available() > 0){

    char incomingByte = Serial.read();
  
    if ( incomingByte == '(' ) _reading = true;
    if ( incomingByte > 33) _commandParts.concat( incomingByte );

    if ( incomingByte == ')' ){
      _reading = false;
      evaluateCommand( _commandParts );
      _commandParts = "";
    }
  }
}

void CommandWatcher::evaluateCommand(String command) {
  
  Serial.println( command );
  if ( command.length() != 0 ) {
    if ( command.length() < 3 ) {
      
      //Probably just noise from the serial connection, so do nothing.
      
    } else if ( command.substring(0,3) != "(c," ) {
      
//        String jsonOutput = "{\"event\":\"error\",\"value\":\"Invalid command\"}";
  //      Serial.println( jsonOutput );       
      
    } else {
      
      command = command.substring(3);
      command = command.substring(0,command.length()-1);
      if ( _commandCallback ) _commandCallback( command );
    }
  }  
  
  
}
