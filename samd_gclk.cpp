
#include <Arduino.h>
#include "samd_gclk.h"

// Note that "ID" in GENDIV and GENCTRL refer to one of the 9 "generic clock
// generators", and "ID" in CLKCTRL refers to one of the 38 "generic clocks".

// 9 generic clock generators: GENDIV[ID], GENCTRL[ID], CLKCTRL[GEN]
const char *SamdGclk::gen_name[SamdGclk::gen_id_max] = {
    "GEN0", "GEN1", "GEN2", "GEN3", "GEN4", "GEN5", "GEN6", "GEN7", "GEN8",
};

// 38 generic clocks: CLKCTRL[ID]
const char *SamdGclk::clk_name[SamdGclk::clk_id_max] = {
    "DFLL48M_REF", "DPLL", "DPLL_32K", "WDT", "RTC", "EIC", "USB",
    "EVSYS_CHANNEL_0", "EVSYS_CHANNEL_1", "EVSYS_CHANNEL_2",
    "EVSYS_CHANNEL_3", "EVSYS_CHANNEL_4", "EVSYS_CHANNEL_5",
    "EVSYS_CHANNEL_6", "EVSYS_CHANNEL_7", "EVSYS_CHANNEL_8",
    "EVSYS_CHANNEL_9", "EVSYS_CHANNEL_10", "EVSYS_CHANNEL_11",
    "SERCOMx_SLOW", "SERCOM0_CORE", "SERCOM1_CORE", "SERCOM2_CORE",
    "SERCOM3_CORE", "SERCOM4_CORE", "SERCOM5_CORE", "TCC0_TCC1", "TCC2_TC3",
    "TC4_TC5", "TC6_TC7", "ADC", "AC_AC1_DIG", "AC_AC1_ANA", "DAC", "PTC",
    "I2S_0", "I2S_1", "TCC3",
};

// 9 clock sources: GENCTRL[SRC]
const char *SamdGclk::src_name[SamdGclk::src_id_max] = {
    "XOSC", "GCLKIN", "GCLKGEN1", "OSCULP32K", "OSC32K", "XOSC32K", "OSC8M",
    "DFLL48M", "FDPLL96M",
};


// Read registers by writing an ID field to the low byte,
// then reading back.


GCLK_CLKCTRL_Type SamdGclk::get_clkctrl(int clk_id)
{
    // assert(clk_id < 64);
    *(volatile uint8_t *)(&GCLK->CLKCTRL.reg) = (uint8_t)(clk_id);
    sync();
    GCLK_CLKCTRL_Type clkctrl;
    clkctrl.reg = GCLK->CLKCTRL.reg;
    return clkctrl;
}


GCLK_GENCTRL_Type SamdGclk::get_genctrl(int gen_id)
{
    // assert(gen_id < 16);
    *(volatile uint8_t *)(&GCLK->GENCTRL.reg) = (uint8_t)(gen_id);
    sync();
    GCLK_GENCTRL_Type genctrl;
    genctrl.reg = GCLK->GENCTRL.reg;
    return genctrl;
}


GCLK_GENDIV_Type SamdGclk::get_gendiv(int gen_id)
{
    // assert(gen_id < 16);
    *(volatile uint8_t *)(&GCLK->GENDIV.reg) = (uint8_t)(gen_id);
    sync();
    GCLK_GENDIV_Type gendiv;
    gendiv.reg = GCLK->GENDIV.reg;
    return gendiv;
}


