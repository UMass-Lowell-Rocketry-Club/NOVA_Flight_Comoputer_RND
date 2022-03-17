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
      if(*funcptr != nullptr){ //Some functions may be undefined, as a consequence they are currently set as nullptr, and we need to be sure that none have that issue before trying to run them.
        queue.push_back(funcptr);
      }
    }

    void service_queue() {
      void (*fPtr)();
      fPtr = queue.front();
      fPtr(); //Calls the function pointer
    }
};

#endif