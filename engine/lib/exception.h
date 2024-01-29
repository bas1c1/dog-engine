#ifndef DOG_ENGINE_EXCEPTIONS
#define DOG_ENGINE_EXCEPTIONS

#include <iostream>

class DogEngineRuntimeError : public std::exception {
private:
    char* msg;
public:
    DogEngineRuntimeError(char* msg) : msg(msg) {}

    char* what() {
        return msg;
    }
};

#endif