#include <pebble.h>

static Window *window;
static TextLayer *rand_text_layer;
static TextLayer *click_count_text_layer;
static TextLayer *up_help_text_layer;
static TextLayer *click_help_text_layer;
static TextLayer *down_help_text_layer;
static GFont *netrunner_font;
static int click_count = 0;
static ActionBarLayer *action_bar;
static GBitmap *die_icon;
static GBitmap *click_icon;
static GBitmap *new_turn_icon;
static bool corpTurn = true;
#ifdef PBL_COLOR
static GBitmap *die_icon_runner;
static GBitmap *click_icon_runner;
static GBitmap *new_turn_icon_runner;
#endif


static void show_help_text() {
  layer_set_hidden((Layer *)up_help_text_layer, false);
  layer_set_hidden((Layer *)click_help_text_layer, false);
  layer_set_hidden((Layer *)down_help_text_layer, false);
}

static void hide_help_text() {
  layer_set_hidden((Layer *)up_help_text_layer, true);
  layer_set_hidden((Layer *)click_help_text_layer, true);
  layer_set_hidden((Layer *)down_help_text_layer, true);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  static char buffer[2];
  int result = (rand() % 5) + 1;
  snprintf(buffer, sizeof(buffer), "%u", result);
  text_layer_set_text(rand_text_layer, buffer);
  hide_help_text();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  click_count = 0;
  corpTurn = !corpTurn;
  text_layer_set_text(rand_text_layer, "");
  text_layer_set_text(click_count_text_layer, "");
  show_help_text();
  #ifdef PBL_COLOR
  if (corpTurn) {
    window_set_background_color(window, GColorBabyBlueEyes);
    action_bar_layer_set_background_color(action_bar, GColorOxfordBlue);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, new_turn_icon);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, die_icon);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, click_icon);
  } else {
    window_set_background_color(window, GColorMelon);
    action_bar_layer_set_background_color(action_bar, GColorBulgarianRose);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, new_turn_icon_runner);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, die_icon_runner);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, click_icon_runner);
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
  hide_help_text();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  #ifdef PBL_SDK_2
  window_set_fullscreen(window, true);
  layer_set_bounds(window_layer, GRect(0, 0, 144, 168));
  #endif

  netrunner_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_NETRUNNER_35));

  rand_text_layer = text_layer_create((GRect) { .origin = { 10, 10 }, .size = { 80, 90 } });
  text_layer_set_background_color(rand_text_layer, GColorClear);
  text_layer_set_text_alignment(rand_text_layer, GTextAlignmentRight);
  text_layer_set_overflow_mode(rand_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(rand_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(rand_text_layer, "");
  layer_add_child(window_layer, text_layer_get_layer(rand_text_layer));

  click_count_text_layer = text_layer_create((GRect) { .origin = { 3, 52 }, .size = { 110, 80 } });
  text_layer_set_background_color(click_count_text_layer, GColorClear);
  text_layer_set_text(click_count_text_layer, "");
  text_layer_set_text_alignment(click_count_text_layer, GTextAlignmentCenter);
  text_layer_set_font(click_count_text_layer, netrunner_font);
  text_layer_set_overflow_mode(click_count_text_layer, GTextOverflowModeWordWrap);

  layer_add_child(window_layer, text_layer_get_layer(click_count_text_layer));

  action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(action_bar, window);
  action_bar_layer_set_click_config_provider(action_bar,
					     click_config_provider);

  die_icon = gbitmap_create_with_resource(RESOURCE_ID_DIE);
  click_icon = gbitmap_create_with_resource(RESOURCE_ID_CLICKICON);
  new_turn_icon = gbitmap_create_with_resource(RESOURCE_ID_NEWTURN);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, new_turn_icon);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, die_icon);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, click_icon);

  #ifdef PBL_COLOR
  window_set_background_color(window, GColorBabyBlueEyes);
  action_bar_layer_set_background_color(action_bar, GColorOxfordBlue);

  die_icon_runner = gbitmap_create_with_resource(RESOURCE_ID_DIE_RED);
  click_icon_runner = gbitmap_create_with_resource(RESOURCE_ID_CLICKICON_RED);
  new_turn_icon_runner = gbitmap_create_with_resource(RESOURCE_ID_NEWTURN_RED);
  #endif

  // line up help text with action bar on different platforms
  #ifdef PBL_SDK_2
  up_help_text_layer = text_layer_create((GRect) { .origin = { 10, 18 }, .size = { 107, 30 } });
  click_help_text_layer = text_layer_create((GRect) { .origin = { 10, 70 }, .size = { 107, 30 } });
  down_help_text_layer = text_layer_create((GRect) { .origin = { 10, 124 }, .size = { 107, 30 } });
  #else
  up_help_text_layer = text_layer_create((GRect) { .origin = { 10, 20 }, .size = { 98, 30 } });
  click_help_text_layer = text_layer_create((GRect) { .origin = { 10, 71 }, .size = { 98, 30 } });
  down_help_text_layer = text_layer_create((GRect) { .origin = { 10, 122 }, .size = { 98, 30 } });
  #endif

  text_layer_set_font(up_help_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(up_help_text_layer, GColorClear);
  text_layer_set_text(up_help_text_layer, "New turn");
  text_layer_set_overflow_mode(up_help_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(up_help_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(up_help_text_layer));

  text_layer_set_font(click_help_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(click_help_text_layer, GColorClear);
  text_layer_set_text(click_help_text_layer, "Random number");
  text_layer_set_overflow_mode(click_help_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(click_help_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(click_help_text_layer));

  text_layer_set_font(down_help_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(down_help_text_layer, GColorClear);
  text_layer_set_text(down_help_text_layer, "Click");
  text_layer_set_overflow_mode(down_help_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(down_help_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(down_help_text_layer));
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
