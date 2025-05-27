#include "../plugin.hpp"
#include "lowstepper_rack/LowstepperRack.h"


struct DebugLowstepperV5 : Module {
	LowstepperRack* lowstepper;
	
	enum ParamId {
		RATE_KNOB_A_PARAM,
		RATE_KNOB_B_PARAM,
		MORPH_KNOB_A_PARAM,
		MORPH_KNOB_B_PARAM,
		START_KNOB_A_PARAM,
		START_KNOB_B_PARAM,
		END_KNOB_A_PARAM,
		END_KNOB_B_PARAM,
		SPEED_TOGGLE_A_PARAM,
		SPEED_TOGGLE_B_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		RATE_CV_A_INPUT,
		RATE_CV_B_INPUT,
		MORPH_CV_A_INPUT,
		MORPH_CV_B_INPUT,
		START_CV_A_INPUT,
		START_CV_B_INPUT,
		END_CV_B_INPUT,
		END_CV_A_INPUT,
		SYNC_IN_A_INPUT,
		RESET_IN_A_INPUT,
		RESET_IN_B_INPUT,
		SYNC_IN_B_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		CV_OUT_A_OUTPUT,
		EOC_OUT_A_OUTPUT,
		CV_OUT_B_OUTPUT,
		EOC_OUT_B_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		ENUMS(LED_A_LIGHT, 2),
		ENUMS(LED_B_LIGHT, 2),
		LIGHTS_LEN
	};

	DebugLowstepperV5() {
	    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(RATE_KNOB_A_PARAM, 0.f, 1.f, 0.f, namesChannelA::rateKnob);
		configParam(RATE_KNOB_B_PARAM, 0.f, 1.f, 0.f, namesChannelB::rateKnob);
		configParam(MORPH_KNOB_A_PARAM, 0.f, 1.f, 0.f, namesChannelA::morphKnob);
		configParam(MORPH_KNOB_B_PARAM, 0.f, 1.f, 0.f, namesChannelB::morphKnob);
		configParam(START_KNOB_A_PARAM, 0.f, 1.f, 0.f, namesChannelA::startKnob);
		configParam(START_KNOB_B_PARAM, 0.f, 1.f, 0.f, namesChannelB::startKnob);
		configParam(END_KNOB_A_PARAM, 0.f, 1.f, 1.f, namesChannelA::endKnob); // default value of 1
		configParam(END_KNOB_B_PARAM, 0.f, 1.f, 1.f, namesChannelB::endKnob); // default value of 1
		configSwitch(SPEED_TOGGLE_A_PARAM, -1.f, 1.f, 0, namesChannelA::speedToggle, {"Slow", "Medium", "Fast"});
		configSwitch(SPEED_TOGGLE_B_PARAM, -1.f, 1.f, 0, namesChannelB::speedToggle, {"Slow", "Medium", "Fast"});
		configInput(RATE_CV_A_INPUT, namesChannelA::rateCv);
		configInput(RATE_CV_B_INPUT, namesChannelB::rateCv);
		configInput(MORPH_CV_A_INPUT, namesChannelA::morphCv);
		configInput(MORPH_CV_B_INPUT, namesChannelB::morphCv);
		configInput(START_CV_A_INPUT, namesChannelA::startCv);
		configInput(START_CV_B_INPUT, namesChannelB::startCv);
		configInput(END_CV_B_INPUT, namesChannelB::endCv);
		configInput(END_CV_A_INPUT, namesChannelA::endCv);
		configInput(SYNC_IN_A_INPUT, namesChannelA::syncIn);
		configInput(RESET_IN_A_INPUT, namesChannelA::resetIn);
		configInput(RESET_IN_B_INPUT, namesChannelB::resetIn);
		configInput(SYNC_IN_B_INPUT, namesChannelB::syncIn);
		configOutput(CV_OUT_A_OUTPUT, namesChannelA::cvOut);
		configOutput(EOC_OUT_A_OUTPUT, namesChannelA::eocOut);
		configOutput(CV_OUT_B_OUTPUT, namesChannelB::cvOut);
		configOutput(EOC_OUT_B_OUTPUT, namesChannelB::eocOut);
		lowstepper = nullptr;
	}

