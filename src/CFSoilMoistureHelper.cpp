/**
 * CFSoilMoistureHelper.cpp
 * 
 * A library for Arduino that helps to integrate with soil moisture sensor.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Sep, 2021
 */

#include <CFSoilMoistureHelper.h>                                               // CF Soil Moisture.

/**
 * Constructor.
 *
 * @param analogPin Analog pin that should be read to get moisture data.
 */
CFSoilMoistureHelper::CFSoilMoistureHelper(int analogPin):
        _analogPin(analogPin),
        _moistureValue(1023), _moisturePercent(0),
        _ttRead(1000),
        _dryValue(1023), _wetValue(0) {
    
}

/**
 * Loop.
 */
void CFSoilMoistureHelper::loop() {
    _readData();
}

/**
 * Collect soil moisture data.
 */
void CFSoilMoistureHelper::_readData() {
    if (_tLastRead == 0 || (millis() - _tLastRead) > _ttRead) {
        Logger::verbose("Reading values.");

        // Read value from analog pin.
        _moistureValue = analogRead(_analogPin);

        // Map the raw value to a reverse read-friendly value.
        _reverseMoistureValue = map(_moistureValue, 1023, 0, 0, 1023);

        // Map the raw value to a percent read-friendly value.
        _moisturePercent = map(_moistureValue, _dryValue, _wetValue, 0, 100);

        // Adjust percent value if it's out of 0-100 range.
        _moisturePercent = (_moisturePercent > 100) ? 100 : ((_moisturePercent < 0) ? 0 : _moisturePercent);

        Logger::verbose("Dry / Wet: " + String(_dryValue) + " / " + String(_wetValue));
        Logger::verbose("Raw value: " + String(_moistureValue));
        Logger::verbose("Percent: " + String(_moisturePercent) + " %");

        // Update last read time.
        _tLastRead = millis();
    }
}

/**
 * Get dry value.
 *
 * @return Dry value.
 */
int CFSoilMoistureHelper::getRawDryValue() {
    return _dryValue;
}

/**
 * Define dry value.
 * If the new value is less or equals than wet value it will keep the previous value.
 *
 * @param dryValue Dry value.
 */
void CFSoilMoistureHelper::setRawDryValue(int dryValue) {
    if (dryValue > _wetValue) {
        _dryValue = dryValue;
    }
}

/**
 * Get wet value.
 *
 * @return Wet value.
 */
int CFSoilMoistureHelper::getRawWetValue() {
    return _wetValue;
}

/**
 * Define wet value.
 * If the new value is greater or equals than dry value it will keep the previous value.
 *
 * @param wetValue Wet value.
 */
void CFSoilMoistureHelper::setRawWetValue(int wetValue) {
    if (wetValue < _dryValue) {
        _wetValue = wetValue;
    }
}

/**
 * Get soil moisture raw value.
 * 0    : Wey
 * 1023 : Dry
 * 
 * @return Raw value read by sensor.
 */
int CFSoilMoistureHelper::getRawSensorValue() {
    return _moistureValue;
}

/**
 * Get reverse soil moisture raw value.
 * 0    : Dry
 * 1023 : Wet
 * 
 * @return Reverse raw value read by sensor.
 */
int CFSoilMoistureHelper::getRawSensorReverseValue() {
    return _reverseMoistureValue;
}

/**
 * Get soil moisture percent value.
 * 
 * @return Mapped percent value.
 */
int CFSoilMoistureHelper::getSersorPercent() {
    return _moisturePercent;
}

/**
 * Define time between readings.
 *
 * @param ttRead Time between readings.
 */
void CFSoilMoistureHelper::setReadingInterval(long ttRead) {
    _ttRead = ttRead;
}