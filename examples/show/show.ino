
#include <Arduino.h>
#include "samd_dev.h"

// This runs on an Adafruit QT Py (SAMD21E18A)
//
// It shows, among other things:
//
// In GCLK, GEN0-GEN3 are set up, and GEN4-GEN8 are not.
// Only GEN0 and GEN1 appear to be used; GEN2 and GEN3 were
// probably use during initialization.
//
// In PM, most of the peripheral clocks have been enabled.
//
// None of the TCs are in used.


void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    Serial.println();
    SamdGclk::show();

    Serial.println();
    SamdPm::show();

    Serial.println();
    SamdTc3::show();
    SamdTc4::show();
    SamdTc5::show();

    Serial.println();
}


void loop()
{
}
