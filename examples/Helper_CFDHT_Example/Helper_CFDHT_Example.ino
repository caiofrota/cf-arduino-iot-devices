/**
 * CF DHT Example.
 *
 * An example of using the CF DHT helper.
 * 
 * @author  Caio Frota <caiofrota@gmail.com>
 * @version 1.0
 * @since   Sep, 2021
 */

 // Include the sensor library.
#include <CFSoilMoistureHelper.h>                                               // CF soil moisture sensor.

// DHT Pins.
const int DHT_PIN_DATA = 2;               // (GPIO2 / D4 - NodeMCU)             // DHT Pin Data.
const int DHT_PIN_RESET  = 0;             // (GPIO0 / D3 - NodeMCU)             // DHT Pin GND or VCC (Workaround for DHT reading failure).

// Create a sensor object.
//CFDHTHelper dht(DHT11, DHT_PIN_DATA, DHT_PIN_RESET);                            // CF DHT sensor (DHT11).
CFDHTHelper dht(DHT22, DHT_PIN_DATA, DHT_PIN_RESET);                            // CF DHT sensor (DHT22).

void setup() {
    // Start serial.
    Serial.begin(9600);

    dht.begin();
    dht.setReadingInterval(5000); // 5 * 1000 milliseconds.                     // Define reading interval.
}

void loop() {
    // Update values.
    dht.loop();

    // Print read values.
    if (dht.isRead()) {
        Serial.print("Temperature C: ");
        Serial.println(dht.getTemperatureC());
        Serial.print("Temperature F: ");
        Serial.println(dht.getTemperatureF());
        Serial.print("Heat Index C:  ");
        Serial.println(dht.getHeatIndexC());
        Serial.print("Heat Index F:  ");
        Serial.println(dht.getHeatIndexC());
        Serial.print("Humidity %:    ");
        Serial.print(dht.getHumidity());
        Serial.println("%\n");
    } else {
        Serial.println("Error reading values.\n");
    }

    // Delay.
    delay(1000);
}