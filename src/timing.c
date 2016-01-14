static char timingRun[] = "Run\n"
  "1. Draw phase\n"
  "1.1. Pay, Rez, Score\n"
  "1.2. Turn begins\n"
  "1.3. Draw one card\n"
  "2. Action phase\n"
  "2.1. Pay, Rez ,Score\n"
  "2.2. Take actions\n"
  "2.2.1. Pay, Rez ,Score\n"
  "3. Discard phase\n"
  "3.1. Discard to max hand size\n"
  "3.2. Pay, Rez\n"
  "3.3. End of turn";

static char timingCorp[] = "Corp turn\n"
  "1. Draw phase\n"
  "1.1. Pay, Rez, Score\n"
  "1.2. Turn begins\n"
  "1.3. Draw one card\n"
  "2. Action phase\n"
  "2.1. Pay, Rez ,Score\n"
  "2.2. Take actions\n"
  "2.2.1. Pay, Rez ,Score\n"
  "3. Discard phase\n"
  "3.1. Discard to max hand size\n"
  "3.2. Pay, Rez\n"
  "3.3. End of turn";

static char timingRunner[] = "Runner turn\n"
  "1. Action phase\n"
  "1.1. Pay, Rez\n"
  "1.2. Turn begins\n"
  "1.3. Pay, Rez\n"
  "1.4. Take actions\n"
  "1.4.1. Pay, Rez\n"
  "2. Discard phase\n"
  "2.1. Discard to max hand size\n"
  "2.2. Pay, Rez\n"
  "2.3. End of turn";

static Window *timing_structure_details_window;
static TextLayer *timing_structure_details_text_layer;

static SimpleMenuLayer *timing_structure_menu_layer;

static SimpleMenuItem timing_structure_items[3];
static SimpleMenuSection timing_structure_sections[1];

static void show_timing_structure_text(char text[]) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Showing timing structure text");

  menu_window = window_create();
  window_set_window_handlers(menu_window, (WindowHandlers) {
    .load = timing_structure_menu_window_load,
    .unload = timing_structure_menu_window_unload,
  });
  // push menu window onto stack to display timing structure choice
  window_stack_push(menu_window, true);
}

static void timing_structure_select_callback(int index, void *ctx) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Selected timing structure %u", int);

  if (0 == int) {
    show_timing_structure_text(timingRun);
  } else if (1 == int) {
    show_timing_structure_text(timingCorp);
  }
  } else if (2 == int) {
    show_timing_structure_text(timingRunner);
  }
}

void set_window_handler(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Displaying timing structure menu");

  timing_structure_items[0] = (SimpleMenuItem) {
    .title = "Run",
    .callback = timing_structure_select_callback,
  };
  timing_structure_items[1] = (SimpleMenuItem) {
    .title = "Corp turn",
    .callback = timing_structure_select_callback,
  };
  timing_structure_items[2] = (SimpleMenuItem) {
    .title = "Runner turn",
    .callback = timing_structure_select_callback,
  };

  timing_structure_sections[0] = (SimpleMenuSection) {
    .title = "Timing structure references"
    .num_items = 3,
    .items = timing_structure_items,
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  timing_structure_menu_layer = simple_menu_layer_create(bounds, window, timing_structure_sections, 1, NULL);
  layer_add_child(window, simple_menu_layer_get_layer(timing_structure_menu_layer));
}

static void timing_structure_menu_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Unloading timing structure menu window");

  simple_menu_layer_destroy(psi_mode_menu_layer);
  window_destroy(psi_mode_window);
}

// Used in pebrunner.c as entry point to the timing structure UI
void show_timing_structure_menu() {
  APP_LOG(APP_LOG_LEVEL_INFO, "Showing timing structure menu");

  menu_window = window_create();
  window_set_window_handlers(menu_window, (WindowHandlers) {
    .load = timing_structure_menu_window_load,
    .unload = timing_structure_menu_window_unload,
  });
  // push menu window onto stack to display timing structure choice
  window_stack_push(menu_window, true);
}
