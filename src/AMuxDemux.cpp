#include "plugin.hpp"


struct AMuxDemux : Module {
	enum ParamId {
		M_SELECTOR_PARAM,
		D_SELECTOR_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		M_INPUT_1,
		M_INPUT_2,
		M_INPUT_3,
		M_INPUT_4,
		D_MAIN_IN,
		INPUTS_LEN,
		N_MUX_IN = M_INPUT_4,
	};
	enum OutputId {
		M_MAIN_OUT,
		D_OUTPUT_1,
		D_OUTPUT_2,
		D_OUTPUT_3,
		D_OUTPUT_4,
		OUTPUTS_LEN,
		N_DEMUX_OUT = D_OUTPUT_4,
	};
	enum LightId {
		M_LIGHT_1,
		M_LIGHT_2,
		M_LIGHT_3,
		M_LIGHT_4,
		D_LIGHT_1,
		D_LIGHT_2,
		D_LIGHT_3,
		D_LIGHT_4,
		LIGHTS_LEN
	};

	AMuxDemux() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(M_SELECTOR_PARAM, 0.f, 3.0f, 0.f, "Mux Selector");
		configParam(D_SELECTOR_PARAM, 0.f, 3.0f, 0.f, "Demux Selector");
		configInput(M_INPUT_1, "");
		configInput(M_INPUT_2, "");
		configInput(M_INPUT_3, "");
		configInput(M_INPUT_4, "");
		configInput(D_MAIN_IN, "");
		configOutput(M_MAIN_OUT, "");
		configOutput(D_OUTPUT_1, "");
		configOutput(D_OUTPUT_2, "");
		configOutput(D_OUTPUT_3, "");
		configOutput(D_OUTPUT_4, "");

		selMux = selDemux = 0;
	}

	void process(const ProcessArgs& args) override {
		// MUX
		// Update the lights so that the one corresponding
		// with the selected input is lit 
		lights[selMux].setBrightness(0.f);
		selMux = (unsigned int)clamp((int)params[M_SELECTOR_PARAM].getValue(), 0, N_MUX_IN);
		lights[selMux].setBrightness(1.f);

		if (outputs[M_MAIN_OUT].isConnected()) {
			if (inputs[selMux].isConnected()) {
				// Send selected input's voltage to the Mux output
				outputs[M_MAIN_OUT].setVoltage(inputs[selMux].getVoltage());
			}
		}

		// TODO: DEMUX Implementation
	}

	unsigned int selMux, selDemux;	// selector values
};


struct AMuxDemuxWidget : ModuleWidget {
	AMuxDemuxWidget(AMuxDemux* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/AMuxDemux.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(-568.123, 30.676)), module, AMuxDemux::M_SELECTOR_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(-688.111, 247.548)), module, AMuxDemux::D_SELECTOR_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-697.054, 20.988)), module, AMuxDemux::M_INPUT_1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-696.309, 58.251)), module, AMuxDemux::M_INPUT_2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-694.818, 99.24)), module, AMuxDemux::M_INPUT_3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-693.328, 141.72)), module, AMuxDemux::M_INPUT_4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(-688.111, 295.245)), module, AMuxDemux::INPUTB1_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-567.378, 94.769)), module, AMuxDemux::M_MAIN_OUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-566.633, 220.719)), module, AMuxDemux::D_OUTPUT_1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-566.633, 259.472)), module, AMuxDemux::D_OUTPUT_2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-566.633, 295.245)), module, AMuxDemux::D_OUTPUT_3));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(-565.887, 333.254)), module, AMuxDemux::D_OUTPUT_4));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-661.363, 17.925)), module, AMuxDemux::M_LIGHT_1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-662.853, 60.405)), module, AMuxDemux::M_LIGHT_2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-661.363, 98.414)), module, AMuxDemux::M_LIGHT_3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-660.617, 141.639)), module, AMuxDemux::M_LIGHT_4));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-596.525, 221.382)), module, AMuxDemux::D_LIGHT_1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-595.034, 261.627)), module, AMuxDemux::D_LIGHT_2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-595.034, 298.89)), module, AMuxDemux::D_LIGHT_3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(-595.779, 334.663)), module, AMuxDemux::D_LIGHT_4));
	}
};


Model* modelAMuxDemux = createModel<AMuxDemux, AMuxDemuxWidget>("AMuxDemux");