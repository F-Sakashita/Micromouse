#ifndef IMU_HPP
#define IMU_HPP

#include "Calculation.h"
#include "ICM_20602.hpp"


class IMU{
    public:
        static IMU& GetInstance(){
            static IMU self;
            return self;
        }

        bool Initialize(uint32_t uiSamplingTimeMs, bool bEnableGyro, bool bEnableAccel);
       	void EnableGyroOffestCalc(uint32_t uiSamplingNum, bool bCalcX=true, bool bCalcY=true, bool bCalcZ=true);
    	bool IsGyroOffsetCompleted();
        void Update();
        const Coord_t& GetGyroDPS();
        const Coord_t& GetAccelG();
        const Coord_t& GetGyroRPS();
    	const Coord_t& GetGyroDeg();

    private:
        IMU();
        ~IMU(){;}
        IMU(const IMU &other);

        void CalcGyroOffset();
        void CalcGyroDeg();

        bool bInitialized;

        Coord_t stRawGyroDPS;	//unit:[deg/s]
        Coord_t stRawAccelG;	//unit:[m/s^2]
        Coord_t stGyroDPS;		//unit:[deg/s]
        Coord_t stGyroRPS;	    //unit:[rad/s]
        Coord_t stAccelG;

        uint32_t uiSamplingTimeMs;
       	bool bGyroOffsetCalcStart;
        bool bGyroOffsetCalcStartFlag[3];
        uint32_t uiGyroOffsetSamplingNum;
        uint32_t uiGyroOffsetSamplingCount;
        bool bGyroOffsetCalcCompleted[3];

        Coord_t stGyroDPSOffsetValue;	//unit:[deg/s]
        Coord_t stGyroDeg;			//unit:[deg]

        static ICM_20602 Icm20602;

};

#endif