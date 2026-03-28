#ifndef COM3505_SENSOR_H
#define COM3505_SENSOR_H

class AnalogSensor {
public:
    AnalogSensor() = default;

    void initialise() {
        analogReadResolution(12);
        analogSetPinAttenuation(sensor_pin, ADC_11db);
    }

    int readRaw() {
        return analogRead(sensor_pin);
    }

    float readVoltage() {
        return (float) readRaw() * 3.3f / 4096.0f;
    }

    float readValue() {
        return (readVoltage() - 0.5f) * 100.0f;
    }

private:
    uint32_t sensor_pin = A0;
} Temp;

#endif