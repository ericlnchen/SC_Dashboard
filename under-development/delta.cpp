
#include "delta.hpp"

namespace delta{

    float predict_laptime(float speed, float length){
        return length/speed;
    }
}