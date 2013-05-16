#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "mini-printf.h"
#include "decimal_time.h"

#define MY_UUID { 0xFC, 0x6A, 0x4A, 0x8B, 0x6A, 0xE3, 0x4B, 0x02, 0x99, 0xA5, 0xF8, 0x7A, 0x37, 0x73, 0xCB, 0x99 }
PBL_APP_INFO(MY_UUID, "Simply Decimal", "Vuzzbox", 1, 0 /* App version */, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

Window window;

TextLayer text_date_layer;
TextLayer text_time_layer;
TextLayer text_dt_layer;

Layer line_layer;

AppTimerHandle timer_handle;

#define COOKIE_DECIMAL_TIME_TIMER 1
#define DECIMAL_TIME_CONVERSION_MULTIPLIER 1.157
// current time zone offset 
#define TIME_ZONE_OFFSET 0

void line_layer_update_callback(Layer *me, GContext* ctx) {
  (void)me;

  graphics_context_set_stroke_color(ctx, GColorWhite);

  graphics_draw_line(ctx, GPoint(8, 97), GPoint(131, 97));
  graphics_draw_line(ctx, GPoint(8, 98), GPoint(131, 98));

}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Simply");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);

  text_layer_init(&text_date_layer, window.layer.frame);
  text_layer_set_text_color(&text_date_layer, GColorWhite);
  text_layer_set_background_color(&text_date_layer, GColorClear);
  layer_set_frame(&text_date_layer.layer, GRect(8, 68, 144-8, 168-68));
  text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_date_layer.layer);

  text_layer_init(&text_time_layer, window.layer.frame);
  text_layer_set_text_color(&text_time_layer, GColorWhite);
  text_layer_set_background_color(&text_time_layer, GColorClear);
  layer_set_frame(&text_time_layer.layer, GRect(7, 92, 144-7, 168-92));
  text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
  layer_add_child(&window.layer, &text_time_layer.layer);

  text_layer_init(&text_dt_layer, window.layer.frame);
  text_layer_set_text_color(&text_dt_layer, GColorWhite);
  text_layer_set_background_color(&text_dt_layer, GColorClear);
  layer_set_frame(&text_dt_layer.layer, GRect(110, 120, 144-110, 168-120));
  text_layer_set_font(&text_dt_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(&window.layer, &text_dt_layer.layer);

  layer_init(&line_layer, window.layer.frame);
  line_layer.update_proc = &line_layer_update_callback;
  layer_add_child(&window.layer, &line_layer);

  timer_handle = app_timer_send_event(ctx, 10 /* milliseconds */, COOKIE_DECIMAL_TIME_TIMER);

  // TODO: Update display here to avoid blank display on launch?
}

void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
  (void)ctx;
  (void)handle;

  if (cookie == COOKIE_DECIMAL_TIME_TIMER) {
    PblDeciTm t;
    static char time_string[8];

    get_decimal_time(&t);

    memset(&time_string[0], 0, sizeof(time_string));
    snprintf(time_string, 8, "%d:%02d", t.tm_hour, t.tm_min);

    text_layer_set_text(&text_time_layer, time_string);
    text_layer_set_text(&text_dt_layer, "dt");

    // Call it again...and again...and again...
    app_timer_send_event(ctx, 50 /* milliseconds */, COOKIE_DECIMAL_TIME_TIMER);
  }
}

void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  // Need to be static because they're used by the system later.
  static char date_text[] = "Xxxxxxxxx 00";

  // TODO: Only update the date when it's changed.
  string_format_time(date_text, sizeof(date_text), "%B %e", t->tick_time);
  text_layer_set_text(&text_date_layer, date_text);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .timer_handler = &handle_timer,
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    },
  };
  app_event_loop(params, &handlers);
}
