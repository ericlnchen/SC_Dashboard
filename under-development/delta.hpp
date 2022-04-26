#ifndef delta
#define delta

#include <fstream>

//  using heuristics to give priority and uniform all return vals
struct heur{
    unsigned char impact_value;
    unsigned char real_value;
    float probablity;
};

//  the delta namespace, technically an inhouse API to call when the team needs predictions
namespace delta{

    float predicted_lapTime(float speed, float length);
    heur predicted_tyreWear();
    heur predicted_temperature();
    heur predicted_wind();
}


#endif