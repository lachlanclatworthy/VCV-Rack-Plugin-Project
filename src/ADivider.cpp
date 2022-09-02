// ADivider.cpp

#include "plugin.hpp"

struct div2 {
    bool status;

    div2() { status = false; }

    bool process() {
        status ^= 1;
        return status;
    }
};

struct ADivider : Module {
    enum ParamIds {
        NUM_PARAMS,
    };

    enum InputIds {
        MAIN_IN,
        NUM_INPUTS,
    };

    enum OutputIds {
        OUTPUT1,
        OUTPUT2,
        OUTPUT4,
        OUTPUT8,
        OUTPUT16,
        OUTPUT32,
        NUM_OUTPUTS,
    };

    enum LightIds {
        LIGHT1,
        LIGHT2,
        LIGHT4,
        LIGHT8,
        LIGHT16,
        LIGHT32,
        NUM_LIGHTS,
    };

    div2 dividers[NUM_OUTPUTS];
    dsp::PulseGenerator pgen[NUM_OUTPUTS];

    ADivider() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }

    void process(const ProcessArgs& args) override;
};

void ADivider::process(const ProcessArgs& args) {}

struct ADividerWidget : ModuleWidget {
    ADividerWidget(ADivider* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(
            asset::plugin(pluginInstance, "res/ADivider.svg")));

        // Input

        // Output

        // Parameters

        // Lights
    }
};

Model* modelADivider = createModel<ADivider, ADividerWidget>("ADivider");
