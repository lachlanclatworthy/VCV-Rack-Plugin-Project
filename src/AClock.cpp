// AClock.cpp

#include "plugin.hpp"

struct AClock : Module {
    enum ParamIds {
        BPM_KNOB,
        BARS_SWITCH,
        DURATION_SWITCH,
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
        configParam(BARS_SWITCH, 0.f, 1.f, 0.f, "Beats or Bars");
        configParam(DURATION_SWITCH, 0.f, 2.f, 1.f, "Note Length: Half - Quarter - Eigth");
        counter = period = 0.f;
    }

    void process(const ProcessArgs& args) override;  // called each sample
};

void AClock::process(const ProcessArgs& args) {
    float BPM = params[BPM_KNOB].getValue();
    int noteDuration = (int)params[DURATION_SWITCH].getValue();
    
    if (noteDuration == 0) {
        BPM = BPM * 0.5f;    // Half Notes
    } else {
        BPM = BPM * noteDuration;   // Quarter and Eighth Notes
    }

    period = 60.f * args.sampleRate / BPM;  // samples

    // Is the period each beat or each bar
    if (params[BARS_SWITCH].getValue()) {
        period = period * 4;    // We're only handling 4/4 currently
    }

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

        addParam(createParamCentered<RoundBigBlackKnob>(
            mm2px(Vec(22.5, 30)), module, AClock::BPM_KNOB));
        addParam(createParamCentered<BefacoSwitch>(mm2px(Vec(42.5, 20)), module,
                                        AClock::BARS_SWITCH));
        addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(42.5, 35)), module,
                                        AClock::DURATION_SWITCH));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.5, 50)), module,
                                                   AClock::PULSE_OUT));

        addChild(createLightCentered<MediumLight<RedLight>>(
            mm2px(Vec(22.5, 50)), module, AClock::PULSE_LIGHT));
    }
};

Model* modelAClock = createModel<AClock, AClockWidget>("AClock");