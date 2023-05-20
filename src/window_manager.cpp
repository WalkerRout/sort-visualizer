
#include <iostream>

#include <SDL2/SDL.h>

#include "sort.h"
#include "window_manager.h"

static auto handle_sdl_keydown(SDL_Keycode key, bool& keep_alive) -> void;

namespace window_manager {

auto SDL2Destructor::operator()(SDL_Window* w)   const -> void { SDL_DestroyWindow(w); }
auto SDL2Destructor::operator()(SDL_Renderer* r) const -> void { SDL_DestroyRenderer(r); }

WindowManager::WindowManager(uint32_t width, uint32_t height): dimensions({width, height}) {
  constexpr uint32_t DEFAULT_INIT_FLAGS = 0;
  constexpr uint32_t DEFAULT_WINDOW_FLAGS = 0;
  constexpr const char *TITLE = "Bubble Sort Algorithm Visualizer";

  if(SDL_Init(DEFAULT_INIT_FLAGS) < 0) goto fail;

  window = std::unique_ptr<SDL_Window, SDL2Destructor>(
    SDL_CreateWindow(
      TITLE, 
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
      width, height, 
      DEFAULT_WINDOW_FLAGS
    )
  );
  if(window == nullptr) goto fail;

  renderer = std::unique_ptr<SDL_Renderer, SDL2Destructor>(
    SDL_CreateRenderer(
      window.get(),
     -1, 0
    )
  );
  if(renderer == nullptr) goto fail;

  return; // return before fail

fail:
  free_resources();
  auto error_msg = std::string("Error - unable to initialize SDL2 in WindowManager(uint32_t, uint32_t) constructor, terminated with: ");
  throw std::runtime_error(error_msg + SDL_GetError());
}

WindowManager::~WindowManager() { free_resources(); }

auto WindowManager::run(void) -> void {
  const auto [width, height] = dimensions;

  constexpr int16_t min_bound = 0;
  constexpr int16_t max_bound = 100;
  constexpr uint32_t number_count = max_bound - min_bound; // + 1?

  constexpr uint32_t FPS = 300;

  const double scale_x = (double) width  / (double) number_count;
  const double scale_y = (double) height / (double) number_count;

  SDL_RenderSetScale(renderer.get(), scale_x, scale_y);

  auto sort = sort::Sort(min_bound, max_bound, number_count);

  // delta time
  uint64_t curr_time = SDL_GetPerformanceCounter();
  uint64_t prev_time = 0;
  double delta_time = 0.0;

  auto keep_alive = true;
  while(keep_alive) {
    // delta time
    prev_time = curr_time;
    curr_time = SDL_GetPerformanceCounter();
    delta_time = ((double) (curr_time - prev_time) * 1000.0 / (double) SDL_GetPerformanceFrequency());

    sort.step(delta_time);

    // event queue
    SDL_Event event{};
    while(SDL_PollEvent(&event) > 0) {
      switch(event.type) {
      case SDL_QUIT:
        keep_alive = false;
        break;
      
      case SDL_KEYDOWN:
        handle_sdl_keydown(event.key.keysym.sym, keep_alive);
        break;

      default: {}
      }
    }

    // rendering
    (void) SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    (void) SDL_RenderClear(renderer.get());

    sort.render(renderer.get());

    (void) SDL_RenderPresent(renderer.get());

    (void) SDL_Delay(1000.0 / FPS);
  }
}

auto WindowManager::free_resources(void) -> void { SDL_Quit(); }

}

// file local functions

static auto handle_sdl_keydown(SDL_Keycode key, bool& keep_alive) -> void {
  switch(key) {
  case SDLK_ESCAPE:
    keep_alive = false;
    break;

  default: {}
  }
}