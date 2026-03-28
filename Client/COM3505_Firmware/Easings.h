#ifndef COM3505_EASINGS_H
#define COM3505_EASINGS_H

class EaseController {
public:
  EaseController() = default;

  void updateDelay(uint32_t progress) {
    max_progress = progress;
  }

  int linearEase(int progress) {
    return (num_indicators + 1) * (float) progress / (float) max_progress;
  }

  int easeInEase(int progress) {
    return (num_indicators + 1) * easeInExpo((float) progress / (float) max_progress);
  }

  int easeOutEase(int progress) {
    return (num_indicators + 1) * easeOutExpo((float) progress / (float) max_progress);
  }

  int easeInOutEase(float progress) {
    return (num_indicators + 1) * easeInOutExpo((float) progress / (float) max_progress);
  }

private:
  static const uint32_t num_indicators = 12;
  uint32_t max_progress;
  
  // from easings.net
  float easeInCubic(float x) { return x * x * x; }
  float easeOutCubic(float x) { float y = 1.0f - x; return 1.0f - y * y * y; }
  float easeInOutCubic(float x) { float y = 2.0f - 2.0f * x; return x < 0.5f ? 4 * x * x * x : 1.0f - 0.5f * y * y * y; }

  float easeInExpo(float x) { return x == 0.0f ? 0.0f : pow(2.0f, 10.0f * x - 10.0f); }
  float easeOutExpo(float x) { return x == 1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * x); }
  float easeInOutExpo(float x) { return x == 0.0f ? 0.0f : x == 1.0f ? 1.0f : x < 0.5f ? 0.5f * pow(2.0f, 20.0f * x - 10.0f) : 1.0f - 0.5f * pow(2.0f, -20.0f * x + 10.0f); }

} Easings;

#endif