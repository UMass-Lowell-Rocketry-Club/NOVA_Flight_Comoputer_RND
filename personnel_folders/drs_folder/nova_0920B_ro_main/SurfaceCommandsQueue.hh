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
        int index = 0;
        int bit = -1;
        for(int index = 0; index < num_of_command_functions; index++){
          bit = mask(code, index); //Essentially converts the integer we got into binary and returns the bit value at a specific index of the 'binary array' which constitutes that integer
          if(bit == 1){ //The code indicated that the function should be called
            functions_queue.add_to_queue(array_of_addresses_of_command_functions.at(index)); //Recall that index is the index of the relevant vector which stores the function address to execute it.
          }
        }
    }

    
 
};

#endif