	void process(const ProcessArgs& args) override {
	    if(lowstepper == nullptr) {
			lowstepper = new LowstepperRack(args.sampleRate);

			lowstepper->bindRateKnob(ChannelId::A, &getParam(RATE_KNOB_A_PARAM));
			lowstepper->bindMorphKnob(ChannelId::A, &getParam(MORPH_KNOB_A_PARAM));
			lowstepper->bindStartKnob(ChannelId::A, &getParam(START_KNOB_A_PARAM));
			lowstepper->bindEndKnob(ChannelId::A, &getParam(END_KNOB_A_PARAM));
			lowstepper->bindSpeedToggle(ChannelId::A, &getParam(SPEED_TOGGLE_A_PARAM));
			lowstepper->bindLed(ChannelId::A, &getLight(LED_A_LIGHT + 0), &getLight(LED_A_LIGHT + 1));
			lowstepper->bindRateCv(ChannelId::A, &getInput(RATE_CV_A_INPUT));
			lowstepper->bindMorphCv(ChannelId::A, &getInput(MORPH_CV_A_INPUT));
			lowstepper->bindStartCv(ChannelId::A, &getInput(START_CV_A_INPUT));
			lowstepper->bindEndCv(ChannelId::A, &getInput(END_CV_A_INPUT));
			lowstepper->bindSyncIn(ChannelId::A, &getInput(SYNC_IN_A_INPUT));
			lowstepper->bindResetIn(ChannelId::A, &getInput(RESET_IN_A_INPUT));
			lowstepper->bindCvOut(ChannelId::A, &getOutput(CV_OUT_A_OUTPUT));
			lowstepper->bindEocOut(ChannelId::A, &getOutput(EOC_OUT_A_OUTPUT));

			lowstepper->bindRateKnob(ChannelId::B, &getParam(RATE_KNOB_B_PARAM));
			lowstepper->bindMorphKnob(ChannelId::B, &getParam(MORPH_KNOB_B_PARAM));
			lowstepper->bindStartKnob(ChannelId::B, &getParam(START_KNOB_B_PARAM));
			lowstepper->bindEndKnob(ChannelId::B, &getParam(END_KNOB_B_PARAM));
			lowstepper->bindSpeedToggle(ChannelId::B, &getParam(SPEED_TOGGLE_B_PARAM));
			lowstepper->bindLed(ChannelId::B, &getLight(LED_B_LIGHT + 0), &getLight(LED_B_LIGHT + 1));
			lowstepper->bindRateCv(ChannelId::B, &getInput(RATE_CV_B_INPUT));
			lowstepper->bindMorphCv(ChannelId::B, &getInput(MORPH_CV_B_INPUT));
			lowstepper->bindStartCv(ChannelId::B, &getInput(START_CV_B_INPUT));
			lowstepper->bindEndCv(ChannelId::B, &getInput(END_CV_B_INPUT));
			lowstepper->bindSyncIn(ChannelId::B, &getInput(SYNC_IN_B_INPUT));
			lowstepper->bindResetIn(ChannelId::B, &getInput(RESET_IN_B_INPUT));
			lowstepper->bindCvOut(ChannelId::B, &getOutput(CV_OUT_B_OUTPUT));
			lowstepper->bindEocOut(ChannelId::B, &getOutput(EOC_OUT_B_OUTPUT));
		}

		lowstepper->tick(args);
	}
};


struct DebugLowstepperV5Widget : ModuleWidget {
	DebugLowstepperV5Widget(DebugLowstepperV5* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/DebugLowstepperV5.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.544, 13.762)), module, DebugLowstepperV5::RATE_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(68.534, 13.358)), module, DebugLowstepperV5::RATE_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.502, 30.254)), module, DebugLowstepperV5::MORPH_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(68.959, 29.714)), module, DebugLowstepperV5::MORPH_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.628, 46.54)), module, DebugLowstepperV5::START_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(69.213, 46.084)), module, DebugLowstepperV5::START_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(12.467, 62.865)), module, DebugLowstepperV5::END_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(69.361, 63.033)), module, DebugLowstepperV5::END_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(29.49, 110.657)), module, DebugLowstepperV5::SPEED_TOGGLE_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(55.202, 110.705)), module, DebugLowstepperV5::SPEED_TOGGLE_B_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.0, 13.614)), module, DebugLowstepperV5::RATE_CV_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(53.903, 13.465)), module, DebugLowstepperV5::RATE_CV_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.837, 30.106)), module, DebugLowstepperV5::MORPH_CV_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(53.895, 30.076)), module, DebugLowstepperV5::MORPH_CV_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(27.647, 46.487)), module, DebugLowstepperV5::START_CV_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(53.974, 46.263)), module, DebugLowstepperV5::START_CV_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(53.737, 62.72)), module, DebugLowstepperV5::END_CV_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.126, 63.057)), module, DebugLowstepperV5::END_CV_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(13.706, 139.859)), module, DebugLowstepperV5::SYNC_IN_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(32.233, 139.991)), module, DebugLowstepperV5::RESET_IN_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.229, 139.643)), module, DebugLowstepperV5::RESET_IN_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(51.434, 139.943)), module, DebugLowstepperV5::SYNC_IN_B_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(13.613, 155.071)), module, DebugLowstepperV5::CV_OUT_A_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(32.432, 155.338)), module, DebugLowstepperV5::EOC_OUT_A_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(51.649, 155.557)), module, DebugLowstepperV5::CV_OUT_B_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.595, 155.482)), module, DebugLowstepperV5::EOC_OUT_B_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenRedLight>>(mm2px(Vec(13.342, 110.941)), module, DebugLowstepperV5::LED_A_LIGHT));
		addChild(createLightCentered<MediumLight<GreenRedLight>>(mm2px(Vec(70.649, 111.197)), module, DebugLowstepperV5::LED_B_LIGHT));
	}
};


Model* modelDebugLowstepperV5 = createModel<DebugLowstepperV5, DebugLowstepperV5Widget>("DebugLowstepperV5");