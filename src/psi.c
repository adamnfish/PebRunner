#include <pebble.h>

// psi-game result display
static Window *psi_result_window;
static TextLayer *psi_result_text_layer;

// psi-game value choice
static Window *psi_value_window;

#define NUM_PSI_VALUE_SECTIONS 1
#define NUM_FIRST_PSI_VALUE_ITEMS 15
static SimpleMenuLayer *psi_value_menu_layer;
static SimpleMenuSection psi_value_sections[NUM_PSI_MODE_SECTIONS];
static SimpleMenuItem first_psi_value_items[NUM_FIRST_PSI_MODE_ITEMS];

#define NUM_PSIPHON_VALUE_SECTIONS 1
#define NUM_FIRST_PSIPHON_VALUE_ITEMS 8
static SimpleMenuLayer *psiphon_value_menu_layer;
static SimpleMenuSection psiphon_value_sections[NUM_PSIPHON_MODE_SECTIONS];
static SimpleMenuItem first_psiphon_value_items[NUM_FIRST_PSIPHON_MODE_ITEMS];

// psi-game mode (runner / corp and siphon)
static Window *psi_mode_window;

#define NUM_PSI_MODE_SECTIONS 2
#define NUM_FIRST_PSI_MODE_ITEMS 2
#define NUM_SECOND_PSI_MODE_ITEMS 2
static SimpleMenuLayer *psi_mode_menu_layer;
static SimpleMenuSection psi_mode_sections[NUM_PSI_MODE_SECTIONS];
static SimpleMenuItem first_psi_mode_items[NUM_FIRST_PSI_MODE_ITEMS];
static SimpleMenuItem second_psi_mode_items[NUM_SECOND_PSI_MODE_ITEMS];

// psi-game state (set by menu interactions)
static bool corp = true;
static bool siphon = false;
static int psi_game_value = 0;

//psi game data (see psi-data.c)
extern unsigned short int oneThresholdsCorp[];
extern unsigned short int twoThresholdsCorp[];
extern unsigned short int oneSiphonThresholdsCorp[];
extern unsigned short int oneThresholdsRunner[];
extern unsigned short int twoThresholdsRunner[];
extern unsigned short int oneSiphonThresholdsRunner[];
extern unsigned short int twoSiphonThresholdsRunner[];

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
  text_layer_set_background_color(psi_result_text_layer, GColorClear);
  text_layer_set_text_color(psi_result_text_layer, GColorBlack);
  text_layer_set_text_alignment(psi_result_text_layer, GTextAlignmentCenter);
  text_layer_set_font(psi_result_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));

  #ifdef PBL_COLOR
  if (corp) {
    window_set_background_color(window, GColorBabyBlueEyes);
  } else {
    window_set_background_color(window, GColorMelon);
  }
  #endif

  static char buffer[2];
  unsigned short int result = calculate_result();
  //  unsigned short int result = (rand() % 2) + 1;
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

static void psi_value_choice_callback(int index, void *context) {
  psi_game_value_index = index;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling psi-game value with index: %u", psi_game_value);

  // display result in new window
  display_psi_result();
  // pop the setup windows from the stack
  window_stack_remove(number_window_get_window(value_choice_window), false);
  window_stack_remove(psi_mode_window, false);
  window_destroy(number_window_get_window(value_choice_window));
}

static void psi_value_menu() {

}

static void psiphon_value_menu() {

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
      label = "How rich are you?";
    } else {
      label = "How many corp credits?";
    }
  } else {
    min = 2;
    max = 10;
    if (corp) {
      label = "Value to runner?";
    } else {
      label = "Value of this run?";
    }
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Setting up psi-game value window: max: %u, min: %u is_corp: %u", max, min, corp);

  psi_value_window = window_create();
  window_set_window_handlers(_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
    .appear = main_window_appear,
    .disappear = main_window_disappear,
  });

  window_stack_push(number_window_get_window(value_choice_window), true);
}

/* ============= psi-game mode menu (corp/runner and siphon?) ============== */

/* TODO: add a "same as previous" item to menu */
static void psi_mode_select_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling psi-game mode selection: %u", index);
  siphon = false;
  // set state based on choice
  switch (index) {
  case 0:
    corp = true;
    break;
  case 1:
    corp = false;
    break;
  }
  // display value chooser window
  display_value_choice();
}

static void psi_mode_select_siphon_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling account siphon psi-game mode selection: %u", index);
  siphon = true;
  // set state based on choice
  switch (index) {
  case 0:
    corp = true;
    break;
  case 1:
    corp = false;
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
    .callback = psi_mode_select_siphon_callback,
  };
  second_psi_mode_items[1] = (SimpleMenuItem) {
    .title = "Runner",
    .callback = psi_mode_select_siphon_callback,
  };
  psi_mode_sections[0] = (SimpleMenuSection) {
    .title = "Your side",
    .num_items = NUM_FIRST_PSI_MODE_ITEMS,
    .items = first_psi_mode_items,
  };
  psi_mode_sections[1] = (SimpleMenuSection) {
    .title = "Account siphon",
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
