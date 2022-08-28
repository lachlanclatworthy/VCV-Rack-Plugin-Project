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
		D_OUTPUT_1,
		D_OUTPUT_2,
		D_OUTPUT_3,
		D_OUTPUT_4,
		M_MAIN_OUT,
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
		configInput(M_INPUT_1, "MUX IN 1");
		configInput(M_INPUT_2, "MUX IN 2");
		configInput(M_INPUT_3, "MUX IN 3");
		configInput(M_INPUT_4, "MUX IN 4");
		configInput(D_MAIN_IN, "DEMUX IN");
		configOutput(M_MAIN_OUT, "MUX OUT");
		configOutput(D_OUTPUT_1, "DEMUX OUT 1");
		configOutput(D_OUTPUT_2, "DEMUX OUT 2");
		configOutput(D_OUTPUT_3, "DEMUX OUT 3");
		configOutput(D_OUTPUT_4, "DEMUX OUT 4");

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
				// Send the voltage of the Mux's selected input, to its output
				outputs[M_MAIN_OUT].setVoltage(inputs[selMux].getVoltage());
			}
		}

		// DEMUX
		// Lights correspond with selected output
		lights[selDemux + N_MUX_IN + 1].setBrightness(0.f);
		selMux = (unsigned int)clamp((int)params[D_SELECTOR_PARAM].getValue(), 0, N_DEMUX_OUT);
		lights[selDemux + N_MUX_IN + 1].setBrightness(1.f);

		if (inputs[D_MAIN_IN].isConnected()) {
			if (outputs[selDemux].isConnected()) {
				// Send the voltage of the Demux's input, to its selected output
				outputs[selDemux].setVoltage(inputs[D_MAIN_IN].getVoltage());
			}
		}
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

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(50, 28)), module, AMuxDemux::M_SELECTOR_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10, 85.525)), module, AMuxDemux::D_SELECTOR_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10, 22.989)), module, AMuxDemux::M_INPUT_1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10, 33.274)), module, AMuxDemux::M_INPUT_2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10, 44.588)), module, AMuxDemux::M_INPUT_3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10, 56.314)), module, AMuxDemux::M_INPUT_4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10, 98.690)), module, AMuxDemux::D_MAIN_IN));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50, 43.354)), module, AMuxDemux::M_MAIN_OUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50, 80)), module, AMuxDemux::D_OUTPUT_1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50, 90)), module, AMuxDemux::D_OUTPUT_2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50, 100)), module, AMuxDemux::D_OUTPUT_3));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(50, 110)), module, AMuxDemux::D_OUTPUT_4));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.921, 24.634)), module, AMuxDemux::M_LIGHT_1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.921, 36.360)), module, AMuxDemux::M_LIGHT_2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.921, 46.851)), module, AMuxDemux::M_LIGHT_3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(19.921, 58.782)), module, AMuxDemux::M_LIGHT_4));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(37.818, 80)), module, AMuxDemux::D_LIGHT_1));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(37.818, 90)), module, AMuxDemux::D_LIGHT_2));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(37.818, 100)), module, AMuxDemux::D_LIGHT_3));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(37.818, 110)), module, AMuxDemux::D_LIGHT_4));
	}
};


Model* modelAMuxDemux = createModel<AMuxDemux, AMuxDemuxWidget>("AMuxDemux");