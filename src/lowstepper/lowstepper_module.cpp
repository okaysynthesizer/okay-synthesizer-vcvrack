#include "../plugin.hpp"
#include "lowstepper_rack/LowstepperRack.h"
#include "lowstepper_rack/RackIORef.h"


struct Lowstepper : Module {
	LowstepperRack* lowstepper;
	
	enum ParamId {
		RATE_KNOB_A_PARAM,
		RATE_KNOB_B_PARAM,
		MORPH_KNOB_B_PARAM,
		MORPH_KNOB_A_PARAM,
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
		START_CV_B_INPUT,
		START_CV_A_INPUT,
		END_CV_A_INPUT,
		END_CV_B_INPUT,
		SYNC_CV_A_INPUT,
		RESET_CV_A_INPUT,
		SYNC_CV_B_INPUT,
		RESET_CV_B_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		CV_OUT_A_OUTPUT,
		EOC_A_OUTPUT,
		CV_OUT_B_OUTPUT,
		EOC_B_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		ENUMS(LED_A_LIGHT, 2),
		ENUMS(LED_B_LIGHT, 2),
		LIGHTS_LEN
	};

	Lowstepper() {
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
		configInput(SYNC_CV_A_INPUT, namesChannelA::syncIn);
		configInput(RESET_CV_A_INPUT, namesChannelA::resetIn);
		configInput(RESET_CV_B_INPUT, namesChannelB::resetIn);
		configInput(SYNC_CV_B_INPUT, namesChannelB::syncIn);
		configOutput(CV_OUT_A_OUTPUT, namesChannelA::cvOut);
		configOutput(EOC_A_OUTPUT, namesChannelA::eocOut);
		configOutput(CV_OUT_B_OUTPUT, namesChannelB::cvOut);
		configOutput(EOC_B_OUTPUT, namesChannelB::eocOut);
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
			lowstepper->bindSyncIn(ChannelId::A, &getInput(SYNC_CV_A_INPUT));
			lowstepper->bindResetIn(ChannelId::A, &getInput(RESET_CV_A_INPUT));
			lowstepper->bindCvOut(ChannelId::A, &getOutput(CV_OUT_A_OUTPUT));
			lowstepper->bindEocOut(ChannelId::A, &getOutput(EOC_A_OUTPUT));

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
			lowstepper->bindSyncIn(ChannelId::B, &getInput(SYNC_CV_B_INPUT));
			lowstepper->bindResetIn(ChannelId::B, &getInput(RESET_CV_B_INPUT));
			lowstepper->bindCvOut(ChannelId::B, &getOutput(CV_OUT_B_OUTPUT));
			lowstepper->bindEocOut(ChannelId::B, &getOutput(EOC_B_OUTPUT));
		}

		lowstepper->tick(args);
	}
};


struct LowstepperWidget : ModuleWidget {
	LowstepperWidget(Lowstepper* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/lowstepper.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.0, 24.117)), module, Lowstepper::RATE_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.0, 24.117)), module, Lowstepper::RATE_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.0, 39.617)), module, Lowstepper::MORPH_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.0, 40.117)), module, Lowstepper::MORPH_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.0, 57.117)), module, Lowstepper::START_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.0, 57.004)), module, Lowstepper::START_KNOB_B_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(7.0, 73.117)), module, Lowstepper::END_KNOB_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.0, 73.117)), module, Lowstepper::END_KNOB_B_PARAM));
		addParam(createParamCentered<SlowMedFastSwitch>(mm2px(Vec(14.0, 81.375)), module, Lowstepper::SPEED_TOGGLE_A_PARAM));
		addParam(createParamCentered<SlowMedFastSwitch>(mm2px(Vec(37.0, 81.375)), module, Lowstepper::SPEED_TOGGLE_B_PARAM));

		addInput(createInputCentered<GoldPort>(mm2px(Vec(20.0, 23.617)), module, Lowstepper::RATE_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(30.0, 23.617)), module, Lowstepper::RATE_CV_B_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(20.0, 39.617)), module, Lowstepper::MORPH_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(30.0, 39.617)), module, Lowstepper::MORPH_CV_B_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(30.0, 56.504)), module, Lowstepper::START_CV_B_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(20.0, 56.617)), module, Lowstepper::START_CV_A_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(20.0, 72.617)), module, Lowstepper::END_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(30.0, 72.617)), module, Lowstepper::END_CV_B_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(6.0, 94.0)), module, Lowstepper::SYNC_CV_A_INPUT));
		addInput(createInputCentered<GoldPort>(mm2px(Vec(19.0, 94.0)), module, Lowstepper::RESET_CV_A_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(31.123, 94.0)), module, Lowstepper::SYNC_CV_B_INPUT));
		addInput(createInputCentered<PurplePort>(mm2px(Vec(44.123, 94.0)), module, Lowstepper::RESET_CV_B_INPUT));

		addOutput(createOutputCentered<GoldPort>(mm2px(Vec(6.0, 107.569)), module, Lowstepper::CV_OUT_A_OUTPUT));
		addOutput(createOutputCentered<GoldPort>(mm2px(Vec(19.0, 107.569)), module, Lowstepper::EOC_A_OUTPUT));
		addOutput(createOutputCentered<PurplePort>(mm2px(Vec(31.123, 107.569)), module, Lowstepper::CV_OUT_B_OUTPUT));
		addOutput(createOutputCentered<PurplePort>(mm2px(Vec(44.123, 107.569)), module, Lowstepper::EOC_B_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenRedLight>>(mm2px(Vec(4.5, 81.257)), module, Lowstepper::LED_A_LIGHT));
		addChild(createLightCentered<MediumLight<GreenRedLight>>(mm2px(Vec(46.679, 81.257)), module, Lowstepper::LED_B_LIGHT));
	}
};


Model* modelLowstepper = createModel<Lowstepper, LowstepperWidget>("lowstepper");