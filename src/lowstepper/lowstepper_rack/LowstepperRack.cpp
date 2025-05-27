#include "LowstepperRack.h"

LowstepperRack::LowstepperRack(float sampleRate)
    : channelA(sampleRate), channelB(sampleRate)
{
    channelA.syncManager.init(sampleRate);
    channelB.syncManager.init(sampleRate);
}

void LowstepperRack::bindRateKnob(ChannelId id, rack::engine::Param *param)
{
    if(id == ChannelId::A) {
        channelA.rateKnob.param = param;
    }
    else {
        channelB.rateKnob.param = param;
    }
}

void LowstepperRack::bindMorphKnob(ChannelId id, rack::engine::Param *param)
{
    if(id == ChannelId::A) {
        channelA.morphKnob.param = param;
    }
    else {
        channelB.morphKnob.param = param;
    }
}

void LowstepperRack::bindStartKnob(ChannelId id, rack::engine::Param *param)
{
    if(id == ChannelId::A) {
        channelA.startKnob.param = param;
    }
    else {
        channelB.startKnob.param = param;
    }
}

void LowstepperRack::bindEndKnob(ChannelId id, rack::engine::Param *param)
{
    if(id == ChannelId::A) {
        channelA.endKnob.param = param;
    }
    else {
        channelB.endKnob.param = param;
    }
}

void LowstepperRack::bindSpeedToggle(ChannelId id, rack::engine::Param *param)
{
    if(id == ChannelId::A) {
        channelA.speedToggle.param = param;
    }
    else {
        channelB.speedToggle.param = param;
    }
}

void LowstepperRack::bindLed(ChannelId id, rack::engine::Light* greenLight, rack::engine::Light* redLight)
{
    if(id == ChannelId::A) {
        channelA.led.greenLight = greenLight;
        channelA.led.redLight = redLight;
    }
    else {
        channelB.led.greenLight = greenLight;
        channelB.led.redLight = redLight;
    }
}

void LowstepperRack::bindRateCv(ChannelId id, rack::engine::Input *input)
{
    if(id == ChannelId::A) {
        channelA.rateCv.input = input;
    }
    else {
        channelB.rateCv.input = input;
    }
}

void LowstepperRack::bindMorphCv(ChannelId id, rack::engine::Input *input)
{
    if(id == ChannelId::A) {
        channelA.morphCv.input = input;
    }
    else {
        channelB.morphCv.input = input;
    }
}

void LowstepperRack::bindStartCv(ChannelId id, rack::engine::Input *input)
{
    if(id == ChannelId::A) {
        channelA.startCv.input = input;
    }
    else {
        channelB.startCv.input = input;
    }
}

void LowstepperRack::bindEndCv(ChannelId id, rack::engine::Input *input)
{
    if(id == ChannelId::A) {
        channelA.endCv.input = input;
    }
    else {
        channelB.endCv.input = input;
    }
}

void LowstepperRack::bindSyncIn(ChannelId id, rack::engine::Input *input)
{
    if(id == ChannelId::A) {
        channelA.syncIn.input = input;
    }
    else {
        channelB.syncIn.input = input;
    }
}

void LowstepperRack::bindResetIn(ChannelId id, rack::engine::Input *input)
{
    if(id == ChannelId::A) {
        channelA.resetIn.input = input;
    }
    else {
        channelB.resetIn.input = input;
    }
}

void LowstepperRack::bindCvOut(ChannelId id, rack::engine::Output *output)
{
    if(id == ChannelId::A) {
        channelA.cvOut.output = output;
    }
    else {
        channelB.cvOut.output = output;
    }
}

void LowstepperRack::bindEocOut(ChannelId id, rack::engine::Output *output)
{
    if(id == ChannelId::A) {
        channelA.eocOut.output = output;
    }
    else {
        channelB.eocOut.output = output;
    }
}

void LowstepperRack::tick(const rack::engine::Module::ProcessArgs &args)
{
    const bool useSyncA = channelA.syncManager.tick(syncATriggerCheck());
	const float avgBpmValueA = channelA.syncManager.getBpm();
    const LowStepperLfoMode modeA = getModeA();   

	const bool useSyncB = channelB.syncManager.tick(syncBTriggerCheck());
	const float avgBpmValueB = channelB.syncManager.getBpm();
    const LowStepperLfoMode modeB = getModeB();
    
    LowStepperInput inputA;
	inputA.phase = channelA.lastOutput.phase;
	inputA.frequency = LowStepperLfo::mapRateInputToFrequency(getRateAInput(), useSyncA, modeA, avgBpmValueA);
	inputA.morph = LowStepperLfo::mapMorphInput(getMorphAInput());
	inputA.start = LowStepperLfo::mapStartInput(getStartAInput(), useSyncA);
	inputA.end = LowStepperLfo::mapLengthInput(getEndAInput(), useSyncA);
	inputA.shouldReset = resetATriggerCheck();

	LowStepperInput inputB;
	inputB.phase = channelB.lastOutput.phase;
	inputB.frequency = LowStepperLfo::mapRateInputToFrequency(getRateBInput(), useSyncB, modeB, avgBpmValueB);
	inputB.morph = LowStepperLfo::mapMorphInput(getMorphBInput());
	inputB.start = LowStepperLfo::mapStartInput(getStartBInput(), useSyncB);
	inputB.end = LowStepperLfo::mapLengthInput(getEndBInput(), useSyncB);
	inputB.shouldReset = resetBTriggerCheck();

    channelA.lastOutput = channelA.lfo.tick(inputA);
    channelB.lastOutput = channelB.lfo.tick(inputB);

    channelA.cvOut.setVolage(5.f * channelA.lastOutput.cvOutput);
    channelB.cvOut.setVolage(5.f * channelB.lastOutput.cvOutput);

    // Holding state is bad.
    channelA.tickEoc();
    channelB.tickEoc();
	channelA.eocOut.setVolage(channelA.eocVoltageOutput);
	channelB.eocOut.setVolage(channelB.eocVoltageOutput);
    
    const float aCvOutGreenBrightness = (channelA.lastOutput.cvOutput + 1.f) / 2.f;
    const float aCvOutRedBrightness = 1.f - aCvOutGreenBrightness;

    const float bCvOutGreenBrightness = (channelB.lastOutput.cvOutput + 1.f) / 2.f;
    const float bCvOutRedBrightness = 1.f - bCvOutGreenBrightness;

    channelA.led.setRedBrightness(aCvOutRedBrightness);
    channelA.led.setGreenBrightness(aCvOutGreenBrightness);

    channelB.led.setRedBrightness(bCvOutRedBrightness);
    channelB.led.setGreenBrightness(bCvOutGreenBrightness);
}

