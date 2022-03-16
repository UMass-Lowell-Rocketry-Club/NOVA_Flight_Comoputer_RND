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
        (1)  -> Test Transmission
        (2)  -> Reserved for ground functions
        (3)  -> Reserved for ground functions
        (4)  -> Reserved for ground functions
        (5)  -> Reserved for ground functions
        (6)  -> Reserved for ground functions
        (7)  -> Reserved for ground functions
        (8)  -> Reserved for ground functions
        (9)  -> Reserved for ground functions
        (10) -> Order Tier 1 Data Packets
        (11) -> Order Tier 2 Data Packets
        */


        //this may be quicker if done via accessing an array in memory where each code is an index and the array stores the address of the function.
        //consider adding this to setup?
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