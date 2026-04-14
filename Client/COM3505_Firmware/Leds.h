/**
 * @file Leds.h
 * @author Edouard Levasseur
 * @brief LED driver utility class to abstract away logic
 */

#ifndef COM3505_LEDS_H
#define COM3505_LEDS_H

/// Declare the total number of LEDs
#define NUM_LEDS 12

/**
 * LED driver class to control them
 */
class LEDController {
public:
  /**
   * @brief Default constructor
   */
  LEDController() = default;

  /**
   * @brief Initialisation function, iterate through the LED pins and set them as outputs
   */
  void initialise() {
    for (int led = 0; led < NUM_LEDS; ++led) {
      pinMode(LED_pins[led], OUTPUT);
    }
  }

  /**
   * @brief Write a value to a single LED
   * @param led int The LED index to write to
   * @param value bool The value to write to it
   */
  void writeSingle(int led, bool value) {
    LED_states &= ~((int) !value << led);
    LED_states |= value << led;
    digitalWrite(LED_pins[led], value);
  }

  /**
   * @brief Turn a specific LED on
   * @param led int The LED index to turn on
   */
  void setSingle(int led) {
    LED_states |= 1 << led;    
    digitalWrite(LED_pins[led], 1);
  }

  /**
   * @brief Turn a specifc LED off
   * @param led int The led index to turn off
   */
  void resetSingle(int led) {
    LED_states &= ~(1 << led);
    digitalWrite(LED_pins[led], 0);
  }

  /**
   * @brief Write all the LEDs at once; i.e., display a certain integer using the LEDs in binary
   * @param leds int The <NUM_LEDS>-bit integer to display
   */
  void writeAll(int leds) {
    LED_states = leds;
    updateAll();
  }

  /**
   * @brief Turn all the LEDs on
   */
  void setAll() {
    LED_states = (1 << NUM_LEDS) - 1;
    updateAll();
  }

  /**
   * @brief Turn all the LEDs off
   */
  void resetAll() {
    LED_states = 0;
    updateAll();
  }

  /**
   * @brief Display a bar chart scale using the LEDs
   * @param value int The number of LEDs to turn on
   */
  void showScale(int value) {
    for (int led = 0; led < value; ++led) LED_states |= 1 << led;
    for (int led = value; led < NUM_LEDS; ++led) LED_states &= ~(1 << led);
    updateAll();
  }

  /**
   * @brief Highlight a group of LEDs of size LED_group_counts (all the same colour)
   * @param group int The index of the group to turn on
   */
  void highlightGroup(int group) {
    resetAll();
    int first_led = group * LED_group_counts;
    int last_led = ++group * LED_group_counts;
    for (int led = first_led; led < last_led; ++led) setSingle(led);
    updateAll();
  }

  /**
   * @brief Turn on a random selection of the LEDs
   * @param seed int The random seed for the pseudo-random generator
   */
  void highlightRandom(int seed) {
    resetAll();
    randomSeed(seed);
    int randomState = random(2 << NUM_LEDS);
    writeAll(randomState);
  }

private:
  /// @brief Class constants, the LED pins and how many consecutive ones are the same colour
  static constexpr uint8_t LED_group_counts = 4;
  static constexpr uint8_t LED_pins[NUM_LEDS] = {3, 4, 5, 6, 9, 10, 11, 12, 13, 14, 15, 16};
  
  /// @brief The current state of the LEDs
  uint16_t LED_states = 0;

  /// @brief Utility function to update all the LED pin states to the stored states
  void updateAll() {
    for (int led = 0; led < NUM_LEDS; ++led) {
      digitalWrite(LED_pins[led], (bool) (LED_states & (1 << led)));
    }
  }
} LEDs; // Default initialise the class

#endif