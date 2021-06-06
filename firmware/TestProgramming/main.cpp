#include <iostream>
#include <cstdint>
#include "Calculation.h"

using namespace std;

int main()
{
    cout << "Hello World" << endl;

    float fTest[] = {
        0.0f,
        90.0f,
        -90.0f,
        180.0f,
        -180.0f,
        270.0f,
        -270.0f,
        360.0f,
        -360.0f,
        600.0f,
        -600.0f,
        623.425f,
        -401.564f        
    };
    cout << "0 to 360 deg limited" << endl;
    for (uint8_t ucCount = 0; ucCount < sizeof(fTest)/sizeof(float); ucCount ++){
        cout << "deg:" << fTest[ucCount] << "\t" << Calc_ConvDegRange0To360(fTest[ucCount]) << endl;
    }
 
    cout << "--------------------------------" << endl;
    cout << "-180 to 180 deg limited" << endl;
    for (uint8_t ucCount = 0; ucCount < sizeof(fTest)/sizeof(float); ucCount ++){
        cout << "deg:" << fTest[ucCount] << "\t" << Calc_ConvDegRangeM180ToP180(fTest[ucCount]) << endl;
    }

    return 0;
}