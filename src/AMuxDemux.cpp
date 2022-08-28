#include "plugin.hpp"


struct AMuxDemux : Module {
	enum ParamId {
		PARAMA1_PARAM,
		PARAMB1_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUTA1_INPUT,
		INPUTA2_INPUT,
		INPUTA3_INPUT,
		INPUTA4_INPUT,
		INPUTB1_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUTA1_OUTPUT,
		OUTPUTB1_OUTPUT,
		OUTPUTB2_OUTPUT,
		OUTPUTB3_OUTPUT,
		OUTPUTB4_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTA1_LIGHT,
		LIGHTA2_LIGHT,
		LIGHTA3_LIGHT,
		LIGHTA4_LIGHT,
		LIGHTB1_LIGHT,
		LIGHTB2_LIGHT,
		LIGHTB3_LIGHT,
		LIGHTB4_LIGHT,
		LIGHTS_LEN
	};

	AMuxDemux() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(PARAMA1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(PARAMB1_PARAM, 0.f, 1.f, 0.f, "");
		configInput(INPUTA1_INPUT, "");
		configInput(INPUTA2_INPUT, "");
		configInput(INPUTA3_INPUT, "");
		configInput(INPUTA4_INPUT, "");
		configInput(INPUTB1_INPUT, "");
		configOutput(OUTPUTA1_OUTPUT, "");
		configOutput(OUTPUTB1_OUTPUT, "");
		configOutput(OUTPUTB2_OUTPUT, "");
		configOutput(OUTPUTB3_OUTPUT, "");
		configOutput(OUTPUTB4_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct AMuxDemuxWidget : ModuleWidget {
	AMuxDemuxWidget(AMuxDemux* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/AMuxDemux.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(-568.123, 30.676)), module, AMuxDemux::PARAMA1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(-688.111, 247.548)), module, AMuxDemux::PARAMB1_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-697.054, 20.988)), module, AMuxDemux::INPUTA1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-696.309, 58.251)), module, AMuxDemux::INPUTA2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-694.818, 99.24)), module, AMuxDemux::INPUTA3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-693.328, 141.72)), module, AMuxDemux::INPUTA4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-688.111, 295.245)), module, AMuxDemux::INPUTB1_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-567.378, 94.769)), module, AMuxDemux::OUTPUTA1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-566.633, 220.719)), module, AMuxDemux::OUTPUTB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-566.633, 259.472)), module, AMuxDemux::OUTPUTB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-566.633, 295.245)), module, AMuxDemux::OUTPUTB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-565.887, 333.254)), module, AMuxDemux::OUTPUTB4_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-661.363, 17.925)), module, AMuxDemux::LIGHTA1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-662.853, 60.405)), module, AMuxDemux::LIGHTA2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-661.363, 98.414)), module, AMuxDemux::LIGHTA3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-660.617, 141.639)), module, AMuxDemux::LIGHTA4_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-596.525, 221.382)), module, AMuxDemux::LIGHTB1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-595.034, 261.627)), module, AMuxDemux::LIGHTB2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-595.034, 298.89)), module, AMuxDemux::LIGHTB3_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-595.779, 334.663)), module, AMuxDemux::LIGHTB4_LIGHT));
	}
};


Model* modelAMuxDemux = createModel<AMuxDemux, AMuxDemuxWidget>("AMuxDemux");