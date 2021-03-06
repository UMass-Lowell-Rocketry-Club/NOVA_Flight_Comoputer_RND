#ifndef SURFACE_TO_ROCKET_COMMANDS
#define SURFACE_TO_ROCKET_COMMANDS

namespace surface_to_rocket_commands{
    void test_transmission();
    void send_less_data();
    void acknowledge_command_recieved();
    bool check_if_code_is_valid(int code);
    int getCommandCodesFromReciever();
    std::vector<void (*)()> setup_vector_command_function_addresses();
}

#endif