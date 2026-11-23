/*
 * Show the host's Caps Lock state on the RGB strip.
 * SPDX-License-Identifier: MIT
 */

#include <dt-bindings/zmk/hid_indicators.h>
#include <zmk/events/hid_indicators_changed.h>
#include <zmk/event_manager.h>
#include <zmk/hid_indicators_types.h>
#include <zmk/rgb_underglow.h>
#include <zephyr/sys/util.h>

static void update_caps_indicator(zmk_hid_indicators_t indicators)
{
    if (indicators & HID_INDICATOR_CAPS_LOCK) {
        zmk_rgb_underglow_set_hsb((struct zmk_led_hsb){
            .h = 0,
            .s = 0,
            .b = CONFIG_ZMK_RGB_UNDERGLOW_BRT_MAX,
        });
        zmk_rgb_underglow_select_effect(0);
        zmk_rgb_underglow_on();
    } else {
        zmk_rgb_underglow_off();
    }
}

static int caps_indicator_listener(const zmk_event_t *eh)
{
    const struct zmk_hid_indicators_changed *event = as_zmk_hid_indicators_changed(eh);
    update_caps_indicator(event->indicators);
    return ZMK_EV_EVENT_HANDLED;
}

ZMK_LISTENER(caps_indicator, caps_indicator_listener);
ZMK_SUBSCRIPTION(caps_indicator, zmk_hid_indicators_changed);
