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
        RESET_BUTTON,
        NUM_PARAMS,
    };

    enum InputIds {
        MAIN_IN,
        RESET_GATE,
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
    // dsp::SchmittTrigger resetGate; // I don't think I really understand how this was supposed to work, yet
    int stepNr = 0;

    ASequencer() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        for (int i = 0; i < ASequencer::NUM_LIGHTS; i++) {
            configParam(PARAM_STEP_1 + i, 0.0, 5.0, 1.0);
        }
    }

    void process(const ProcessArgs& args) override;
};

void ASequencer::process(const ProcessArgs& args) {
    // Reset on gate or button input
    if (inputs[RESET_GATE].getVoltage() == 10.f || params[RESET_BUTTON].getValue()) {
        stepNr = 0;
    }

    // Step forward on main input, wrapping round after 8 steps
    if (edgeDetector.process(inputs[MAIN_IN].getVoltage())) {
        stepNr = (stepNr + 1) % 8;  // avoids modulus operator
    }

    // Update the lights
    for (int i = 0; i < NUM_LIGHTS; i++) {
        lights[i].setBrightnessSmooth(i == stepNr, 5e-6f);
    }

    // Update the output
    outputs[MAIN_OUT].setVoltage(params[stepNr].getValue());
}

struct ASequencerWidget : ModuleWidget {
    ASequencerWidget(ASequencer* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(
            asset::plugin(pluginInstance, "res/ASequencer.svg")));

        // Input
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5,20)), module, ASequencer::MAIN_IN));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.5,30)), module, ASequencer::RESET_GATE));

        // Output
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(45,20)), module, ASequencer::MAIN_OUT));

        // Parameters
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,30)), module, ASequencer::PARAM_STEP_1));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,40)), module, ASequencer::PARAM_STEP_2));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,50)), module, ASequencer::PARAM_STEP_3));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,60)), module, ASequencer::PARAM_STEP_4));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,70)), module, ASequencer::PARAM_STEP_5));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,80)), module, ASequencer::PARAM_STEP_6));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,90)), module, ASequencer::PARAM_STEP_7));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30,100)), module, ASequencer::PARAM_STEP_8));
        addParam(createParamCentered<VCVButton>(mm2px(Vec(12.5, 40)), module, ASequencer::RESET_BUTTON));
        
        // Lights
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 30)), module, ASequencer::LIGHT_STEP_1));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 40)), module, ASequencer::LIGHT_STEP_2));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 50)), module, ASequencer::LIGHT_STEP_3));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 60)), module, ASequencer::LIGHT_STEP_4));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 70)), module, ASequencer::LIGHT_STEP_5));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 80)), module, ASequencer::LIGHT_STEP_6));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 90)), module, ASequencer::LIGHT_STEP_7));
        addChild(createLightCentered<MediumLight<YellowLight>>(
            mm2px(Vec(40, 100)), module, ASequencer::LIGHT_STEP_8));
    }
};

Model* modelASequencer = createModel<ASequencer, ASequencerWidget>("ASequencer");
