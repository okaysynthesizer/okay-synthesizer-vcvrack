#include <cmath>
#include "DryWet.h"

float DryWet::blend(float dry, float wet, float blend) {
  return (dry * (1.0f - blend)) + (wet * blend);
}
