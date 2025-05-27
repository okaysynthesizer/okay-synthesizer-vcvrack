#include "RackIORef.h"
#include <math.h>

#define M_PI_2 1.57079632679489661923

extern rack::Plugin* pluginInstance;

SlowMedFastSwitch::SlowMedFastSwitch()
    : currentMode(MEDIUM)
{
    // Load shared_ptr svgs from plugin resources
    // https://github.com/VCVRack/Rack/blob/v2/src/app/SvgKnob.cpp

    this->slowSvg = rack::Svg::load(rack::asset::plugin(pluginInstance, "res/3-way-switch-slow.svg"));
    this->mediumSvg = rack::Svg::load(rack::asset::plugin(pluginInstance, "res/3-way-switch-medium.svg"));
    this->fastSvg = rack::Svg::load(rack::asset::plugin(pluginInstance, "res/3-way-switch-fast.svg"));

    this->horizontal = true;
    this->speed = 1.5;
    this->minAngle = -M_PI_2;
    this->maxAngle = M_PI_2;

    this->fb = new rack::widget::FramebufferWidget;
    addChild(fb);

    this->sw = new rack::widget::SvgWidget;
    this->fb->addChild(this->sw);

	setSvg(this->mediumSvg);
}

void SlowMedFastSwitch::setSvg(std::shared_ptr<rack::window::Svg> svg) {
    if(svg == this->sw->svg) 
        return;

    this->sw->setSvg(svg);
    this->fb->box.size = sw->box.size;
    this->box.size = sw->box.size;

    this->fb->setDirty();
}

void SlowMedFastSwitch::onChange(const ChangeEvent &e)
{
    // https://github.com/VCVRack/Rack/blob/8c6f41b778b4bf8860b89b36d5503fd37924077f/src/app/SvgKnob.cpp#L40

	rack::engine::ParamQuantity* pq = getParamQuantity();
    if(pq) {
		float value = pq->getValue();
        if(value == 0.f) {
            this->currentMode = MEDIUM;
            setSvg(this->mediumSvg);
        }
        else if(value > 0.f) {
            this->currentMode = FAST;
            setSvg(this->fastSvg);
        }
        else {
            this->currentMode = SLOW;
            setSvg(this->slowSvg);
        }
    }

	this->fb->setDirty();

    // Call this as parent
	Knob::onChange(e);

    //INFO("on change!");
}

extern rack::Plugin* pluginInstance;

//setSvg(Svg::load(asset::system("res/ComponentLibrary/PJ301M.svg")));

GoldPort::GoldPort() {
    setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/components/GoldPort.svg")));
}

PurplePort::PurplePort() {
    setSvg(rack::Svg::load(rack::asset::plugin(pluginInstance, "res/components/PurplePort.svg")));
}

float ParamRef::getValue()
{
    return param->getValue();
}

void ParamRef::setValue(float value)
{
    param->setValue(value);
}

bool InputRef::isConnected()
{
    return input->isConnected();
}

float InputRef::getVoltage()
{
    return input->getVoltage();
}

void InputRef::setVolage(float voltage)
{
    input->setVoltage(voltage);
}

bool OutputRef::isConnected()
{
    return output->isConnected();
}

float OutputRef::getVoltage()
{
    return output->getVoltage();
}

void OutputRef::setVolage(float voltage)
{
    output->setVoltage(voltage);
}

void GreenRedLightRef::setRedBrightness(float brightness) 
{
    redLight->setBrightness(brightness);
}

void GreenRedLightRef::setGreenBrightness(float brightness)
{
    greenLight->setBrightness(brightness);
}

void InputTrigger::update(float voltage)
{
    const float HIGH = 2.f;
    const float CUTOFF = 0.1f;

    if(isGateHigh) {
        if(voltage <= CUTOFF) {
            isGateHigh = false;
        }
    }
    else {
        if(voltage >= HIGH && !isGateHigh) {
            isGateHigh = true;
            isTriggered = true;
        }
    }
}

bool InputTrigger::triggerCheck()
{
    if(isTriggered) {
        isTriggered = false;
        return true;
    }
    return false;
}
