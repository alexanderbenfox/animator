#pragma once

#include <chrono>
#include <SDL.h>
#include <string>
#include <vector>
#include <io.h>

using namespace std::literals::chrono_literals;

class Timer
{
private:
  //The clock time when the timer started
  int startTicks;

  //The ticks stored when the timer was paused
  int pausedTicks;

  //The timer status
  bool paused;
  bool started;

public:
  //Initializes variables
  Timer();

  //The various clock actions
  void start();
  void stop();
  void pause();
  void unpause();

  //Gets the timer's time
  int get_ticks();

  //Checks the status of the timer
  bool is_started();
  bool is_paused();
};

struct cpu_clock {

  typedef std::chrono::milliseconds ms;
  typedef std::chrono::nanoseconds ns;

  //ms timestep = 16ms;
  Uint32 lag = 0;

  int fps = 60;

  int timestep = 1000 / fps;

  float frametime = 0.0075*(30.0 / (float)fps);

  using rep = clock_t;
  using period = std::ratio<1, CLOCKS_PER_SEC>;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<cpu_clock, duration>;
  static const bool is_steady = false;

  static Uint32 now() noexcept {
    return SDL_GetTicks();
  }

  static std::chrono::nanoseconds toNano(duration t) noexcept {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t);
  }

  static float toSeconds(duration t) noexcept {
    return std::chrono::duration_cast<std::chrono::duration<float>>(t).count();
  }

  void setFPS() {
    SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0 };
    SDL_GetDisplayMode(0, 0, &mode);
    fps = mode.refresh_rate;
  }

};

namespace FileUtils {
  inline bool FileExists(const std::string& name) {
    return _access(name.c_str(), 0) == 0;
  }
}

namespace StringUtils {
  inline std::vector<std::string> Split(const std::string& original, const std::string& delim) {
    std::vector<std::string> split;
    size_t last = 0;
    size_t pos = 0;
    while ((pos = original.find(delim, last)) != std::string::npos) {
      split.push_back(original.substr(last, pos));
      last = pos + delim.length();
    }
    split.push_back(original.substr(last, original.length()));
    return split;
  }
}