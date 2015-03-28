////////////////////
//	
//	Built by @kieran_gould, @Half_Shot, and @RichardR (Chelmsford Makerspace)
//	Made in roughly two days for @hackessex
//
////////////////////

#include <pebble.h>

Window *window1;
TextLayer *text_layer;

void handle_init(void) {
  window1 = window_create();
	window_set_fullscreen(window1, true);
	window_set_background_color(window1, GColorBlack);

  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(window1, true);
}

void handle_deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(window1);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
