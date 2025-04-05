
#include <Arduino.h>
#include "samd_tc.h"


static void samd_tc_show(Tc *tc, int verbosity)
{
    if (tc->COUNT32.STATUS.bit.SLAVE == 1) {
        // slave in 32-bit mode
        Serial.println("  32-bit slave");
        return;
    }

    int mode = tc->COUNT8.CTRLA.bit.MODE;

    if (mode == TC_CTRLA_MODE_COUNT32_Val)
        Serial.println("  32-bit master");

    char buf[80];

    // CTRLA same in all modes
    TC_CTRLA_Type ctrla;
    ctrla.reg = tc->COUNT8.CTRLA.reg;
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

} // void samd_tc_show


void SamdTc3::show(int verbosity)
{
    Serial.print("TC3");
    samd_tc_show(TC3, verbosity);
}


void SamdTc4::show(int verbosity)
{
    Serial.print("TC4");
    samd_tc_show(TC4, verbosity);
}


void SamdTc5::show(int verbosity)
{
    Serial.print("TC5");
    samd_tc_show(TC5, verbosity);
}
