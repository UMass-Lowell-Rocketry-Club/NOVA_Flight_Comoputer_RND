#ifndef SURFACE_COMMANDS_QUEUE
#define SURFACE_COMMANDS_QUEUE
#include "FunctionQueue.hh"
#include "surface_to_rocket_commands.hh"
#include "macros.hh"

class SurfaceCommandsQueue {

  private:
    FunctionQueue functions_queue;

  public:

    SurfaceCommandsQueue() {
        FunctionQueue functions_queue;
    }

    void accept_code_input(int code){
        surface_to_rocket_commands::acknowledge_command_recieved(); //Just tells the ground reciever that the command was recieved.
        functions_queue.add_to_queue(array_of_addresses_of_command_functions.at(code)); //Recall that code is the index of the relevant vector which stores the function address to execute it.
    }
    
 
};

#endif