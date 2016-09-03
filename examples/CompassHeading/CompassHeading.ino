#include <DTWI.h>
#include <HMC5883_DTWI.h>

DTWI1 internalWire;
HMC5883 compass(internalWire);

void setup() {
    Serial.begin(115200);
    compass.begin();
}

void loop() {
    float headingDegrees = compass.readHeadingCompensated(0, 0, 0);
    Serial.printf("%8.2f\r\n", headingDegrees);

    delay(100);
}
