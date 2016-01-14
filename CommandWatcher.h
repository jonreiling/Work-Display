// -----
// CommandWatcher.h - Library for recieving commands from another serial source.
// -----
#ifndef CommandWatcher_h
#define CommandWatcher_h

#include "Arduino.h"

// ----- Callback function types -----

extern "C" {
  typedef void (*commandCallbackFunction)(String tag);
}


class CommandWatcher
{
public:

  CommandWatcher();
  void onCommand(commandCallbackFunction newFunction);
  void tick(void);
  void init2(void);

private:

  String _commandParts;
  boolean _reading;
  commandCallbackFunction _commandCallback;
  void evaluateCommand(String command);
};

#endif