// pot = knob
// cv = jack
float LowstepperRack::combinePotAndCv(float pot, float cv)
{
    // Expect that pot is within the range of 0-1;
	// Expect that cv is within the range of +/-5V
	const float adjustedCv = cv / 5.f;
	return fmin(1, fmax(0, (pot + adjustedCv)));
}

float LowstepperRack::getRateAInput()
{
    const float pot = channelA.rateKnob.getValue();
    const float cv = channelA.rateCv.isConnected() ? channelA.rateCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getMorphAInput()
{   
    const float pot = channelA.morphKnob.getValue();
    const float cv = channelA.morphCv.isConnected() ? channelA.morphCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getStartAInput()
{
    const float pot = channelA.startKnob.getValue();
    const float cv = channelA.startCv.isConnected() ? channelA.startCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getEndAInput()
{
    const float pot = channelA.endKnob.getValue();
    const float cv = channelA.endCv.isConnected() ? channelA.endCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getRateBInput()
{
    const float pot = channelB.rateKnob.getValue();
    const float cv = channelB.rateCv.isConnected() ? channelB.rateCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getMorphBInput()
{
    const float pot = channelB.morphKnob.getValue();
    const float cv = channelB.morphCv.isConnected() ? channelB.morphCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getStartBInput()
{
    const float pot = channelB.startKnob.getValue();
    const float cv = channelB.startCv.isConnected() ? channelB.startCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

float LowstepperRack::getEndBInput()
{
    const float pot = channelB.endKnob.getValue();
    const float cv = channelB.endCv.isConnected() ? channelB.endCv.getVoltage() : 0;
    return combinePotAndCv(pot, cv);
}

LowStepperLfoMode LowstepperRack::getModeA()
{
    if(channelA.speedToggle.getValue() == 0.f) {
        return LowStepperLfoMode::MEDIUM;
    }
    else if(channelA.speedToggle.getValue() == -1.f) {
        return LowStepperLfoMode::SLOW;
    }
    else {
        return LowStepperLfoMode::FAST;
    }
}

LowStepperLfoMode LowstepperRack::getModeB()
{
    if(channelB.speedToggle.getValue() == 0.f) {
        return LowStepperLfoMode::MEDIUM;
    }
    else if(channelB.speedToggle.getValue() == -1.f) {
        return LowStepperLfoMode::SLOW;
    }
    else {
        return LowStepperLfoMode::FAST;
    }
}

bool LowstepperRack::syncATriggerCheck()
{
    if(!channelA.syncIn.isConnected()) {
        channelA.syncTrigger.isTriggered = false;
    }
    else {
        channelA.syncTrigger.update(channelA.syncIn.getVoltage());
    }
    return channelA.syncTrigger.triggerCheck();
}

bool LowstepperRack::syncBTriggerCheck()
{
    if(!channelB.syncIn.isConnected()) {
        channelB.syncTrigger.isTriggered = false;
    }
    else {
        channelB.syncTrigger.update(channelB.syncIn.getVoltage());
    }
    return channelB.syncTrigger.triggerCheck();
}

bool LowstepperRack::resetATriggerCheck()
{
    if(!channelA.resetIn.isConnected()) {
        channelA.resetTrigger.isTriggered = false;
    }
    else {
        channelA.resetTrigger.update(channelA.resetIn.getVoltage());
    }
    return channelA.resetTrigger.triggerCheck();
}

bool LowstepperRack::resetBTriggerCheck()
{
    if(!channelB.resetIn.isConnected()) {
        channelB.resetTrigger.isTriggered = false;
    }
    else {
        channelB.resetTrigger.update(channelB.resetIn.getVoltage());
    }
    return channelB.resetTrigger.triggerCheck();
}

void LowstepperRack::updateSampleRate(float sampleRate)
{
    channelA.lfo.updateSampleRate(sampleRate);
    channelB.lfo.updateSampleRate(sampleRate);
    channelA.syncManager.init(sampleRate);
    channelB.syncManager.init(sampleRate);
}