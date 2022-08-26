// AComparator.cpp

#include "plugin.hpp"		

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

	void process(const ProcessArgs& args) override;
};