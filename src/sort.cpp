
#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <random>

#include "sort.h"

namespace sort {

Sort::Sort(int16_t min_bound, int16_t max_bound, uint32_t number_count) {
  numbers.resize(number_count);

  std::random_device rnd_device;
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random integers
  std::uniform_int_distribution<int16_t> dist{min_bound, max_bound};

  std::ranges::generate(numbers, [&dist, &mersenne_engine]{
    return dist(mersenne_engine);
  });

  i = 0;
  j = 0;

  rand_range_bounds = {min_bound, max_bound};
  rand_number_count = number_count;
}

// basic bubble sort state machine
auto Sort::step(double delta_time) -> void {
  (void) delta_time;
  
  // stop steps when algorithm completes
  if(i >= numbers.size()) return;

  //printf("i = %d, j = %d\n", i, j);
  assert(i < numbers.size());
  assert(j < numbers.size());

  if(numbers[j] < numbers[i])
    std::swap(numbers[j], numbers[i]);

  if((j+1) % numbers.size() == 0) {
    i += 1;
    j = 0;
  } else {
    j += 1;
  }
}

auto Sort::render(SDL_Renderer* renderer) -> void {
  const auto [min, max] = rand_range_bounds;

  uint32_t index = 0;
  for(auto n : numbers) {
    if(index == i)
      SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    else if (index == j)
      SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    else
      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    
    SDL_RenderDrawLine(renderer, index, n, index, max);
    
    index += 1;
  }
}

}