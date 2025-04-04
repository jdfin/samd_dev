
#include <Arduino.h>
#include "samd_pm.h"

// Partial implementation

void SamdPm::show(int verbosity)
{
    char buf[80];

    Serial.println("PM");

    // CTRL
    // SLEEP
    // CPUSEL
    // APBASEL
    // APBBSEL
    // APBCSEL

    // AHBMASK
    PM_AHBMASK_Type ahbmask;
    ahbmask.reg = PM->AHBMASK.reg;
    sprintf(buf, "  AHBMASK  0x%08x: ", ahbmask.reg);
    Serial.println(buf);
    sprintf(buf, "    USB=%d DMAC=%d NVMCTRL=%d DSU=%d HPB2=%d HPB1=%d HPB0=%d",
            ahbmask.bit.USB_, ahbmask.bit.DMAC_, ahbmask.bit.NVMCTRL_,
            ahbmask.bit.DSU_, ahbmask.bit.HPB2_, ahbmask.bit.HPB1_,
            ahbmask.bit.HPB0_);
    Serial.println(buf);

    // APBAMASK
    PM_APBAMASK_Type apbamask;
    apbamask.reg = PM->APBAMASK.reg;
    sprintf(buf, "  APBAMASK 0x%08x: ", apbamask.reg);
    Serial.println(buf);
    sprintf(buf, "    EIC=%d RTC=%d WDT=%d GCLK=%d SYSCTRL=%d PM=%d PAC0=%d",
            apbamask.bit.EIC_, apbamask.bit.RTC_, apbamask.bit.WDT_,
            apbamask.bit.GCLK_, apbamask.bit.SYSCTRL_, apbamask.bit.PM_,
            apbamask.bit.PAC0_);
    Serial.println(buf);

    // APBBMASK
    PM_APBBMASK_Type apbbmask;
    apbbmask.reg = PM->APBBMASK.reg;
    sprintf(buf, "  APBBMASK 0x%08x: ", apbbmask.reg);
    Serial.println(buf);
    sprintf(buf, "    HMATRIX=%d USB=%d DMAC=%d PORT=%d NVMCTRL=%d DSU=%d PAC1=%d",
            apbbmask.bit.HMATRIX_, apbbmask.bit.USB_, apbbmask.bit.DMAC_,
            apbbmask.bit.PORT_, apbbmask.bit.NVMCTRL_, apbbmask.bit.DSU_,
            apbbmask.bit.PAC1_);
    Serial.println(buf);

    // APBCMASK
    PM_APBCMASK_Type apbcmask;
    apbcmask.reg = PM->APBCMASK.reg;
    sprintf(buf, "  APBCMASK 0x%08x: ", apbcmask.reg);
    Serial.println(buf);
    sprintf(buf, "    I2S=%d PTC=%d DAC=%d AC=%d ADC=%d",
            apbcmask.bit.I2S_, apbcmask.bit.PTC_, apbcmask.bit.DAC_,
            apbcmask.bit.AC_, apbcmask.bit.ADC_);
    Serial.println(buf);
    sprintf(buf, "    TC7=%d TC6=%d TC5=%d TC4=%d TC3=%d TCC2=%d TCC1=%d TCC0=%d",
            apbcmask.bit.TC7_, apbcmask.bit.TC6_, apbcmask.bit.TC5_,
            apbcmask.bit.TC4_, apbcmask.bit.TC3_, apbcmask.bit.TCC2_,
            apbcmask.bit.TCC1_, apbcmask.bit.TCC0_);
    Serial.println(buf);
    sprintf(buf, "    SERCOM5=%d SERCOM4=%d SERCOM3=%d SERCOM2=%d SERCOM1=%d SERCOM0=%d",
            apbcmask.bit.SERCOM5_, apbcmask.bit.SERCOM4_, apbcmask.bit.SERCOM3_,
            apbcmask.bit.SERCOM2_, apbcmask.bit.SERCOM1_, apbcmask.bit.SERCOM0_);
    Serial.println(buf);
    sprintf(buf, "    EVSYS=%d PAC2=%d",
            apbcmask.bit.EVSYS_, apbcmask.bit.PAC2_);
    Serial.println(buf);

    // INTENCLR
    // INTENSET
    // INTFLAG
    // RCAUSE

} // void SamdPm::show
