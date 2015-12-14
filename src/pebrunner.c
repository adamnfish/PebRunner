#include <pebble.h>

static Window *window;
static TextLayer *rand_text_layer;
static TextLayer *click_count_text_layer;
static TextLayer *time_text_layer;
static TextLayer *up_help_text_layer;
static TextLayer *click_help_text_layer;
static TextLayer *down_help_text_layer;
static GFont netrunner_font;
static int click_count = 0;
static ActionBarLayer *action_bar;
static GBitmap *die_icon;
static GBitmap *click_icon;
static GBitmap *new_turn_icon;
static bool corpTurn = true;
static unsigned int start_time = 0;
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling SELECT click");
  static char buffer[2];
  int result = (rand() % 5) + 1;
  snprintf(buffer, sizeof(buffer), "%u", result);
  text_layer_set_text(rand_text_layer, buffer);
  hide_help_text();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling UP click");
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling DOWN click");
  static char click_buffer[32] = "";
  text_layer_set_text(rand_text_layer, "");
  int i;
  click_buffer[0] = 0;
  click_count++;
  if (click_count > 9) {
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

static void update_game_time() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating game time");
  static char buffer[] = "000:00";
  unsigned int current_time = time(NULL);
  unsigned int seconds = (current_time - start_time) % 60;
  unsigned int minutes = (current_time - start_time - seconds) / 60;
  snprintf(buffer, sizeof(buffer), "%d:%02d", minutes, seconds);
  text_layer_set_text(time_text_layer, buffer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Game time updated");
}

static void tick_handler(struct tm *tick_time, TimeUnits time_changed) {
  update_game_time();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  netrunner_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_NETRUNNER_35));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Custom font loaded");

  // displays the click counter
  click_count_text_layer = text_layer_create((GRect) { .origin = { 3, 5 }, .size = { 110, 120 } });
  text_layer_set_background_color(click_count_text_layer, GColorClear);
  text_layer_set_text(click_count_text_layer, "");
  text_layer_set_text_alignment(click_count_text_layer, GTextAlignmentCenter);
  text_layer_set_font(click_count_text_layer, netrunner_font);
  text_layer_set_overflow_mode(click_count_text_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(click_count_text_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Click counter layer setup");

  // displays the "hand access" random numbers
  #ifdef PBL_ROUND
  rand_text_layer = text_layer_create((GRect) { .origin = { 75, 75 }, .size = { 30, 30 } });
  #else
  rand_text_layer = text_layer_create((GRect) { .origin = { 77, 125 }, .size = { 30, 30 } });
  #endif
  text_layer_set_background_color(rand_text_layer, GColorClear);
  text_layer_set_text_alignment(rand_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(rand_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(rand_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(rand_text_layer, "");
  layer_add_child(window_layer, text_layer_get_layer(rand_text_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Random number layer setup");

  // displays the game time
  #ifdef PBL_ROUND
  time_text_layer = text_layer_create((GRect) { .origin = { 50, 130 }, .size = { 80, 30 } });
  #else
  time_text_layer = text_layer_create((GRect) { .origin = { 0, 125 }, .size = { 70, 30 } });
  #endif
  text_layer_set_background_color(time_text_layer, GColorClear);
  text_layer_set_text_alignment(time_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(time_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(time_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(time_text_layer, "0:00");
  layer_add_child(window_layer, text_layer_get_layer(time_text_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Game time layer setup");

  // actionbar shows user the available functionality
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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Actionbar layer setup");

  // add coloured actionbar and icons with background colours
  #ifdef PBL_COLOR
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting up colours");
  window_set_background_color(window, GColorBabyBlueEyes);
  action_bar_layer_set_background_color(action_bar, GColorOxfordBlue);
  die_icon_runner = gbitmap_create_with_resource(RESOURCE_ID_DIE_RED);
  click_icon_runner = gbitmap_create_with_resource(RESOURCE_ID_CLICKICON_RED);
  new_turn_icon_runner = gbitmap_create_with_resource(RESOURCE_ID_NEWTURN_RED);
  #endif

  // line up help text with action bar on different platforms
  #ifdef PBL_ROUND
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Lining up resources for Chalk");
  up_help_text_layer = text_layer_create((GRect) { .origin = { 25, 47 }, .size = { 107, 30 } });
  click_help_text_layer = text_layer_create((GRect) { .origin = { 25, 77 }, .size = { 107, 30 } });
  down_help_text_layer = text_layer_create((GRect) { .origin = { 25, 107 }, .size = { 107, 30 } });
  #else
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Lining up resources for rectangle screens");
  up_help_text_layer = text_layer_create((GRect) { .origin = { 10, 20 }, .size = { 98, 30 } });
  click_help_text_layer = text_layer_create((GRect) { .origin = { 10, 71 }, .size = { 98, 30 } });
  down_help_text_layer = text_layer_create((GRect) { .origin = { 10, 122 }, .size = { 98, 30 } });
  #endif

  // help text layers are aligned with action bar buttons
  text_layer_set_font(up_help_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(up_help_text_layer, GColorClear);
  text_layer_set_text(up_help_text_layer, "New turn");
  text_layer_set_overflow_mode(up_help_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(up_help_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(up_help_text_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Help text UP layer setup");

  text_layer_set_font(click_help_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(click_help_text_layer, GColorClear);
  text_layer_set_text(click_help_text_layer, "Random number");
  text_layer_set_overflow_mode(click_help_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(click_help_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(click_help_text_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Help text SELECT layer setup");

  text_layer_set_font(down_help_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(down_help_text_layer, GColorClear);
  text_layer_set_text(down_help_text_layer, "Click");
  text_layer_set_overflow_mode(down_help_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(down_help_text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(down_help_text_layer));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Help text DOWN layer setup");
}

static void window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloading window");

  text_layer_destroy(rand_text_layer);
  text_layer_destroy(click_count_text_layer);
  fonts_unload_custom_font(netrunner_font);

  APP_LOG(APP_LOG_LEVEL_INFO, "Window unloaded");
}

static void init(void) {
  start_time = time(NULL);
  srand(start_time);

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Window created: %p", window);

  // register tick handler
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick handler registered");

  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_INFO, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
