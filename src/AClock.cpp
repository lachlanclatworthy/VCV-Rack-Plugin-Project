// AClock.cpp

#include "plugin.hpp"		

struct AClock : Module {
    enum ParamIds {
        BPM_KNOB,
        NUM_PARAMS,
    };

    enum InputIds {
        NUM_INPUTS,
    };

    enum OutputIds {
        PULSE_OUT,
        NUM_OUTPUTS,
    };

    enum LightIds {
        PULSE_LIGHT,
        NUM_LIGHTS,
    };

    dsp::PulseGenerator pgen;
    float counter, period;

    AClock() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(BPM_KNOB, 30.0f, 360.0f, 120.0f, "Tempo", "BPM");
        counter = period = 0.f;
    }

    void process(const ProcessArgs& args) override; // called each sample
};

struct AClockWidget : ModuleWidget {

};

void AClock::process(const ProcessArgs& args) {

}

Model* modelAClock = createModel<AClock, AClockWidget>("AClock");