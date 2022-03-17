#include "macros.hh"
#include "Vector3D.hh"
#include "binary_code.hh"

using namespace custom_binary;

int mask(int value, int index) {
    if(index >= I32_SIZE || index < 0){
        return nullptr;
    }
    return 0 != (value & (1 << index));
}
