#include <HMC5883_DTWI.h>

uint8_t HMC5883::readRegister(uint8_t reg) {
    uint8_t state = 0;
    uint8_t val = 0;
    while (1) {
        switch (state) {
            case 0: // begin write 
                if (dtwi->startMasterWrite(0x1E)) {
                    state = 1;
                }
                break;
            case 1: // Send register address
                if (dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }
                break;
            case 2: // Stop Master
                if (dtwi->stopMaster()) {
            state = 3;
                }
                break;
            case 3:
                if (dtwi->startMasterRead(0x1E, 1)) {
                    state = 4;
                }
                break;
            case 4:
                if (dtwi->available()) {
                    dtwi->read(&val, 1);
                    state = 5;
                }
                break;
            case 5:
                if (dtwi->stopMaster()) {
                    return val;
                }
                break;
        }
    }
}

void HMC5883::writeRegister(uint8_t reg, uint8_t val) {
    uint8_t state = 0;
    while (1) {
        switch (state) {
            case 0: // begin write 
                if (dtwi->startMasterWrite(0x1E)) {
                    state = 1;
                }
                break;
            case 1: // Send register address
                if (dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }
                break;
            case 2: // Send register address
                if (dtwi->write(&val, 1) == 1) {
                    state = 3;
                }
                break;
            case 3: // Stop Master
                if (dtwi->stopMaster()) {
                    return;
                }
                break;
        }
    }
}

int16_t HMC5883::readRaw(uint8_t h, uint8_t l) {
    uint8_t msb = readRegister(h);
    uint8_t lsb = readRegister(l);
    return (msb << 8) | lsb;
}

void HMC5883::begin() {
    dtwi->beginMaster();
    writeRegister(0, 0b01111000);
    writeRegister(1, 0b00100000);
    writeRegister(2, 0b00000000);
}

int16_t HMC5883::readRawX() { return readRaw(3, 4); }
int16_t HMC5883::readRawY() { return readRaw(7, 8); }
int16_t HMC5883::readRawZ() { return readRaw(5, 6); }

float HMC5883::readX() { return readRaw(3, 4) * gain; }
float HMC5883::readY() { return readRaw(7, 8) * gain; }
float HMC5883::readZ() { return readRaw(5, 6) * gain; }

float HMC5883::readHeading() {
    readZ(); // Need this to get a new reading
    float heading = atan2(readY(), readX());
    heading += declination;
    if (heading < 0) heading += 2*M_PI;
    if (heading > 2*PI) heading -= 2*M_PI;
    return heading * 180/M_PI;
}

float HMC5883::readHeadingCompensated(float ax, float ay, float az) {
    float cx = readX();
    float cy = readY();
    float cz = readZ();
    float xh=cx*cos(ay)+cy*sin(ay)*sin(ax)-cz*cos(ax)*sin(ay);
    float yh=cy*cos(ax)+cz*sin(ax);
    float heading = atan2(yh, xh);
    heading += declination;
    if (heading < 0) heading += 2*M_PI;
    if (heading > 2*PI) heading -= 2*M_PI;
    return heading * 180/M_PI;
}

void HMC5883::setGain(uint8_t g) {
    switch (g & 0x7) {
        case GAIN_1370:
            writeRegister(1, 0b00000000);
            gain = 0.73;
            break;
        case GAIN_1090:
            writeRegister(1, 0b00100000);
            gain = 0.92;
            break;
        case GAIN_820:
            writeRegister(1, 0b01000000);
            gain = 1.22;
            break;
        case GAIN_660:
            writeRegister(1, 0b01100000);
            gain = 1.52;
            break;
        case GAIN_440:
            writeRegister(1, 0b10000000);
            gain = 2.27;
            break;
        case GAIN_390:
            writeRegister(1, 0b10100000);
            gain = 2.56;
            break;
        case GAIN_330:
            writeRegister(1, 0b11000000);
            gain = 3.03;
            break;
        case GAIN_230:
            writeRegister(1, 0b11100000);
            gain = 4.35;
            break;
    }
}

void HMC5883::setDeclination(float d) {
    declination = d;
}
