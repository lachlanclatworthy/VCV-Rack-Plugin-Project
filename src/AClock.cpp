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
        configParam(BPM_KNOB, 30.0f, 360.0f, 120.0f, "Tempo", " BPM");
        counter = period = 0.f;
    }

    void process(const ProcessArgs& args) override;  // called each sample
};

void AClock::process(const ProcessArgs& args) {
    float BPM = params[BPM_KNOB].getValue();
    period = 60.f * args.sampleRate / BPM;  // samples

    if (counter >= period) {
        pgen.trigger();
        counter -= period;  // keep the fractional part
    }

    counter++;
    float out = pgen.process(args.sampleTime);
    outputs[PULSE_OUT].setVoltage(10.f * out);
    lights[PULSE_LIGHT].setBrightnessSmooth(
        out, 5e-6f);  // smooth light brightness to handle triggers between GUI
                      // updates
}

struct AClockWidget : ModuleWidget {
    AClockWidget(AClock* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(
            asset::plugin(pluginInstance, "res/AClock.svg")));

        addParam(createParamCentered<RoundBigBlackKnob>(mm2px(Vec(32.5, 30)), module, AClock::BPM_KNOB));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.5, 50)), module, AClock::PULSE_OUT));

        addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(22.5, 50)), module, AClock::PULSE_LIGHT));
    }
};

Model* modelAClock = createModel<AClock, AClockWidget>("AClock");