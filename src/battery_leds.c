/*
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/devicetree.h>
#include <zephyr/drivers/led.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include <zmk/battery.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static const struct led_dt_spec battery_leds[] = {
    LED_DT_SPEC_GET(DT_NODELABEL(pwm_led0)),
    LED_DT_SPEC_GET(DT_NODELABEL(pwm_led1)),
    LED_DT_SPEC_GET(DT_NODELABEL(pwm_led2)),
};

static void update_battery_leds(uint8_t state_of_charge) {
    for (size_t i = 0; i < ARRAY_SIZE(battery_leds); i++) {
        if (!led_is_ready_dt(&battery_leds[i])) {
            LOG_ERR("Battery LED %u is not ready", i);
            continue;
        }

        const uint8_t brightness = CLAMP(state_of_charge * ARRAY_SIZE(battery_leds) - i * 100, 0, 100);
        const int err = led_set_brightness_dt(&battery_leds[i], brightness);
        if (err) {
            LOG_ERR("Failed to set battery LED %u: %d", i, err);
        }
    }
}

static int battery_led_event_listener(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *event = as_zmk_battery_state_changed(eh);
    if (event) {
        update_battery_leds(event->state_of_charge);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(battery_led, battery_led_event_listener);
ZMK_SUBSCRIPTION(battery_led, zmk_battery_state_changed);

static int battery_led_init(void) {
    update_battery_leds(zmk_battery_state_of_charge());
    return 0;
}

SYS_INIT(battery_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
