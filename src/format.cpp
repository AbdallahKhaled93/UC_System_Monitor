#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  long minutes = (seconds / 60) % 60;
  long hours = (seconds / 60) / 60;
  long remainingSeconds = seconds % 60;

  return string(std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
                std::to_string(remainingSeconds));
}