#include "../plugin.hpp"
#include "lowstepper_rack/LowstepperRack.h"
#include "lowstepper_rack/RackIORef.h"

struct DebugLowstepperV7 : Module {
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

	DebugLowstepperV7() {
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


	void onSampleRateChange(const SampleRateChangeEvent &e) override {
		lowstepper->updateSampleRate(e.sampleRate);
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

struct DebugLowstepperV7Widget : ModuleWidget {
	DebugLowstepperV7Widget(DebugLowstepperV7* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/DebugLowstepperV7.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4.516, 20.894)), module, DebugLowstepperV7::RATE_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(44.705, 20.611)), module, DebugLowstepperV7::RATE_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4.486, 32.436)), module, DebugLowstepperV7::MORPH_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(45.015, 32.058)), module, DebugLowstepperV7::MORPH_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4.578, 43.832)), module, DebugLowstepperV7::START_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(45.201, 43.513)), module, DebugLowstepperV7::START_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(4.46, 55.257)), module, DebugLowstepperV7::END_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(45.309, 55.375)), module, DebugLowstepperV7::END_KNOB_B_PARAM));
		//addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(16.154, 88.702)), module, DebugLowstepperV7::SPEED_TOGGLE_A_PARAM));
		//addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(34.956, 88.736)), module, DebugLowstepperV7::SPEED_TOGGLE_B_PARAM));

		SlowMedFastSwitch* speedA = createParamCentered<SlowMedFastSwitch>(mm2px(Vec(16.154, 88.702)), module, DebugLowstepperV7::SPEED_TOGGLE_A_PARAM);
		SlowMedFastSwitch* speedB = createParamCentered<SlowMedFastSwitch>(mm2px(Vec(33.656, 88.736)), module, DebugLowstepperV7::SPEED_TOGGLE_B_PARAM);

		addParam(speedA);
		addParam(speedB);


		addInput(createInputCentered<GoldPort>(mm2px(Vec(15.818, 20.79)), module, DebugLowstepperV7::RATE_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(34.006, 20.687)), module, DebugLowstepperV7::RATE_CV_B_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(15.7, 32.332)), module, DebugLowstepperV7::MORPH_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(34.0, 32.311)), module, DebugLowstepperV7::MORPH_CV_B_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(15.561, 43.795)), module, DebugLowstepperV7::START_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(34.057, 43.639)), module, DebugLowstepperV7::START_CV_B_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(33.884, 55.156)), module, DebugLowstepperV7::END_CV_B_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(15.911, 55.392)), module, DebugLowstepperV7::END_CV_A_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(4.612, 107.021)), module, DebugLowstepperV7::SYNC_IN_A_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(18.159, 107.113)), module, DebugLowstepperV7::RESET_IN_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(45.944, 106.87)), module, DebugLowstepperV7::RESET_IN_B_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(32.2, 107.08)), module, DebugLowstepperV7::SYNC_IN_B_INPUT));

		addOutput(createOutputCentered<GoldPort>(mm2px(Vec(4.544, 117.666)), module, DebugLowstepperV7::CV_OUT_A_OUTPUT));
		addOutput(createOutputCentered<GoldPort>(mm2px(Vec(18.305, 117.853)), module, DebugLowstepperV7::EOC_OUT_A_OUTPUT));
		addOutput(createOutputCentered<PurplePort>(mm2px(Vec(32.358, 118.007)), module, DebugLowstepperV7::CV_OUT_B_OUTPUT));
		addOutput(createOutputCentered<PurplePort>(mm2px(Vec(46.212, 117.954)), module, DebugLowstepperV7::EOC_OUT_B_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenRedLight>>(mm2px(Vec(4.346, 88.901)), module, DebugLowstepperV7::LED_A_LIGHT));
		addChild(createLightCentered<MediumLight<GreenRedLight>>(mm2px(Vec(46.251, 89.08)), module, DebugLowstepperV7::LED_B_LIGHT));
	}
};


Model* modelDebugLowstepperV7 = createModel<DebugLowstepperV7, DebugLowstepperV7Widget>("DebugLowstepperV7");