/**
 * @file Easings.h
 * @author Edouard Levasseur
 * @brief Easing helper utility class to abstract away logic
 */

#ifndef COM3505_EASINGS_H
#define COM3505_EASINGS_H

// For NUM_LEDS
#include "Leds.h"

/**
 * Easing helper functions in a class for utility
 */
class EaseController {
public:
  /**
   * @brief Default constructor
   */
  EaseController() = default;

  /**
   * @brief Update the TOP value for the easings
   * @param progress int The new TOP value
   */
  void updateDelay(int progress) {
    max_progress = progress;
  }

  /**
   * @brief Ease linearly between the start and end
   * @param progress int The progress between 0 and TOP
   * @return The number of LEDs that should be on for that progress
   */
  int linearEase(int progress) {
    return (num_indicators + 1) * (float) progress / (float) max_progress;
  }

  /**
   * @brief Ease in between the start and end
   * @param progress int The progress between 0 and TOP
   * @return The number of LEDs that should be on for that progress
   */
  int easeInEase(int progress) {
    return (num_indicators + 1) * easeInExpo((float) progress / (float) max_progress);
  }

  /**
   * @brief Ease out between the start and end
   * @param progress int The progress between 0 and TOP
   * @return The number of LEDs that should be on for that progress
   */
  int easeOutEase(int progress) {
    return (num_indicators + 1) * easeOutExpo((float) progress / (float) max_progress);
  }
  /**
   * @brief Ease in and out between the start and end
   * @param progress int The progress between 0 and TOP
   * @return The number of LEDs that should be on for that progress
   */
  int easeInOutEase(float progress) {
    return (num_indicators + 1) * easeInOutExpo((float) progress / (float) max_progress);
  }

private:
  /// @brief The total number of indicators 
  static const uint32_t num_indicators = NUM_LEDS;

  /// @brief The current TOP value for the progress (like in hardware timers etc.)
  uint32_t max_progress;
  
  // Easing functions taken from https://easings.net

  /// @brief Cubic easings
  /// @param x float Input between 0 and 1
  /// @return float Output between 0 and 1
  float easeInCubic(float x) { return x * x * x; }
  float easeOutCubic(float x) { float y = 1.0f - x; return 1.0f - y * y * y; }
  float easeInOutCubic(float x) { float y = 2.0f - 2.0f * x; return x < 0.5f ? 4 * x * x * x : 1.0f - 0.5f * y * y * y; }

  /// @brief Exponential easings
  /// @param x float Input between 0 and 1
  /// @return float Output between 0 and 1
  float easeInExpo(float x) { return x == 0.0f ? 0.0f : pow(2.0f, 10.0f * x - 10.0f); }
  float easeOutExpo(float x) { return x == 1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * x); }
  float easeInOutExpo(float x) { return x == 0.0f ? 0.0f : x == 1.0f ? 1.0f : x < 0.5f ? 0.5f * pow(2.0f, 20.0f * x - 10.0f) : 1.0f - 0.5f * pow(2.0f, -20.0f * x + 10.0f); }

} Easings; // Default intiialise the class

#endif