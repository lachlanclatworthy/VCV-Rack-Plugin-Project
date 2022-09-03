// ARandom.cpp

#include "plugin.hpp"

struct ARandom : Module {
    enum ParamIds {
        PARAM_HOLD,
        PARAM_DISTRIB,
        NUM_PARAMS,
    };

    enum InputIds {
        NUM_INPUTS,
    };

    enum OutputIds {
        RANDOM_OUT,
        NUM_OUTPUTS,
    };

    enum LightIds {
        NUM_LIGHTS,
    };

    enum { DISTR_UNIFORM = 0, DISTR_NORMAL = 1, NUM_DISTRIBUTIONS };

    int counter;
    float value;

    ARandom() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(PARAM_HOLD, 0, 1, 1, "Hold time", " s");
        configParam(PARAM_DISTRIB, 0.0, 1.0, 0.0, "Distribution");
        counter = 0;
        value = 0.f;
    }

    void process(const ProcessArgs& args) override;
};

void ARandom::process(const ProcessArgs& args) {
    int hold = std::floor(params[PARAM_HOLD].getValue() *
                          args.sampleRate);  // how long do we hold?
    int distr = std::round(
        params[PARAM_DISTRIB].getValue());  // which distribution do we use?

    // Have we reached hold threshold
    if (counter >= hold) {
        // Sample new value from distribution
        if (distr == DISTR_UNIFORM) {
            value = 10.f * random::uniform();  // UNIFORM DISTRIBUTION
        } else {
            value = clamp(5.f * random::normal(), -10.f,
                          10.f);  // NORMAL DISTRIBUTION
        }
        counter = 0;
    }

    counter++;
    outputs[RANDOM_OUT].setVoltage(value);
}

struct ARandomWidget : ModuleWidget {
    ARandomWidget(ARandom* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ARandom.svg")));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(20,20)), module, ARandom::RANDOM_OUT));
        
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(20,40)), module, ARandom::PARAM_HOLD));
        addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(20,60)), module, ARandom::PARAM_DISTRIB));
    }
};

Model* modelARandom = createModel<ARandom, ARandomWidget>("ARandom");