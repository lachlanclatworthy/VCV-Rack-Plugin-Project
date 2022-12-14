// ADivider.cpp

#include "plugin.hpp"

const float TRIG_TIME = 0.5f;

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

    /// @brief Iteratively divides clock pulse by 2 for each output
    /// @param idx
    void iterActiv(int idx) {
        if (idx > NUM_OUTPUTS - 1) return;  // stop iteration

        bool activation = dividers[idx].process();

        pgen[idx].trigger(TRIG_TIME);

        if (activation) {
            iterActiv(idx + 1);
        }
    }

    dsp::SchmittTrigger
        edgeDetector;  // activates on a clock edge, stays high until it reaches
                       // a low threshold (i.e. 0)

    ADivider() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }

    void process(const ProcessArgs& args) override;
};

void ADivider::process(const ProcessArgs& args) {
    if (edgeDetector.process(inputs[MAIN_IN].getVoltage())) {
        iterActiv(0);  // run the first divider (/2) and iterate if necessary
    }

    for (int i = 0; i < NUM_OUTPUTS; i++) {
        float out = pgen[i].process(args.sampleTime);
        outputs[i].setVoltage(10.f * out);
        lights[i].setBrightnessSmooth(out, 0.5f);
    }
}

struct ADividerWidget : ModuleWidget {
    int outputY = 50;
    int outputYOffset = 15;

    ADividerWidget(ADivider* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(
            asset::plugin(pluginInstance, "res/ADivider.svg")));

        // Input
        addInput(createInput<PJ301MPort>(mm2px(Vec(10,20)),module,ADivider::MAIN_IN));

        // Output
        addOutput(createOutput<PJ301MPort>(mm2px(Vec(20,outputY)),module,ADivider::OUTPUT2));
        addOutput(createOutput<PJ301MPort>(mm2px(Vec(20,outputY + outputYOffset)),module,ADivider::OUTPUT4));
        addOutput(createOutput<PJ301MPort>(mm2px(Vec(20,outputY + outputYOffset * 2)),module,ADivider::OUTPUT8));
        addOutput(createOutput<PJ301MPort>(mm2px(Vec(20,outputY + outputYOffset * 3)),module,ADivider::OUTPUT16));
        addOutput(createOutput<PJ301MPort>(mm2px(Vec(20,outputY + outputYOffset * 4)),module,ADivider::OUTPUT32));

        // Lights
        addChild(createLight<MediumLight<YellowLight>>(mm2px(Vec(5, outputY)), module, ADivider::LIGHT2));
        addChild(createLight<MediumLight<YellowLight>>(mm2px(Vec(5, outputY + outputYOffset)), module, ADivider::LIGHT4));
        addChild(createLight<MediumLight<YellowLight>>(mm2px(Vec(5, outputY + outputYOffset * 2)), module, ADivider::LIGHT8));
        addChild(createLight<MediumLight<YellowLight>>(mm2px(Vec(5, outputY + outputYOffset * 3)), module, ADivider::LIGHT16));
        addChild(createLight<MediumLight<YellowLight>>(mm2px(Vec(5, outputY + outputYOffset * 4)), module, ADivider::LIGHT32));
    }
};

Model* modelADivider = createModel<ADivider, ADividerWidget>("ADivider");