#ifndef SORT_H
#define SORT_H

#include <utility>
#include <vector>

#include <SDL2/SDL.h>

#include "common.h"

namespace sort {

struct Sort {
  Sort(int16_t min_bound, int16_t max_bound, uint32_t number_count);

public:
  auto step(double delta_time) -> void; // emulate step in loop
  auto render(SDL_Renderer* renderer) -> void;

public:
  std::vector<int16_t> numbers;

private:
  uint32_t i;
  uint32_t j;

  std::pair<uint16_t, uint16_t> rand_range_bounds;
  uint32_t rand_number_count;
};

}

#endif // SORT_H