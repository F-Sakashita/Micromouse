#ifndef TRAPEZOIDAL_VELOCITY_HPP
#define TRAPEZOIDAL_VELOCITY_HPP

class TrapezoidVel{
    public:
        typedef enum{
            EN_VEL_NORMAL = 0,
            EN_VEL_SCURVE,
        }EN_VEL_MODE;

        typedef enum{
            EN_ARRIVAL_MODE_NORMAL = 0,
            EN_ARRIVAL_MODE_CREEP,
        }EN_ARRIVAL_MODE;

        TrapezoidVel(EN_VEL_MODE enVelMode, float fTargetPos, float fTargetMaxVel, float fTargetAcc, float fTargetDec);
        ~TrapezoidVel();


        float GetTargetVel();
        bool IsArrival(float fAccuracy);
        void AddTargetPos();

    private:
};

#endif