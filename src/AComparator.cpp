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

		// SEC. 1

		addInput(createInput<PJ301MPort>(Vec(50, 78), module, AComparator::INPUTA1));

		addInput(createInput<PJ301MPort>(Vec(50, 108), module, AComparator::INPUTB1));

		addOutput(createOutput<PJ3410Port>(Vec(46, 138), module, AComparator::OUTPUT1));

		addChild(createLight<TinyLight<GreenLight>>(Vec(80, 150), module, AComparator::LIGHT_1));

		// SEC. 2

		addInput(createInput<PJ301MPort>(Vec(50, mm2px(80.373f)), module, AComparator::INPUTA2));

		addInput(createInput<PJ301MPort>(Vec(50, mm2px(91.662f)), module, AComparator::INPUTB2));

		addOutput(createOutput<PJ3410Port>(Vec(46, mm2px(102.951f)), module, AComparator::OUTPUT2));

		addChild(createLight<TinyLight<GreenLight>>(Vec(80, mm2px(105.746f)), module, AComparator::LIGHT_2));
	}
};

void AComparator::process(const ProcessArgs& args) {
	float out;
	int outputIndex = 0;
	float threshold = 0.1f;
	float voltageA;
	float voltageB;

	// For each dual set of inputs
	for (int i = 0; i < NUM_INPUTS; i += 2) {
		if (inputs[i].isConnected() && inputs[i + 1].isConnected()) {

			voltageA = inputs[i].getVoltage();
			voltageB = inputs[i + 1].getVoltage();

			out = voltageA >= voltageB ? 1.f : 0.f;

			// Only update output and light if threshold is surpassed
			if (abs(voltageA - voltageB) >= threshold) {
				outputs[outputIndex].setVoltage(10.f * out);
				lights[outputIndex].setBrightness(out);
			}
			outputIndex++;
		}
	}
}

Model* modelAComparator = createModel<AComparator, AComparatorWidget>("AComparator");