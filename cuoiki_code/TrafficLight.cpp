#include "TrafficLight.h"

TrafficLight::TrafficLight(int greenPin, int yellowPin, int redPin, int greenPinInterval, int yellowPinInterval, int redPinInterval)
    : _greenPin(greenPin), _yellowPin(yellowPin), _redPin(redPin),
      _greenPinInterval(greenPinInterval), _yellowPinInterval(yellowPinInterval), _redPinInterval(redPinInterval),
      _greenTmp(greenPinInterval), _redTmp(redPinInterval) {}

void TrafficLight::begin()
{
    pinMode(_redPin, OUTPUT);
    pinMode(_yellowPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
}

void TrafficLight::greenPinActivate()
{
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_yellowPin, LOW);
    digitalWrite(_redPin, LOW);
}

void TrafficLight::yellowPinActivate()
{
    digitalWrite(_yellowPin, HIGH);
    digitalWrite(_greenPin, LOW);
    digitalWrite(_redPin, LOW);
}

void TrafficLight::redPinActivate()
{
    digitalWrite(_redPin, HIGH);
    digitalWrite(_greenPin, LOW);
    digitalWrite(_yellowPin, LOW);
}

void TrafficLight::pinDeactivate()
{
    digitalWrite(_redPin, LOW);
    digitalWrite(_greenPin, LOW);
    digitalWrite(_yellowPin, LOW);
}

int TrafficLight::getGreenPinInterval()
{
    return _greenPinInterval;
}

int TrafficLight::getYellowPinInterval()
{
    return _yellowPinInterval;
}

int TrafficLight::getRedPinInterval()
{
    return _redPinInterval;
}

void TrafficLight::peakHourMode()
{
    _greenPinInterval += 30;
    _redPinInterval += 30;
}

void TrafficLight::normalInterval()
{
    _greenPinInterval = _greenTmp;
    _redPinInterval = _redTmp;
}

void TrafficLight::updateIntervals(int greenInterval, int yellowInterval, int redInterval)
{
    _greenPinInterval = greenInterval;
    _yellowPinInterval = yellowInterval;
    _redPinInterval = redInterval;
}