/**
 * @file Sensor.h
 * @author Edouard Levasseur
 * @brief Analog sensor utility class to abstract away setup and reading logic
 */

#ifndef COM3505_SENSOR_H
#define COM3505_SENSOR_H

/**
 * Analog sensor class
 */
class AnalogSensor {
public:
    /**
     * @brief Default field initialisation as the constructor
     */
    AnalogSensor() = default;

    /**
     * @brief Set the analog parameters for reading
     */
    void initialise() {
        analogReadResolution(12);
        analogSetPinAttenuation(sensor_pin, ADC_11db);
    }

    /**
     * @brief Read the ADC directly
     * @return int The ADC value in the range [0, 4096)
     */
    int readRaw() {
        return analogRead(sensor_pin);
    }

    /**
     * @brief Calculate the voltage at the analog pin
     * @return float The voltage at the analog pin
     */
    float readVoltage() {
        return (float) readRaw() * 3.3f / 4096.0f;
    }

    /**
     * @brief Calculate the temperature of the sensor
     * @return float The measured temperature in degrees C 
     */
    float readValue() {
        return (readVoltage() - 0.5f) * 100.0f;
    }

private:
    /// @brief The analog pin to read from
    uint32_t sensor_pin = A0;
} Temp; // Default initialise an instance of the class

#endif