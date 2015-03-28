////////////////////
//	
//	Built by @kieran_gould, @Half_Shot, and @RichardR (Chelmsford Makerspace)
//	Made in roughly two days for @hackessex
//
////////////////////

#include <pebble.h>

char fact_buffer[128];

enum {
	KEY_FACT = 0,
};
	
Window *window;
TextLayer *title_layer, *fact_layer, *credit_layer;

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{

}

void window_load(Window *window)
{
  ResHandle font_bold = resource_get_handle(RESOURCE_ID_FONT_MAIN_BOLD_20); 	// Title font
	ResHandle font_small = resource_get_handle(RESOURCE_ID_FONT_MAIN_BOLD_9);		// Credit font
	ResHandle font_normal = resource_get_handle(RESOURCE_ID_FONT_MAIN_18);			// Fact font
	
	// Title layer
  title_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(title_layer, GColorClear);
  text_layer_set_text_color(title_layer, GColorWhite);	
	text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(title_layer));
	text_layer_set_font(title_layer, fonts_load_custom_font(font_bold));
  text_layer_set_text(title_layer, "You Are More Likely To...");
	
	// Credit layer
	credit_layer = text_layer_create(GRect(0, 156, 144, 168));
  text_layer_set_background_color(credit_layer, GColorClear);
  text_layer_set_text_color(credit_layer, GColorWhite);	
	text_layer_set_text_alignment(credit_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(credit_layer));
	text_layer_set_font(credit_layer, fonts_load_custom_font(font_small));
  text_layer_set_text(credit_layer, "@hackessex | @CM_Makerspace");
	
	// Fact layer
	fact_layer = text_layer_create(GRect(0, 50, 144, 100));
  text_layer_set_background_color(fact_layer, GColorClear);
  text_layer_set_text_color(fact_layer, GColorWhite);	
	text_layer_set_text_alignment(fact_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(fact_layer));
	text_layer_set_font(fact_layer, fonts_load_custom_font(font_normal));
  text_layer_set_text(fact_layer, "LOADING...");
	
	struct tm *t;
	time_t temp;	
	temp = time(NULL);	
	t = localtime(&temp);	
	
	tick_handler(t, MINUTE_UNIT);
}
 
void window_unload(Window *window)
{
  text_layer_destroy(title_layer);
	text_layer_destroy(credit_layer);
	text_layer_destroy(fact_layer);
}
 
void process_tuple(Tuple *t)
{
	//Get key
	int key = t->key;

	//Get integer value, if present
	int value = t->value->int32;

	//Get string value, if present
	char string_value[32];
	strcpy(string_value, t->value->cstring);

	//Decide what to do
	switch(key) {
		case KEY_FACT:
			//Temperature received
			snprintf(fact_buffer, sizeof("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"), "%dF", value);
			text_layer_set_text(fact_layer, (char*) &fact_buffer);
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
	//Every five minutes
	if(tick_time->tm_min % 1 == 0)
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
