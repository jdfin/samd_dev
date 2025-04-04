#pragma once

#include <Arduino.h>

class SamdPm {

    public:

        static void show(int verbosity=0);

        static bool tcc0_get_enable() { return PM->APBCMASK.bit.TCC0_ != 0; }
        static void tcc0_enable()     { PM->APBCMASK.bit.TCC0_ = 1; }
        static void tcc0_disable()    { PM->APBCMASK.bit.TCC0_ = 0; }

        static bool tcc1_get_enable() { return PM->APBCMASK.bit.TCC1_ != 0; }
        static void tcc1_enable()     { PM->APBCMASK.bit.TCC1_ = 1; }
        static void tcc1_disable()    { PM->APBCMASK.bit.TCC1_ = 0; }

        static bool tcc2_get_enable() { return PM->APBCMASK.bit.TCC2_ != 0; }
        static void tcc2_enable()     { PM->APBCMASK.bit.TCC2_ = 1; }
        static void tcc2_disable()    { PM->APBCMASK.bit.TCC2_ = 0; }

        static bool tc3_get_enable()  { return PM->APBCMASK.bit.TC3_ != 0; }
        static void tc3_enable()      { PM->APBCMASK.bit.TC3_ = 1; }
        static void tc3_disable()     { PM->APBCMASK.bit.TC3_ = 0; }

        static bool tc4_get_enable()  { return PM->APBCMASK.bit.TC4_ != 0; }
        static void tc4_enable()      { PM->APBCMASK.bit.TC4_ = 1; }
        static void tc4_disable()     { PM->APBCMASK.bit.TC4_ = 0; }

        static bool tc5_get_enable()  { return PM->APBCMASK.bit.TC5_ != 0; }
        static void tc5_enable()      { PM->APBCMASK.bit.TC5_ = 1; }
        static void tc5_disable()     { PM->APBCMASK.bit.TC5_ = 0; }

        static bool tc6_get_enable()  { return PM->APBCMASK.bit.TC6_ != 0; }
        static void tc6_enable()      { PM->APBCMASK.bit.TC6_ = 1; }
        static void tc6_disable()     { PM->APBCMASK.bit.TC6_ = 0; }

        static bool tc7_get_enable()  { return PM->APBCMASK.bit.TC7_ != 0; }
        static void tc7_enable()      { PM->APBCMASK.bit.TC7_ = 1; }
        static void tc7_disable()     { PM->APBCMASK.bit.TC7_ = 0; }

}; // class SamdPm
