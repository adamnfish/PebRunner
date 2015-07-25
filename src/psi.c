#include <pebble.h>

// psi-game window
static Window *psi_mode_window;

// runner / corp menu
#define NUM_PSI_MODE_SECTIONS 2
#define NUM_FIRST_PSI_MODE_ITEMS 2
#define NUM_SECOND_PSI_MODE_ITEMS 2
static SimpleMenuLayer *psi_mode_menu_layer;
static SimpleMenuSection psi_mode_sections[NUM_PSI_MODE_SECTIONS];
static SimpleMenuItem first_psi_mode_items[NUM_FIRST_PSI_MODE_ITEMS];
static SimpleMenuItem second_psi_mode_items[NUM_SECOND_PSI_MODE_ITEMS];

// psi-game choice
static NumberWindow *value_choice_window;

// psi-game result display
static Window *psi_result_window;
static TextLayer *psi_result_text_layer;

// psi-game state (set by menu interactions)
static bool corp = true;
static bool siphon = false;
static int psi_game_value = 0;


/* ============= psi-game result display ============== */

static unsigned short int calculate_result() {
  // TODO: real psi-game logic here
  unsigned short int result = 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Calculated psi-game result: %u", result);
  return result;
}

static void psi_result_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading psi-game result window");

  psi_result_text_layer = text_layer_create((GRect) { .origin = { 40, 50 }, .size = { 64, 60 } });
  #ifdef PBL_COLOR
  text_layer_set_background_color(psi_result_text_layer, GColorRed);
  #else
  text_layer_set_background_color(psi_result_text_layer, GColorClear);
  #endif
  text_layer_set_text_color(psi_result_text_layer, GColorWhite);
  text_layer_set_text_alignment(psi_result_text_layer, GTextAlignmentCenter);
  text_layer_set_font(psi_result_text_layer, FONT_KEY_GOTHIC_28);

  static char buffer[1];
  unsigned short int result = calculate_result();
  snprintf(buffer, sizeof(buffer), "%u", result);
  text_layer_set_text(psi_result_text_layer, buffer);

  APP_LOG(APP_LOG_LEVEL_INFO, "Debugging, window: %p, top_window: %p", window, window_stack_get_top_window());

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(psi_result_text_layer));
}

static void psi_result_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Unloading psi-game result window");

  text_layer_destroy(psi_result_text_layer);
  window_destroy(psi_result_window);
}

static void display_psi_result() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting up psi-game result window");

  psi_result_window = window_create();
  window_set_window_handlers(psi_result_window, (WindowHandlers) {
    .load = psi_result_window_load,
    .unload = psi_result_window_unload
  });
  window_stack_push(psi_result_window, true);
}

/* ============= psi-game's value ============== */

static void value_choice_window_select_handler(struct NumberWindow *number_window, void *context) {
  psi_game_value = number_window_get_value(number_window);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling psi-game value: %u", psi_game_value);

  // pop the setup windows from the stack
  //window_stack_remove(number_window_get_window(value_choice_window), false);
  //window_stack_remove(psi_mode_window, false);
  window_stack_pop(false);
  window_stack_pop(false);
  window_destroy(number_window_get_window(value_choice_window));
  // display result in new window
  display_psi_result();
}

static void display_value_choice() {
  // use state to calculate label, min and max
  static int min;
  static int max;
  static char * label;
  if (siphon) {
    min = 1;
    max = 7;
    if (corp) {
      label = "How many credits do you have?";
    } else {
      label = "How many credits does the corp have?";
    }
  } else {
    min = 2;
    max = 10;
    if (corp) {
      label = "what's the value of the runner winning?";
    } else {
      label = "What's the value of winning this psi-game?";
    }
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting up psi-game value window: max: %u, min: %u is_corp: %u", max, min, corp);

  value_choice_window = number_window_create(label, (NumberWindowCallbacks) {
    .incremented = NULL,
    .decremented = NULL,
    .selected = value_choice_window_select_handler
  }, NULL);
  number_window_set_max(value_choice_window, max);
  number_window_set_min(value_choice_window, min);

  window_stack_push(number_window_get_window(value_choice_window), true);
}

/* ============= psi-game mode menu (corp/runner and siphon?) ============== */

/* TODO: add a "same as previous" item to menu */
static void psi_mode_select_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling psi-game mode selection: %u", index);

  // set state based on choice
  switch (index) {
  case 0:
    corp = true;
    siphon = false;
    break;
  case 1:
    corp = false;
    siphon = false;
    break;
  case 2:
    corp = true;
    siphon = true;
    break;
  case 3:
    corp = false;
    siphon = true;
    break;
  }
  // display value chooser window
  display_value_choice();
}

static void display_psi_mode_menu(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Displaying psigame mode menu");

  first_psi_mode_items[0] = (SimpleMenuItem) {
    .title = "Corp",
    .callback = psi_mode_select_callback,
  };
  first_psi_mode_items[1] = (SimpleMenuItem) {
    .title = "Runner",
    .callback = psi_mode_select_callback,
  };
  second_psi_mode_items[0] = (SimpleMenuItem) {
    .title = "Corp",
    .callback = psi_mode_select_callback,
  };
  second_psi_mode_items[1] = (SimpleMenuItem) {
    .title = "Runner",
    .callback = psi_mode_select_callback,
  };
  psi_mode_sections[0] = (SimpleMenuSection) {
    .title = "Your side",
    .num_items = NUM_FIRST_PSI_MODE_ITEMS,
    .items = first_psi_mode_items,
  };
  psi_mode_sections[1] = (SimpleMenuSection) {
    .title = "Your side (account siphon)",
    .num_items = NUM_SECOND_PSI_MODE_ITEMS,
    .items = second_psi_mode_items,
  };
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  psi_mode_menu_layer = simple_menu_layer_create(bounds, window, psi_mode_sections, NUM_PSI_MODE_SECTIONS, NULL);

  // set to previous selection when it opens
  if (corp) {
    simple_menu_layer_set_selected_index(psi_mode_menu_layer, 0, false);
  } else {
    simple_menu_layer_set_selected_index(psi_mode_menu_layer, 1, false);
  }

  // display the menu
  layer_add_child(window_layer, simple_menu_layer_get_layer(psi_mode_menu_layer));
}

/* ============= psi-game mode initial window ============== */

static void psi_mode_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading psi-game mode window");

  display_psi_mode_menu(window);
}

static void psi_mode_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloading psi-game mode window");

  simple_menu_layer_destroy(psi_mode_menu_layer);
  window_destroy(psi_mode_window);
}

// called from pebrunner.c to invoke the psi-game wizard
void show_psi_menu() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Invoking psi-game wizard");

  psi_mode_window = window_create();
  window_set_window_handlers(psi_mode_window, (WindowHandlers) {
    .load = psi_mode_window_load,
    .unload = psi_mode_window_unload,
  });

  // push psi window onto stack to start up psi-game wizard
  window_stack_push(psi_mode_window, true);
}
