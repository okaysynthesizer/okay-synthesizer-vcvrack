#include "../plugin.hpp"

#include "DiscomfortInternal/DiscomfortInternal.h"
#include "../RackUtils.h"
#include "daisysp.h"

using namespace daisysp;

struct Discomfort : Module {
	enum ParamId {
		IN_GAIN_PARAM,
		FOLD_MIX_KNOB_PARAM,
		DIST_MIX_KNOB_PARAM,
		OUT_GAIN_PARAM,
		FOLD_A_KNOB_PARAM,
		DIST_A_KNOB_PARAM,
		ENV_GAIN_PARAM,
		FOLD_B_KNOB_PARAM,
		DIST_B_KNOB_PARAM,
		ENV_ATTACK_PARAM,
		FOLD_C_KNOB_PARAM,
		DIST_C_KNOB_PARAM,
		ENV_DECAY_PARAM,
		FOLD_ENV_CV_PARAM,
		DIST_ENV_CV_PARAM,
		ENV_SELECT_A_PARAM,
		ENV_SELECT_B_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		IN_L_INPUT,
		IN_R_INPUT,
		FOLD_MIX_POT_INPUT,
		DIST_MIX_POT_INPUT,
		FOLD_A_CV_INPUT,
		DIST_A_CV_INPUT,
		FOLD_B_CV_INPUT,
		DIST_B_CV_INPUT,
		FOLD_C_CV_INPUT,
		DIST_C_CV_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		ENV_OUT_OUTPUT,
		OUT_L_OUTPUT,
		OUT_R_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LED_IN_L_LIGHT,
		LED_OUT_L_LIGHT,
		LED_IN_R_LIGHT,
		LED_OUT_R_LIGHT,
		LED_ENV_LIGHT,
		FOLD_A_LED_LIGHT,
		DIST_A_LED_LIGHT,
		FOLD_B_LED_LIGHT,
		DIST_B_LED_LIGHT,
		FOLD_C_LED_LIGHT,
		DIST_C_LED_LIGHT,
		LIGHTS_LEN
	};

	DiscomfortInternal *discomfortInternalL;
	DiscomfortInternal *discomfortInternalR;

	Discomfort() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(IN_GAIN_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FOLD_MIX_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DIST_MIX_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(OUT_GAIN_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FOLD_A_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DIST_A_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FOLD_B_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DIST_B_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FOLD_C_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DIST_C_KNOB_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ENV_GAIN_PARAM, 0.0001f, 2.f, 0.0001f, "");
		configParam(ENV_ATTACK_PARAM, 0.001f, 10000.f, 1.f, "");
		configParam(ENV_DECAY_PARAM, 0.001f, 10000.f, 1.f, "");
		configParam(FOLD_ENV_CV_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DIST_ENV_CV_PARAM, 0.f, 1.f, 0.f, "");
		configParam(ENV_SELECT_A_PARAM, 0.f, 7.f, 0.f, ""); // 0-7 for 3 bits
		configParam(ENV_SELECT_B_PARAM, 0.f, 7.f, 0.f, ""); // 0-7 for 3 bits
		configInput(IN_L_INPUT, "");
		configInput(IN_R_INPUT, "");
		configInput(FOLD_MIX_POT_INPUT, "");
		configInput(DIST_MIX_POT_INPUT, "");
		configInput(FOLD_A_CV_INPUT, "");
		configInput(DIST_A_CV_INPUT, "");
		configInput(FOLD_B_CV_INPUT, "");
		configInput(DIST_B_CV_INPUT, "");
		configInput(FOLD_C_CV_INPUT, "");
		configInput(DIST_C_CV_INPUT, "");
		configOutput(ENV_OUT_OUTPUT, "");
		configOutput(OUT_L_OUTPUT, "");
		configOutput(OUT_R_OUTPUT, "");

		discomfortInternalL = new DiscomfortInternal();
		discomfortInternalL->init(APP->engine->getSampleRate());
		discomfortInternalR = new DiscomfortInternal();
		discomfortInternalR->init(APP->engine->getSampleRate());
	}

