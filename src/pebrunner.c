#include <pebble.h>

static Window *window;
static TextLayer *rand_text_layer;
static TextLayer *click_count_text_layer;
static GFont *netrunner_font;
static int click_count = 0;
static ActionBarLayer *action_bar;
static GBitmap *die_icon;
static GBitmap *click_icon;
static GBitmap *new_turn_icon;
static bool corpTurn = true;


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  static char buffer[2];
  int result = (rand() % 5) + 1;
  snprintf(buffer, sizeof(buffer), "%u", result);
  text_layer_set_text(rand_text_layer, buffer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  click_count = 0;
  corpTurn = !corpTurn;
  text_layer_set_text(rand_text_layer, "New turn");
  text_layer_set_text(click_count_text_layer, "");
  #ifdef PBL_COLOR
  if (corpTurn) {
    window_set_background_color(window, GColorBabyBlueEyes);
    action_bar_layer_set_background_color(action_bar, GColorOxfordBlue);
  } else {
    window_set_background_color(window, GColorMelon);
    action_bar_layer_set_background_color(action_bar, GColorBulgarianRose);
  }
  #endif
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  static char click_buffer[32] = "";
  text_layer_set_text(rand_text_layer, "");
  int i;
  click_buffer[0] = 0;
  click_count++;
  if (click_count > 6) {
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

  rand_text_layer = text_layer_create((GRect) { .origin = { 10, 10 }, .size = { 80, 90 } });
  text_layer_set_background_color(rand_text_layer, GColorClear);
  text_layer_set_text_alignment(rand_text_layer, GTextAlignmentRight);
  text_layer_set_overflow_mode(rand_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(rand_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(rand_text_layer, "New turn");
  layer_add_child(window_layer, text_layer_get_layer(rand_text_layer));

  click_count_text_layer = text_layer_create((GRect) { .origin = { 3, 52 }, .size = { 110, 80 } });
  text_layer_set_background_color(click_count_text_layer, GColorClear);
  text_layer_set_text(click_count_text_layer, "");
  text_layer_set_text_alignment(click_count_text_layer, GTextAlignmentCenter);
  text_layer_set_font(click_count_text_layer, netrunner_font);
  text_layer_set_overflow_mode(click_count_text_layer, GTextOverflowModeWordWrap);

  layer_add_child(window_layer, text_layer_get_layer(click_count_text_layer));

  die_icon = gbitmap_create_with_resource(RESOURCE_ID_DIE);
  click_icon = gbitmap_create_with_resource(RESOURCE_ID_CLICKICON);
  new_turn_icon = gbitmap_create_with_resource(RESOURCE_ID_NEWTURN);

  action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(action_bar, window);
  action_bar_layer_set_click_config_provider(action_bar,
					     click_config_provider);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, new_turn_icon);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, die_icon);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, click_icon);

  #ifdef PBL_COLOR
  window_set_background_color(window, GColorBabyBlueEyes);
  action_bar_layer_set_background_color(action_bar, GColorOxfordBlue);
  #endif
}

static void window_unload(Window *window) {
  text_layer_destroy(rand_text_layer);
  text_layer_destroy(click_count_text_layer);
  fonts_unload_custom_font(netrunner_font);
}

static void init(void) {
  srand(time(NULL));

  window = window_create();
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
