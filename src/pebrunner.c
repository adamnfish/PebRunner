#include <pebble.h>

static Window *window;
static TextLayer *rand_text_layer;
static TextLayer *click_count_text_layer;
static GFont *netrunner_font;
static GFont *random_font;
static int click_count = 0;


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  static char buffer[2];
  int result = (rand() % 5) + 1;
  snprintf(buffer, sizeof(buffer), "%u", result);
  text_layer_set_text(rand_text_layer, buffer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  click_count = 0;
  text_layer_set_text(rand_text_layer, "New turn");
  text_layer_set_text(click_count_text_layer, "");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  static char click_buffer[32] = "";
  text_layer_set_text(rand_text_layer, "");
  int i;
  click_buffer[0] = 0;
  click_count++;
  if (click_count > 8) {
    click_count = 1;
  }
  for (i = 0; i < click_count; i++) {
    strcat(click_buffer, "");
  }
  text_layer_set_text(click_count_text_layer, click_buffer);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  netrunner_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_NETRUNNER_35));
  random_font = fonts_get_system_font(FONT_KEY_GOTHIC_24);

  rand_text_layer = text_layer_create((GRect) { .origin = { 0, 10 }, .size = { bounds.size.w, 90 } });
  text_layer_set_background_color(rand_text_layer, GColorClear);
  text_layer_set_text_alignment(rand_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(rand_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(rand_text_layer, random_font);
  text_layer_set_text(rand_text_layer, "Down for click\nUp for new turn\nSelect for random #");
  layer_add_child(window_layer, text_layer_get_layer(rand_text_layer));

  click_count_text_layer = text_layer_create((GRect) { .origin = { 0, 52 }, .size = { bounds.size.w, 100 } });
  text_layer_set_background_color(click_count_text_layer, GColorClear);
  text_layer_set_text(click_count_text_layer, "");
  text_layer_set_text_alignment(click_count_text_layer, GTextAlignmentCenter);
  text_layer_set_font(click_count_text_layer, netrunner_font);
  text_layer_set_overflow_mode(click_count_text_layer, GTextOverflowModeWordWrap);

  layer_add_child(window_layer, text_layer_get_layer(click_count_text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(rand_text_layer);
}

static void init(void) {
  srand(time(NULL));

  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
