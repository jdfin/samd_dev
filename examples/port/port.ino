
#include <Arduino.h>
#include "samd_dev.h"

// This shows how to use the SamdPort class for GPIOs.
//
// digitalWrite() is measured to take about 1.5 usec per call, while
// using SamdPort is about 150 nsec per call (10 times as fast).

// pin to pulse
static const int pin = 0;

static SamdPort gpio(pin, true, false, false, false);

int d = 1;


void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    // set up GEN4 with DFLL48M as source, divide by 1 (48 MHz)
    SamdGclk::gen_config(4, GCLK_GENCTRL_SRC_DFLL48M_Val, 1);

    // connect GEN4 (48 MHz) to TC4/TC5
    SamdGclk::set_clk_src(GCLK_CLKCTRL_ID_TC4_TC5_Val, GCLK_CLKCTRL_GEN_GCLK4_Val);

    // enable TC4 and TC5 on the peripheral bus (probably done by bootloader)
    SamdPm::tc4_enable();
    SamdPm::tc5_enable();

    SamdTc4::reset();

    // free-running 32 bit counter (setup is rather gritty, sorry)
    SamdTc4::set_ctrla(TC_CTRLA_PRESCSYNC_PRESC |
                       TC_CTRLA_PRESCALER_DIV1 |
                       TC_CTRLA_WAVEGEN_NFRQ |
                       TC_CTRLA_MODE_COUNT32 |
                       TC_CTRLA_ENABLE);

    gpio.clear();

} // void setup()


void loop()
{
    uint32_t t1_ck, t2_ck;

    // Timings were measured on an Adafruit Qt Py, 48 MHz SAMD21E18A.

    Serial.println();

    // Sanity-check tick counter rate ////////////////////////////////////////

    delay(2);
    t1_ck = SamdTc4::count32();
    delay(100);
    t2_ck = SamdTc4::count32();

    Serial.print("100 msec: ");
    Serial.print(t2_ck - t1_ck);
    Serial.println(" ticks");

    // Tick counter appears to be going at 48 MHz as expected.

    // Just reading the tick counter /////////////////////////////////////////

    noInterrupts();
    t1_ck = SamdTc4::count32();
    t2_ck = SamdTc4::count32();
    interrupts();

    Serial.print("nop: ");
    Serial.print(t2_ck - t1_ck);
    Serial.println(" ticks");

    // 14 ticks. This is the "exit" from the first read and "entry" of the
    // second read, so it takes ~14 ticks (292 nsec) to read the counter.
    // It varies a bit depending on whether the count can stay in a register
    // or must be put on the stack.

    delay(100);

    // Using digitalWrite() //////////////////////////////////////////////////

    noInterrupts();
    t1_ck = SamdTc4::count32();
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
    t2_ck = SamdTc4::count32();
    interrupts();

    Serial.print("digitalWrite: ");
    Serial.print(t2_ck - t1_ck);
    Serial.println(" ticks");

    // 157 ticks. Subtracting the nop time means 143 ticks to call it twice,
    // or 71.5 ticks (1.5 usec) per call.
    //
    // On a scope, the pulse is 1.5 usec wide, agreeing with the 1.5 usec
    // per call estimate.

    delay(100);

    // Using gpio.write() ////////////////////////////////////////////////////

    noInterrupts();
    t1_ck = SamdTc4::count32();
    gpio.write(d);
    gpio.write(1 - d);
    t2_ck = SamdTc4::count32();
    interrupts();

    Serial.print("gpio.write: ");
    Serial.print(t2_ck - t1_ck);
    Serial.println(" ticks");

    // 47 ticks. Subtracting the nop time means 33 ticks to call it twice,
    // or 16.5 ticks (344 nsec) per call.
    //
    // On a scope, the pulse is ~310 nsec wide, more-or-less agreeing with
    // the estimate.

    delay(100);

    // gpio.set/clear ////////////////////////////////////////////////////////

    noInterrupts();
    t1_ck = SamdTc4::count32();
    gpio.set();
    gpio.clear();
    t2_ck = SamdTc4::count32();
    interrupts();

    Serial.print("gpio.set/clear: ");
    Serial.print(t2_ck - t1_ck);
    Serial.println(" ticks");

    // 32 ticks.
    //
    // Subtracting the nop time means 18 ticks to call set then clear,
    // or 9 ticks (188 nsec) per call.
    //
    // On a scope, the pulse is ~144 nsec wide, more-or-less agreeing with
    // the estimate.

    delay(100);

    // Assembly //////////////////////////////////////////////////////////////

    // This ends up generating this assembly:
    //
    // ldr      r2, [r6, #8]        volatile uint32_t *set = gpio.set_ptr();
    // ldr      r1, [r6, #12]       volatile uint32_t *clear = gpio.clear_ptr();
    // ldr      r3, [r6, #4]        uint32_t mask = gpio.mask();
    // cpsid    i                   noInterrupts()
    // ldr      r6, [r5, #16]       t1_ck = SamdTc4::count32();
    // str      r3, [r2, #0]        set
    // str      r3, [r1, #0]        clear
    // ldr      r5, [r5, #16]       t1_ck = SamdTc4::count32();
    // cpsie    i                   interrupts()

    volatile uint32_t *set = gpio.set_ptr();
    volatile uint32_t *clear = gpio.clear_ptr();
    uint32_t mask = gpio.mask();

    noInterrupts();
    t1_ck = SamdTc4::count32();
    __asm__ volatile (
        "  str    %[msk], [%[set], #0];"
        "  str    %[msk], [%[clr], #0];"
        : // outputs
        : // inputs
        [msk] "l" (mask),
        [set] "l" (set),
        [clr] "l" (clear)
    );
    t2_ck = SamdTc4::count32();
    interrupts();

    Serial.print("asm: ");
    Serial.print(t2_ck - t1_ck);
    Serial.println(" ticks");

    // 23 ticks.
    //
    // Disable NVMCTRL cache to get slower, but more deterministic, control
    // over timing.
    //
    // On a scope, this pulse appears to be 4 clocks wide (83.3ns).

    delay(700);

} // void loop()
