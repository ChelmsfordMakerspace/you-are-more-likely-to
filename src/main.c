////////////////////
//	
//	Built by @kieran_gould, @Half_Shot, and @RichardR (Chelmsford Makerspace)
//	Made in roughly two days for @hackessex
//
////////////////////

#include <pebble.h>

Window *window;
TextLayer *title_layer, *fact_layer, *credit_layer;

void window_load(Window *window)
{
  ResHandle font_bold = resource_get_handle(RESOURCE_ID_FONT_MAIN_BOLD_20);
	
  title_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(title_layer, GColorClear);
  text_layer_set_text_color(title_layer, GColorWhite);	
	text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_layer));
	text_layer_set_font(title_layer, fonts_load_custom_font(font_bold));
  text_layer_set_text(title_layer, "You Are More Likely To...");
}
 
void window_unload(Window *window)
{
  //We will safely destroy the Window's elements here!
  text_layer_destroy(title_layer);
}
 
void init()
{
  //Initialize the app elements here!
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
	
	window_set_background_color(window, GColorBlack);
	window_set_fullscreen(window, true);
  window_stack_push(window, true);
}
 
void deinit()
{
  //De-initialize elements here to save memory!
  window_destroy(window);
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}
