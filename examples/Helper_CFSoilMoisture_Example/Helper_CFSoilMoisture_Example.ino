/**
 * CF Soil Moisture Example.
 *
 * An example of using the CF soil moisture helper.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Sep, 2021
 */

// Include the sensor library.
#include <CFSoilMoistureHelper.h>                                               // CF soil moisture sensor.

// Create a sensor object.
CFSoilMoistureHelper soilMoisture(A0);                                          // CF soil moisture sensor.

void setup() {
    // Start serial.
    Serial.begin(9600);

    // Before calibration.
    Serial.println("--- Before Calibration ---");
    Serial.print("Raw Dry Value: ");
    Serial.println(soilMoisture.getRawDryValue());
    Serial.print("Raw Wet Value: ");
    Serial.println(soilMoisture.getRawWetValue());
    
    // Sensor calibration.
    soilMoisture.setRawDryValue(740);
    soilMoisture.setRawWetValue(320);

    // After calibration.
    Serial.println("--- After Calibration ---");
    Serial.print("Raw Dry Value: ");
    Serial.println(soilMoisture.getRawDryValue());
    Serial.print("Raw Wet Value: ");
    Serial.println(soilMoisture.getRawWetValue());
    Serial.println("--- End Calibration ---\n");

    // Set reading interval to 5 seconds. Default is 1 second (1000 milliseconds).
    soilMoisture.setReadingInterval(5000); // 5 * 1000 milliseconds.
}

void loop() {
    // Update values.
    soilMoisture.loop();

    // Print read values.
    Serial.print("Raw Value (0 wet - 1023 dry):         ");
    Serial.println(soilMoisture.getRawSensorValue());
    Serial.print("Reverse Raw Value (0 dry - 1023 wet): ");
    Serial.println(soilMoisture.getRawSensorReverseValue());
    Serial.print("Percent Value (0% dry - 100% wet):    ");
    Serial.print(soilMoisture.getSersorPercent());
    Serial.println("%\n");

    // Delay.
    delay(1000);
}