#ifndef DISCOMFORT_CLIPPER_H
#define DISCOMFORT_CLIPPER_H

#define CLIPPER_MIN_GAIN 1
#define CLIPPER_MAX_GAIN 40
#define CLIPPER_MIN_BEND 1
#define CLIPPER_MAX_BEND 200

enum ClipperType { CLIPPER_HARD, CLIPPER_SOFT, CLIPPER_ALT };

class Clipper {
  public:
    static float clip(float input, float gain, float bend);
};

#endif