	void onSampleRateChange(const SampleRateChangeEvent &e)
	{
		discomfortInternalL->init(e.sampleRate);
		discomfortInternalR->init(e.sampleRate);
	}

	void process(const ProcessArgs& args) override {
		DiscomfortInput input = DiscomfortInput::create();

		input.inputGain = map(
			params[IN_GAIN_PARAM].getValue(),
			0, 1,
			INPUT_MIN_GAIN,
			INPUT_MAX_GAIN
		);

		input.outputGain = map(
			params[OUT_GAIN_PARAM].getValue(),
			0, 1,
			OUTPUT_MIN_GAIN,
			OUTPUT_MAX_GAIN
		);

		input.setAParams(
			fclamp(params[FOLD_C_KNOB_PARAM].getValue() + rackCvToInternal(inputs[FOLD_C_CV_INPUT].getVoltage()), 0, 1),
			fclamp(params[FOLD_B_KNOB_PARAM].getValue() + rackCvToInternal(inputs[FOLD_B_CV_INPUT].getVoltage()), 0, 1),
			fclamp(params[FOLD_A_KNOB_PARAM].getValue() + rackCvToInternal(inputs[FOLD_A_CV_INPUT].getVoltage()), 0, 1),
			fclamp(params[FOLD_MIX_KNOB_PARAM].getValue() + rackCvToInternal(inputs[FOLD_MIX_POT_INPUT].getVoltage()), 0, 1)
		);

		input.setBParams(
			fclamp(params[DIST_A_KNOB_PARAM].getValue() + rackCvToInternal(inputs[DIST_A_CV_INPUT].getVoltage()), 0, 1),
			fclamp(params[DIST_B_KNOB_PARAM].getValue() + rackCvToInternal(inputs[DIST_B_CV_INPUT].getVoltage()), 0, 1),
			fclamp(params[DIST_C_KNOB_PARAM].getValue() + rackCvToInternal(inputs[DIST_C_CV_INPUT].getVoltage()), 0, 1),
			fclamp(params[DIST_MIX_KNOB_PARAM].getValue() + rackCvToInternal(inputs[DIST_MIX_POT_INPUT].getVoltage()), 0, 1)
		);

		input.setEnvFollowerValues(
			params[ENV_ATTACK_PARAM].getValue(),
			params[ENV_DECAY_PARAM].getValue(),
			params[ENV_GAIN_PARAM].getValue()
		);

		input.input = rackCvToInternal(inputs[IN_L_INPUT].getVoltage());
		input.envRoutingSelection = (int)round(params[ENV_SELECT_A_PARAM].getValue());
		DiscomfortOutput outputL = discomfortInternalL->process(input);
		outputs[OUT_L_OUTPUT].setVoltage(outputL.audioOutput);
		outputs[ENV_OUT_OUTPUT].setVoltage(outputL.followerOutput);
		lights[LED_IN_L_LIGHT].setSmoothBrightness(input.input, args.sampleTime);
		lights[LED_OUT_L_LIGHT].setSmoothBrightness(outputL.audioOutput, args.sampleTime);
		lights[LED_ENV_LIGHT].setSmoothBrightness(outputL.followerOutput, args.sampleTime);
		lights[FOLD_A_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 1 ? outputL.followerOutput : 0, args.sampleTime);
		lights[FOLD_B_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 2 ? outputL.followerOutput : 0, args.sampleTime);
		lights[FOLD_C_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 4 ? outputL.followerOutput : 0, args.sampleTime);

		input.input = rackCvToInternal(inputs[IN_R_INPUT].getVoltage());
		input.envRoutingSelection = (int)round(params[ENV_SELECT_B_PARAM].getValue());
		DiscomfortOutput outputR = discomfortInternalR->process(input);
		outputs[OUT_R_OUTPUT].setVoltage(outputR.audioOutput);
		lights[LED_IN_R_LIGHT].setSmoothBrightness(input.input, args.sampleTime);
		lights[LED_OUT_R_LIGHT].setSmoothBrightness(outputR.audioOutput, args.sampleTime);
		lights[DIST_A_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 1 ? outputL.followerOutput : 0, args.sampleTime);
		lights[DIST_B_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 2 ? outputL.followerOutput : 0, args.sampleTime);
		lights[DIST_C_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 4 ? outputL.followerOutput : 0, args.sampleTime);

		// lights[DIST_A_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 4, args.sampleTime);
		// lights[DIST_B_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 5, args.sampleTime);
		// lights[DIST_C_LED_LIGHT].setSmoothBrightness(input.envRoutingSelection & 6, args.sampleTime);
	}
};

