#ifndef FUNCTION_QUEUE
#define FUNCTION_QUEUE
#include"macros.hh"
class FunctionQueue {

  private:
    std::vector<void (*)()> queue; //Pointer to a void of type void with no parameters

  public:

    FunctionQueue() {
      std::vector<void (*)()> queue;
    }
    


    void add_to_queue(void (*funcptr)()) {
      queue.push_back(funcptr);
    }

    void service_queue() {
      void (*fPtr)();
      fPtr = queue.front();
      fPtr(); //Calls the function pointer
    }
};

#endif