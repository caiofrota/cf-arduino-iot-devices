/**
 * CFSoilMoistureHelper.h
 * 
 * A library for Arduino that helps to integrate with soil moisture sensor.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Sep, 2021
 */

#ifndef CFSoilMoistureHelper_h
#define CFSoilMoistureHelper_h

#include <Arduino.h>                                                            // Arduino library.
#include <Logger.h>                                                             // Logger.

class CFSoilMoistureHelper {
    private:
        // Attributes.
        int _analogPin;                                                         // Analog pin that should be read to get moisture data.
        int _moistureValue;                                                     // Soil moisture value (wet 0 - 1023 dry).
        int _reverseMoistureValue;                                              // Reverse soil moisture value (dry 0 - 1023 wet).
        int _moisturePercent;                                                   // Soil moisture percentage (dry 0-100 wet).
        int _dryValue;                                                          // Sensor value when soil is dry. Default 1023.
        int _wetValue;                                                          // Sensor value when soil is wet. Default 0.

        // Loop control.
        unsigned long _ttRead;                                                  // Time between readings.
        unsigned long _tLastRead;                                               // Last time data was read.

        // Methods.
        void _readData();                                                       // Collect soil moisture data.

    public:
        // Methods.
        CFSoilMoistureHelper(int analogPin);                                    // Constructor.
        void loop();                                                            // Loop.

        // Accessors.
        int getRawDryValue();                                                   // Get dry value.
        void setRawDryValue(int dryValue);                                      // Define dry value.
        int getRawWetValue();                                                   // Get wet value.
        void setRawWetValue(int wetValue);                                      // Define wet value.
        int getRawSensorValue();                                                // Get raw value from sensor.
        int getRawSensorReverseValue();                                         // Get mapped value from sensor.
        int getSersorPercent();                                                 // Get mapped percent value.
        void setReadingInterval(long ttRead);                                   // Define time between readings.
};

#endif