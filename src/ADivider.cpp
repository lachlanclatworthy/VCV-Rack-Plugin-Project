// ADivider.cpp

#include "plugin.hpp"

struct div2 {

};

struct ADivider : Module {
    enum ParamIds {
        NUM_PARAMS,
    };

    enum InputIds {
        NUM_INPUTS,
    };

    enum OutputIds {
        NUM_OUTPUTS,
    };

    enum LightIds {
        NUM_LIGHTS,
    };

    ADivider() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    }

    void process(const ProcessArgs& args) override;
};

void ADivider::process(const ProcessArgs& args) {

}

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
