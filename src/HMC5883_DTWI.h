#ifndef _HMC5883_DTWI_H
#define _HMC5883_DTWI_H

#include <Arduino.h>
#include <DTWI.h>

class HMC5883 {
    private:
        DTWI *dtwi;

        uint8_t readRegister(uint8_t reg);
        void writeRegister(uint8_t reg, uint8_t val);
        int16_t readRaw(uint8_t h, uint8_t l);

        float gain;
        float declination;

    public:
        static const uint8_t GAIN_1370 = 0b000;
        static const uint8_t GAIN_1090 = 0b001;
        static const uint8_t GAIN_820  = 0b010;
        static const uint8_t GAIN_660  = 0b011;
        static const uint8_t GAIN_440  = 0b100;
        static const uint8_t GAIN_390  = 0b101;
        static const uint8_t GAIN_330  = 0b110;
        static const uint8_t GAIN_230  = 0b111;

        HMC5883(DTWI *d) : dtwi(d), gain(0.92), declination(0) {}
        HMC5883(DTWI &d) : dtwi(&d), gain(0.92), declination(0) {}
        
        void begin();
        int16_t readRawX();
        int16_t readRawY();
        int16_t readRawZ();
        float readX();
        float readY();
        float readZ();
        float readHeading();
        float readHeadingCompensated(float ax, float ay, float az);
        void setGain(uint8_t g);
        void setDeclination(float d);
};

#endif
