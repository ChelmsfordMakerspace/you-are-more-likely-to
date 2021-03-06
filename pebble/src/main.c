////////////////////
//	
//	Built by @kieran_gould, @Half_Shot, and @RichardR (Chelmsford Makerspace)
//	Made in roughly two days for @hackessex
//
//	I'm making a note here: HUGE SUCCESS. - GLaDOS
//
////////////////////

#include <pebble.h>

char fact_buffer[256];
char times_buffer[16];

enum {
	KEY_FACT = 0,
	KEY_TIMES = 1,
};
	
Window *window;
GBitmap *load_bitmap;
BitmapLayer *load_layer;
TextLayer *youare_layer, *times_layer, *morelikely_layer, *fact_layer, *credit_layer;

void on_animation_stopped(Animation *anim, bool finished, void *context)
{
    //Free the memory used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay)
{
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 
    //Set stopped handler to free memory
    AnimationHandlers handlers = {
        //The reference to the stopped handler is the only one in the array
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
 
    //Start animation!
    animation_schedule((Animation*) anim);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{

}

void window_load(Window *window)
{
  ResHandle font_big = resource_get_handle(RESOURCE_ID_FONT_MAIN_BOLD_35); 	// Title font
	ResHandle font_small = resource_get_handle(RESOURCE_ID_FONT_MAIN_BOLD_9);		// Credit font
	ResHandle font_normal = resource_get_handle(RESOURCE_ID_FONT_MAIN_BOLD_18);			// Fact font
	load_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOADING);
		
	// You Are layer
  youare_layer = text_layer_create(GRect(2, 2, 144, 168));
  text_layer_set_background_color(youare_layer, GColorClear);
  text_layer_set_text_color(youare_layer, GColorWhite);	
	text_layer_set_text_alignment(youare_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(youare_layer));
	text_layer_set_font(youare_layer, fonts_load_custom_font(font_normal));
  text_layer_set_text(youare_layer, "You Are...");
	
	// Times layer
  times_layer = text_layer_create(GRect(2, 20, 144, 50));
  text_layer_set_background_color(times_layer, GColorClear);
  text_layer_set_text_color(times_layer, GColorWhite);	
	text_layer_set_text_alignment(times_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(times_layer));
	text_layer_set_font(times_layer, fonts_load_custom_font(font_big));
  text_layer_set_text(times_layer, "Loading");
	
	// More Likely To layer
  morelikely_layer = text_layer_create(GRect(2, 60, 144, 168));
  text_layer_set_background_color(morelikely_layer, GColorClear);
  text_layer_set_text_color(morelikely_layer, GColorWhite);	
	text_layer_set_text_alignment(morelikely_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(morelikely_layer));
	text_layer_set_font(morelikely_layer, fonts_load_custom_font(font_normal));
  text_layer_set_text(morelikely_layer, "More likely to...");
	
	// Fact layer
  fact_layer = text_layer_create(GRect(2, 79, 144, 168));
  text_layer_set_background_color(fact_layer, GColorClear);
  text_layer_set_text_color(fact_layer, GColorWhite);	
	text_layer_set_text_alignment(fact_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(fact_layer));
	text_layer_set_font(fact_layer, fonts_load_custom_font(font_normal));
  text_layer_set_text(fact_layer, "Please wait!");
	
	// Credit layer
	credit_layer = text_layer_create(GRect(0, 156, 144, 168));
  text_layer_set_background_color(credit_layer, GColorClear);
  text_layer_set_text_color(credit_layer, GColorWhite);	
	text_layer_set_text_alignment(credit_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(credit_layer));
	text_layer_set_font(credit_layer, fonts_load_custom_font(font_small));
  text_layer_set_text(credit_layer, "@hackessex | @CM_Makerspace");
	
	// Loading image
	load_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(load_layer, load_bitmap);
	bitmap_layer_set_background_color(load_layer, GColorBlack);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(load_layer));
		
	struct tm *t;
	time_t temp;	
	temp = time(NULL);	
	t = localtime(&temp);	
	
	tick_handler(t, MINUTE_UNIT);
}
 
void window_unload(Window *window)
{
  text_layer_destroy(youare_layer);
	text_layer_destroy(morelikely_layer);
	text_layer_destroy(fact_layer);
	text_layer_destroy(times_layer);
	text_layer_destroy(credit_layer);
	bitmap_layer_destroy(load_layer);
	gbitmap_destroy(load_bitmap);
}
 
void process_tuple(Tuple *t)
{
	//Get key
	int key = t->key;

	//Get integer value, if present
	int value = t->value->int32;

	//Get string value, if present
	char string_value[256];
	strcpy(string_value, t->value->cstring);

	//Decide what to do
	switch(key) {
		case KEY_FACT:
			//Fact received
			snprintf(fact_buffer, sizeof("X")*256, "%s", string_value);
			text_layer_set_text(fact_layer, (char*) &fact_buffer);
			break;
		case KEY_TIMES:
			//Times received
			snprintf(times_buffer, sizeof("X")*16, "%dx", value);
			text_layer_set_text(times_layer, (char*) &times_buffer);
			// Animate out the loading screen
			GRect start = GRect(0, 0, 144, 168);
  		GRect finish = GRect(0, 336, 144, 168);
  		animate_layer(bitmap_layer_get_layer(load_layer), &start, &finish, 1000, 0);
			break;
	}
}

static void in_received_handler(DictionaryIterator *iter, void *context) 
{
	(void) context;
	
	//Get data
	Tuple *t = dict_read_first(iter);
	while(t != NULL)
	{
		process_tuple(t);
		
		//Get next
		t = dict_read_next(iter);
	}
}

void send_int(uint8_t key, uint8_t cmd)
{
	DictionaryIterator *iter;
 	app_message_outbox_begin(&iter);
 	
 	Tuplet value = TupletInteger(key, cmd);
 	dict_write_tuplet(iter, &value);
 	
 	app_message_outbox_send();
}

void tick_callback(struct tm *tick_time, TimeUnits units_changed)
{
	//Every ten minutes
	if(tick_time->tm_min % 10 == 0)
	{
		//Send an arbitrary message, the response will be handled by in_received_handler()
		send_int(5, 5);
	}
}

void init()
{
	window = window_create();
	WindowHandlers handlers = {
		.load = window_load,
		.unload = window_unload
	};
	window_set_window_handlers(window, handlers);

	//Register AppMessage events
	app_message_register_inbox_received(in_received_handler);					 
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());		//Largest possible input and output buffer sizes
	
	//Register to receive minutely updates
	tick_timer_service_subscribe(MINUTE_UNIT, tick_callback);

	window_set_fullscreen(window, true);
	window_set_background_color(window, GColorBlack);
	window_stack_push(window, true);
}

void deinit()
{
	tick_timer_service_unsubscribe();
  window_destroy(window);
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}
