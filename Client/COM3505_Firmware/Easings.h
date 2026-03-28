#ifndef COM3505_EASINGS_H
#define COM3505_EASINGS_H

class EaseController {
public:
  EaseController() = default;

  void updateDelay(uint32_t progress) {
    max_progress = progress;
  }

  int linearEase(int progress) {
    float x = (float) progress / (float) max_progress;
    return num_indicators * x;
  }

  int easeInEase(int progress) {
    float x = (float) progress / (float) max_progress;
    return num_indicators * x * x * x;
  }

  int easeOutEase(int progress) {
    float x = (float) progress / (float) max_progress;
    float y = 1.0f - x;
    return num_indicators * (1.0f - y * y * y);
  }

  float easeInOutEase(float progress) {
    float x = (float) progress / (float) max_progress;
    if (x < 0.5) {
      return num_indicators * 4 * x * x * x;
    } else {
      float y = 2.0f - 2.0f * x;
      return num_indicators * (1.0f - 0.5f * y * y * y);
    }
  }
private:
  static const uint32_t num_indicators = 12;
  uint32_t max_progress;

} Easings;

#endif