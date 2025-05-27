#pragma once

#include "Names.h"
#include "LowstepperChannel.h"

class LowstepperRack {
public:
    LowstepperRack(float sampleRate);

    void bindRateKnob(ChannelId id, rack::engine::Param* param);
    void bindMorphKnob(ChannelId id, rack::engine::Param* param);
    void bindStartKnob(ChannelId id, rack::engine::Param* param);
    void bindEndKnob(ChannelId id, rack::engine::Param* param);
    void bindSpeedToggle(ChannelId id, rack::engine::Param* param);
    void bindLed(ChannelId id, rack::engine::Light* greenLight, rack::engine::Light* redLight);
    void bindRateCv(ChannelId id, rack::engine::Input* input);
    void bindMorphCv(ChannelId id, rack::engine::Input* input);
    void bindStartCv(ChannelId id, rack::engine::Input* input);
    void bindEndCv(ChannelId id, rack::engine::Input* input);
    void bindSyncIn(ChannelId id, rack::engine::Input* input);
    void bindResetIn(ChannelId id, rack::engine::Input* input);
    void bindCvOut(ChannelId id, rack::engine::Output* output);
    void bindEocOut(ChannelId id, rack::engine::Output* output);

    void tick(const rack::engine::Module::ProcessArgs& args);
    void updateSampleRate(float sampleRate);

private:
    static float combinePotAndCv(float pot, float cv);

    float getRateAInput();
    float getMorphAInput();
    float getStartAInput();
    float getEndAInput();

    float getRateBInput();
    float getMorphBInput();
    float getStartBInput();
    float getEndBInput();

    LowStepperLfoMode getModeA();
    LowStepperLfoMode getModeB();

    bool syncATriggerCheck();
    bool syncBTriggerCheck();

    bool resetATriggerCheck();
    bool resetBTriggerCheck();
private:

    Channel channelA;
    Channel channelB;
};