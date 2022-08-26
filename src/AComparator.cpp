// AComparator.cpp

#include "plugin.hpp"		

/// <summary>
/// Takes two input voltages. Returns a high output voltage if Input 1 is higher than Input 2 -- 
/// High Voltage = 10V, Low Voltage = 0V
/// </summary>
struct AComparator : Module {
	enum ParamIds {
		NUM_PARAMS,
	};

	enum InputIds {
		INPUTA1,
		INPUTB1,
		INPUTA2,
		INPUTB2,
		NUM_INPUTS,
	};

	enum OutputIds {
		OUTPUT1,
		OUTPUT2,
		NUM_OUTPUTS,
	};

	enum LightsIds {
		LIGHT_1,
		LIGHT_2,
		NUM_LIGHTS,
	};

	AComparator() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	/// <summary>
	/// DSP Implementation. Operates sample by sample. Called periodically at sampling rate i.e. 44.1KHz
	/// </summary>
	/// <param name="args"></param>
	void process(const ProcessArgs& args) override;
};

struct AComparatorWidget : ModuleWidget {
	AComparatorWidget(AComparator* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AComparator.svg")));

		addInput(createInput<PJ301MPort>(Vec(50, 78), module, AComparator::INPUTA1));

		addInput(createInput<PJ301MPort>(Vec(50, 108), module, AComparator::INPUTB1));

		addOutput(createOutput<PJ3410Port>(Vec(46, 138), module, AComparator::OUTPUT1));

		addChild(createLight<TinyLight<GreenLight>>(Vec(80, 150), module, AComparator::LIGHT_1));
	}
};

void AComparator::process(const ProcessArgs& args) {
	if (inputs[INPUTA1].isConnected() && inputs[INPUTB1].isConnected()) {
		float out = inputs[INPUTA1].getVoltage() >= input[INPUTB1].getVoltage();
		outputs[OUTPUT1].setVoltage(10.f * out);
		lights[OUTPUT1].setBrightness(out);
	}
}