// verbosity:
// 0 - only print for enabled devices
// 1 - print for enabled devices, and print "disabled" for others
// 2 - decode for all devices
void SamdGclk::show(int verbosity)
{
    char buf[80];

    Serial.println("GCLK");

    if (verbosity >= 1) {

        GCLK_CTRL_Type ctrl;
        ctrl.reg = GCLK->CTRL.reg;

        sprintf(buf, "  CTRL    0x%04x: SWRST=%d", ctrl.reg, ctrl.bit.SWRST);
        Serial.println(buf);

        GCLK_STATUS_Type status;
        status.reg = GCLK->STATUS.reg;

        sprintf(buf, "  STATUS  0x%04x: SYNCBUSY=%d", status.reg, status.bit.SYNCBUSY);
        Serial.println(buf);

    } // if (verbosity...)

    // up to 16 generic clock generators

    Serial.println("  Generic Clock Generators");

    for (int gen_id = 0; gen_id < gen_id_max; gen_id++) {

        const GCLK_GENCTRL_Type genctrl = get_genctrl(gen_id);

        bool enabled = (genctrl.bit.GENEN != 0);

        // only print "disabled" if verbosity >= 1
        if (!enabled && (verbosity < 1))
            continue;

        sprintf(buf, "    %s: %s", gen_name[gen_id], enabled ? "enabled" : "disabled");
        Serial.println(buf);

        // only decode disabled devices if verbosity >= 2
        if (!enabled && (verbosity < 2))
            continue;

        const GCLK_GENDIV_Type gendiv = get_gendiv(gen_id);

        sprintf(buf, "      GENCTRL=0x%08x GENDIV=0x%08x", genctrl.reg, gendiv.reg);
        Serial.println(buf);

        if (genctrl.bit.SRC >= 0 && genctrl.bit.SRC < src_id_max)
            sprintf(buf, "      SRC=%s", src_name[genctrl.bit.SRC]);
        else
            sprintf(buf, "      SRC=<invalid>");
        Serial.print(buf);

        sprintf(buf, " DIV=0x%04x=%d RUNSTDBY=%d DIVSEL=%d OE=%d OOV=%d IDC=%d",
                gendiv.bit.DIV, gendiv.bit.DIV, genctrl.bit.RUNSTDBY,
                genctrl.bit.DIVSEL, genctrl.bit.OE, genctrl.bit.OOV,
                genctrl.bit.IDC);
        Serial.println(buf);

    } // for (int gen_id...)

    // up to 64 generic clocks

    Serial.println("  Generic Clocks");

    for (int clk_id = 0; clk_id < clk_id_max; clk_id++) {

        const GCLK_CLKCTRL_Type clkctrl = get_clkctrl(clk_id);

        bool enabled = (clkctrl.bit.CLKEN != 0);

        // only print "disabled" if verbosity >= 1
        if (!enabled && (verbosity < 1))
            continue;

        sprintf(buf, "    %s: %s", clk_name[clk_id], enabled ? "enabled" : "disabled");
        Serial.println(buf);

        // only decode disabled devices if verbosity >= 2
        if (!enabled && (verbosity < 2))
            continue;

        sprintf(buf, "      CLKCTRL=0x%04x", clkctrl.reg);
        Serial.println(buf);

        sprintf(buf, "      WRTLOCK=%d", clkctrl.bit.WRTLOCK);
        Serial.print(buf);

        if (clkctrl.bit.GEN >= 0 && clkctrl.bit.GEN < gen_id_max)
            sprintf(buf, " GEN=%s", gen_name[clkctrl.bit.GEN]);
        else
            sprintf(buf, " GEN=<invalid>");
        Serial.println(buf);

    } // for (int clk_id...)

} // void SamdGclk::show()


//////////////////////////////////////////////////////////////////////////////

void SamdGclk::gen_config(int gen_id, int src, int div)
{
    // assert(gen_id >= 0 && gen_id < 9);
    // assert(src >= 0 && src < 9);

    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(div)
                     | GCLK_GENDIV_ID(gen_id);

    GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN
                      | GCLK_GENCTRL_SRC(src)
                      | GCLK_GENCTRL_ID(gen_id);

    sync();
}


// set the generic clock generator to use as the source for a generic clock
// clk_id is one of the generic clocks (38 of them)
// gen_id is one of the generic clock generators (9 of them)
void SamdGclk::set_clk_src(int clk_id, int gen_id)
{
    // assert(clk_id >= 0 && clk_id < 38);
    // assert(gen_id >= 0 && gen_id < 9);
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN
                      | GCLK_CLKCTRL_GEN(gen_id)
                      | GCLK_CLKCTRL_ID(clk_id);
    sync();
}
