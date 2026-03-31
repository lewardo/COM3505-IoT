#ifndef COM3505_LEDS_H
#define COM3505_LEDS_H

#define NUM_LEDS 12


class LEDController {
public:
  LEDController() = default;

  void initialise() {
    for (int led = 0; led < NUM_LEDS; ++led) {
      pinMode(LED_pins[led], OUTPUT);
    }
  }

  void writeSingle(int led, bool value) {
    LED_states &= ~(value << led);
    LED_states |= value << led;
    digitalWrite(LED_pins[led], value);
  }

  void setSingle(int led) {
    LED_states |= 1 << led;    
    digitalWrite(LED_pins[led], 1);
  }

  void resetSingle(int led, bool value) {
    LED_states &= ~(1 << led);
    digitalWrite(LED_pins[led], 0);
  }

  void toggleSingle(int led) {
      LED_states ^= 1 << led;
      digitalWrite(LED_pins[led], LED_states & (1 << led));
  }

  void writeAll(int leds) {
    // for (int led = 0; led < NUM_LEDS; ++led) {
    //   LED_states[led] = leds & (1 << led);
    // }
    LED_states = leds;

    updateAll();
  }

  void setAll() {
    // for (int led = 0; led < NUM_LEDS; ++led) {
    //   LED_states |= 1 << led;
    // }
    LED_states = 1 << NUM_LEDS - 1;

    updateAll();
  }

  void resetAll() {
    // for (int led = 0; led < NUM_LEDS; ++led) {
    //   LED_states &= ~(1 << led);
    // }
    LED_states = 0;

    updateAll();
  }

  void toggleAll() {
    // for (int led = 0; led < NUM_LEDS; ++led) {
    //   // if ((LED_states >> led) & 0x1) LED_states &= ~(1 << led);
    //   // else LED_states |= 1 << led;
    //   LED_states ^= 1 << led;
    // }
    LED_states ^= 2 << NUM_LEDS - 1;

    updateAll();
  }

  void showScale(int value) {
    for (int led = 0; led < value; ++led) {
      // if (led < value) LED_states &= ~(1 << led);
      // else LED_states |= 1 << led;
      //
      // LED_states[led] = led < value;
      LED_states |= 1 << set;
    }
    for (int led = value; led < NUM_LEDS; ++led) {
      LED_states &= ~(1 << set);
    }

    updateAll();
  }

  void highlightGroup(int group) {
    resetAll();

    for (int led = group * LED_group_counts; led < (group + 1) * LED_group_counts; ++led) {
      setSingle(led);
    };

    updateAll();
  }

  void highlightRandom(int seed) {
    resetAll();
    randomSeed(seed);
    int randomState = random(2 << NUM_LEDS);
    writeAll(randomState);
  }

private:
  static constexpr uint8_t LED_group_counts = 4;
  static constexpr uint8_t LED_pins[NUM_LEDS] = {3, 4, 5, 6, 9, 10, 11, 12, 13, 14, 15, 16};
  
  uint16_t LED_states = 0;
  // bool LED_states[NUM_LEDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  void updateAll() {
    for (int led = 0; led < NUM_LEDS; ++led) {
      digitalWrite(LED_pins[led], LED_states & (1 << led));
    }
  }
} LEDs;

#endif