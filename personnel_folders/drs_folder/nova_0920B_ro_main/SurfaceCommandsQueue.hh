#ifndef SURFACE_COMMANDS_QUEUE
#define SURFACE_COMMANDS_QUEUE
#include "FunctionQueue.hh"
#include "surface_to_rocket_commands.hh"
class SurfaceCommandsQueue {

  private:
    FunctionQueue functions_queue;

  public:

    SurfaceCommandsQueue() {
        FunctionQueue functions_queue;
    }

    void accept_code_input(int code){
        /*
        Key to integer codes to trigger certain function calls:
        (0) -> Do nothing
        (1) -> Test Transmission
        */
       if(code == 0){
           return;
       }
       surface_to_rocket_commands::acknowledge_command_recieved(); //Just tells the ground reciever to stop transmitting the command
       if(code == 1){
           functions_queue.add_to_queue(&surface_to_rocket_commands::test_transmission);
       }
       else{
           //Send to log that an invalid code was recieved, log will be sent to ground
       }
    }
    
 
};

#endif