
#pragma once

namespace sel {
  class Transitioner {
  public:
    Transitioner();

    float getValue();

  private:
    float duration;
    float startValue;
    float endValue;
    float value;
  };
  
}
