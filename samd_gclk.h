#pragma once

#include <Arduino.h>


class SamdGclk {

    public:

        SamdGclk();

        ~SamdGclk();

        static void show(int verbosity=0);

        static void gen_config(int gen_id, int src, int div);

        static void set_clk_src(int clk_id, int gen_id);

    private:

        // GEN0 ... GEN8
        static const int gen_id_max = 9;

        static const char *gen_name[gen_id_max];

        static const int clk_id_max = 38;

        static const char *clk_name[clk_id_max];

        static const int src_id_max = 9;

        static const char *src_name[src_id_max];

        static void sync()
        {
            while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY != 0)
                ;
        }

        static GCLK_CLKCTRL_Type get_clkctrl(int clk_id);

        static GCLK_GENCTRL_Type get_genctrl(int gen_id);

        static GCLK_GENDIV_Type get_gendiv(int gen_id);

};
