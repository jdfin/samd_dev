
#include <Arduino.h>
#include "samd_tc.h"


void SamdTc::show(int verbosity)
{
    Serial.print("TC");
    Serial.println(_tc_num);

    if (_tc->COUNT32.STATUS.bit.SLAVE == 1) {
        // slave in 32-bit mode
        Serial.println("  32-bit slave");
        return;
    }

    int mode = _tc->COUNT8.CTRLA.bit.MODE;

    if (mode == TC_CTRLA_MODE_COUNT32_Val)
        Serial.println("  32-bit master");

    char buf[80];

    // CTRLA same in all modes
    TC_CTRLA_Type ctrla;
    ctrla.reg = _tc->COUNT8.CTRLA.reg;
    sprintf(buf, "  CTRLA    0x%04x: ", ctrla.reg);
    Serial.println(buf);

    sprintf(buf, "    PRESCSYNC=%d RUNSTDBY=%d PRESCALER=%d ",
            ctrla.bit.PRESCSYNC, ctrla.bit.RUNSTDBY, ctrla.bit.PRESCALER);
    Serial.print(buf);

    sprintf(buf, "WAVEGEN=%d MODE=%d ENABLE=%d SWRST=%d",
            ctrla.bit.WAVEGEN, ctrla.bit.MODE, ctrla.bit.ENABLE, ctrla.bit.SWRST);
    Serial.println(buf);

    if (mode == TC_CTRLA_MODE_COUNT8_Val) {
        // CTRLA already printed
        // READREQ
        // CTRLBCLR
        // CTRLBSET
        // CTRLC
        // DBGCTRL
        // EVTCTRL
        // INTENCLR
        // INTENSET
        // INTFLAG
        // STATUS
        // COUNT
        // PER
    } else if (mode == TC_CTRLA_MODE_COUNT16_Val) {
        // CTRLA already printed
    } else if (mode == TC_CTRLA_MODE_COUNT32_Val) {
        // CTRLA already printed
    } else {
        Serial.print("  invalid mode: ");
        Serial.println(mode);
        return;
    }

} // void SamdTc::show


void SamdTc::reset()
{
    _tc->COUNT8.CTRLA.bit.SWRST = 1;
    sync();
    while (_tc->COUNT8.CTRLA.bit.SWRST != 0)
        ;
}