struct DiscomfortWidget : ModuleWidget {
	DiscomfortWidget(Discomfort* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/discomfort.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(10.46, 42.983)), module, Discomfort::IN_GAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(29.707, 42.983)), module, Discomfort::FOLD_MIX_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(48.239, 42.983)), module, Discomfort::DIST_MIX_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(81.219, 42.983)), module, Discomfort::OUT_GAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(6.828, 59.304)), module, Discomfort::FOLD_A_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(82.403, 58.666)), module, Discomfort::DIST_A_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.715, 73.938)), module, Discomfort::ENV_GAIN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(6.798, 75.247)), module, Discomfort::FOLD_B_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(82.714, 74.513)), module, Discomfort::DIST_B_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.715, 89.786)), module, Discomfort::ENV_ATTACK_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(6.764, 92.805)), module, Discomfort::FOLD_C_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(82.774, 92.13)), module, Discomfort::DIST_C_KNOB_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(43.715, 107.402)), module, Discomfort::ENV_DECAY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(8.553, 109.77)), module, Discomfort::FOLD_ENV_CV_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(81.566, 109.115)), module, Discomfort::DIST_ENV_CV_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(28.457, 119.713)), module, Discomfort::ENV_SELECT_A_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(57.724, 120.08)), module, Discomfort::ENV_SELECT_B_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.41, 22.62)), module, Discomfort::IN_L_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.228, 22.62)), module, Discomfort::IN_R_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(34.047, 22.62)), module, Discomfort::FOLD_MIX_POT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(45.865, 22.62)), module, Discomfort::DIST_MIX_POT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(19.197, 59.023)), module, Discomfort::FOLD_A_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.815, 58.563)), module, Discomfort::DIST_A_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(19.078, 74.965)), module, Discomfort::FOLD_B_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.809, 74.589)), module, Discomfort::DIST_B_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(18.813, 92.59)), module, Discomfort::FOLD_C_CV_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(70.741, 92.078)), module, Discomfort::DIST_C_CV_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(58.089, 22.62)), module, Discomfort::ENV_OUT_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(69.502, 22.62)), module, Discomfort::OUT_L_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(81.32, 22.62)), module, Discomfort::OUT_R_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(3.431, 20.981)), module, Discomfort::LED_IN_L_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(88.071, 21.049)), module, Discomfort::LED_OUT_L_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(3.573, 24.688)), module, Discomfort::LED_IN_R_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(88.213, 24.756)), module, Discomfort::LED_OUT_R_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(61.205, 29.165)), module, Discomfort::LED_ENV_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(27.244, 59.44)), module, Discomfort::FOLD_A_LED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(62.575, 59.723)), module, Discomfort::DIST_A_LED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(27.623, 75.315)), module, Discomfort::FOLD_B_LED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(62.955, 75.598)), module, Discomfort::DIST_B_LED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(27.762, 92.659)), module, Discomfort::FOLD_C_LED_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(63.093, 92.943)), module, Discomfort::DIST_C_LED_LIGHT));
	}
};

Model* modelDiscomfort = createModel<Discomfort, DiscomfortWidget>("discomfort");