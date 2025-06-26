#ifndef DISCOMFORT_FOLDER_H
#define DISCOMFORT_FOLDER_H

// todo: reorder the words lol

#define FOLDER_MIN_GAIN 1
#define FOLDER_MAX_GAIN 50

// NOTE:
// These were originally +/- 1 - changed for tests
#define FOLDER_MIN_OFFSET 0 
#define FOLDER_MAX_OFFSET 1

#define FOLDER_MIN_SYMMETRY 0
#define FOLDER_MAX_SYMMETRY 1

class Folder {
  public:
    static float fold(float input, float gain, float offset, float symmetry);
};

#endif
