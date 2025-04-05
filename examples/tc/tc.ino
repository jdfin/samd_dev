#include <Arduino.h>
#include "samd_dev.h"

// This initializes a free-running 32 bit counter at 1 MHz and shows that it
// is counting at the correct rate (compared to the system clock).
//
// This is a much lower-overhead way of getting micros().


static SamdTc4 tc4;
static SamdTc5 tc5;


void setup()
{

    Serial.begin(115200);
    while (!Serial)
        ;

    // set up GEN4 with DFLL48M as source, divide by 48 (1 MHz)
    SamdGclk::gen_config(4, GCLK_GENCTRL_SRC_DFLL48M_Val, 48);

    // connect GEN4 (1 MHz) to TC4/TC5
    SamdGclk::set_clk_src(GCLK_CLKCTRL_ID_TC4_TC5_Val, GCLK_CLKCTRL_GEN_GCLK4_Val);

    // enable TC4 and TC5 on the peripheral bus
    // (probably already done by bootloader)
    SamdPm::tc4_enable();
    SamdPm::tc5_enable();

    // set up TC4/TC5 as free-running 32 bit counter
    tc4.reset();
    tc4.set_ctrla(TC_CTRLA_PRESCSYNC_PRESC |
                  TC_CTRLA_PRESCALER_DIV1 |
                  TC_CTRLA_WAVEGEN_NFRQ |
                  TC_CTRLA_MODE_COUNT32 |
                  TC_CTRLA_ENABLE);

    Serial.println();
    SamdGclk::show();

    Serial.println();
    tc4.show();
    tc5.show();

    Serial.println();

} // void setup()


void loop()
{
    const uint32_t interval_ms = 1000;
    static uint32_t next_ms = millis() + 10; // start a few ms from now

    int32_t wait_ms = next_ms - millis();

    if (wait_ms > 0)
        return;

    Serial.println(tc4.count32());

    next_ms += interval_ms;

} // void loop()
