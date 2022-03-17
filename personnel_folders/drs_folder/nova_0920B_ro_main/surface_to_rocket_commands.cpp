#include "macros.hh"
#include "Vector3D.hh"
#include "SurfaceCommandsQueue.hh"
#include "surface_to_rocket_commands.hh"


using namespace surface_to_rocket_commands;
void test_transmission(){

}

void send_less_data(){
    commanded_to_send_less_data = true;
}

void acknowledge_command_recieved(){
    //Implement code to send down a message that the rocket recieved a command
}

std::vector<void (*)()> setup_vector_command_function_addresses(){
    std::vector<void (*)()> command_function_addresses;
    int i = 0;
    while(i<num_of_command_functions){
        if(i == 0){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 1){
            command_function_addresses.push_back(&surface_to_rocket_commands::test_transmission);
        }
        else if(i == 2){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 3){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 4){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 5){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 6){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 7){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 8){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 9){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 10){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 11){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 12){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 13){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 15){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 16){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 17){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 18){
            command_function_addresses.push_back(nullptr);
        }
        else if(i == 19){
            command_function_addresses.push_back(nullptr);
        }
        else{
           //Nothing
        }
        i++;
    }
    return command_function_addresses;
    
}

