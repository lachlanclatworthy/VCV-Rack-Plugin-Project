// ASequencer.cpp

#include "plugin.hpp"

struct ASequencer : Module {
    enum ParamIds {
        PARAM_STEP_1,
        PARAM_STEP_2,
        PARAM_STEP_3,
        PARAM_STEP_4,
        PARAM_STEP_5,
        PARAM_STEP_6,
        PARAM_STEP_7,
        PARAM_STEP_8,
        NUM_PARAMS,
    };

    enum InputIds {
        MAIN_IN,
        NUM_INPUTS,
    };

    enum OutputIds {
        MAIN_OUT,
        NUM_OUTPUTS,
    };

    enum LightIds {
        LIGHT_STEP_1,
        LIGHT_STEP_2,
        LIGHT_STEP_3,
        LIGHT_STEP_4,
        LIGHT_STEP_5,
        LIGHT_STEP_6,
        LIGHT_STEP_7,
        LIGHT_STEP_8,
        NUM_LIGHTS,
    };

    dsp::SchmittTrigger edgeDetector;
    int stepNr = 0;

    ASequencer() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        for (int i = 0; i < ASequencer::NUM_LIGHTS; i++) {
            configParam(PARAM_STEP_1 + i, 0.0, 5.0, 1.0);
        }
    }

    void process(const ProcessArgs &args) override;
};

void ASequencer::process(const ProcessArgs &args) {
    if (edgeDetector.process(inputs[MAIN_IN].getVoltage())) {
        stepNr = stepNr % 8;    // avoids modulus operator
    }

    for (int i = 0; i < NUM_LIGHTS; i++) {
        lights[i].setBrightnessSmooth(i == stepNr, 5e-6f);
    }

    outputs[MAIN_OUT].setVoltage(params[stepNr].getValue());
}


