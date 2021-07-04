/*           
 * Title        :   14. Command pattern
 * Filename     :   main.cc
 * Author       :   jaej.dev@gmail.com
 * Description  :
 *
 * Reference 11. GOF 23 Design Patterns
 * C++11 
 */

#ifdef __cplusplus
#endif

#include "Command.h"

using namespace std;

/*
 * main entry
 * client
 */ 

int main(void)
{
  cout << "\nmain: Create receiver ----------------------------------" << endl;
  Light *light = new Light;
  Fan *fan = new Fan;

  cout << "\nmain: Create concrete command object -------------------" << endl;
  LightOnCommand *lightOn = new LightOnCommand(light);
  LightOffCommand *lightOff = new LightOffCommand(light);
  FanOnCommand *fanOn = new FanOnCommand(fan);
  FanOffCommand *fanOff = new FanOffCommand(fan);
  NullCommand *nullOn = new NullCommand();
  NullCommand *nullOff = new NullCommand();

  cout << "\nmain: Create invoker object ----------------------------" << endl;
  RemoteControl *control = new RemoteControl;

  cout << "\nmain: Execute ------------------------------------------" << endl;
  control->SetCommand(LIGHT, lightOn, lightOff);
  control->OnButtonPressed(LIGHT);
  control->OffButtonPressed(LIGHT);

  control->SetCommand(FAN, fanOn, fanOff);
  control->OnButtonPressed(FAN);
  control->OffButtonPressed(FAN);

  control->SetCommand(NONE, nullOn, nullOff);
  control->OnButtonPressed(NONE);
  control->OffButtonPressed(NONE);

  cout << "\nmain: Program exit -------------------------------------" << endl;
  delete control;
  delete nullOff;
  delete nullOn;
  delete fanOff;
  delete fanOn;
  delete lightOff;
  delete lightOn;
  delete fan;
  delete light;

  return 0;
}
