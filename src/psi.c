#include <pebble.h>

// psi-game window
static Window *psi_window;

static TextLayer *tmp_text_layer;


/* ============= psi-game wizard window ============== */

static void psi_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  // setup psi window here
  tmp_text_layer = text_layer_create((GRect) { .origin = { 20, 25 }, .size = { 114, 60 } });
  text_layer_set_background_color(tmp_text_layer, GColorClear);
  text_layer_set_text_alignment(tmp_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(tmp_text_layer, GTextOverflowModeWordWrap);
  text_layer_set_font(tmp_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(tmp_text_layer, "Psi game");
  layer_add_child(window_layer, text_layer_get_layer(tmp_text_layer));
}

static void psi_window_unload(Window *window) {
  // destroy things here
  text_layer_destroy(tmp_text_layer);

  window_destroy(psi_window);
}

// called from pebrunner.c to invoke the psi-game wizard
void show_psi_menu() {
  psi_window = window_create();
  window_set_window_handlers(psi_window, (WindowHandlers) {
    .load = psi_window_load,
    .unload = psi_window_unload,
  });

  // push psi window onto stack
  window_stack_push(psi_window, true);
}
