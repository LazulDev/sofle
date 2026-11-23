/*
 * Temporary diagnostic: illuminate one WS2812 at a time, in strip order.
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/device.h>
#include <errno.h>
#include <zephyr/drivers/led_strip.h>
#include <drivers/ext_power.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>
#include <zmk/rgb_underglow.h>
#include <string.h>

#define STRIP_NODE DT_CHOSEN(zmk_underglow)
#define STRIP_LED_COUNT DT_PROP(STRIP_NODE, chain_length)
#define LED_ON_TIME K_SECONDS(2)

LOG_MODULE_REGISTER(sofle_rgb_led_test, LOG_LEVEL_INF);

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);
static struct led_rgb pixels[STRIP_LED_COUNT];
static struct k_work_delayable cycle_work;
static struct k_work_delayable start_work;
static size_t current_led;

static void show_current_led(void)
{
	memset(pixels, 0, sizeof(pixels));
	pixels[current_led] = (struct led_rgb){ .r = 32, .g = 32, .b = 32 };

	int err = led_strip_update_rgb(strip, pixels, ARRAY_SIZE(pixels));
	if (err) {
		LOG_ERR("Failed to update LED strip: %d", err);
	}
	LOG_INF("Showing strip LED index %u", (unsigned int)current_led);
}

static void cycle_led(struct k_work *work)
{
	ARG_UNUSED(work);
	current_led = (current_led + 1) % ARRAY_SIZE(pixels);
	show_current_led();
	k_work_schedule(&cycle_work, LED_ON_TIME);
}

static void start_test(struct k_work *work)
{
	ARG_UNUSED(work);

	/* Stop ZMK's 50 ms underglow refresh before writing individual pixels. */
	int err = zmk_rgb_underglow_off();
	if (err) {
		LOG_ERR("Failed to stop normal underglow updates: %d", err);
	}

	/* RGB_TOG normally cuts this board's LED power too; restore power for test. */
	const struct device *ext_power = device_get_binding("EXT_POWER");
	if (ext_power != NULL) {
		err = ext_power_enable(ext_power);
		if (err) {
			LOG_ERR("Failed to restore LED strip power: %d", err);
		}
	}

	current_led = 0;
	show_current_led();
	k_work_schedule(&cycle_work, LED_ON_TIME);
}

static int rgb_led_test_init(void)
{
	if (!device_is_ready(strip)) {
		LOG_ERR("Underglow LED strip device is not ready");
		return -ENODEV;
	}

	LOG_INF("Starting one-at-a-time test for %u strip LEDs",
		(unsigned int)ARRAY_SIZE(pixels));
	k_work_init_delayable(&cycle_work, cycle_led);
	k_work_init_delayable(&start_work, start_test);
	/* Let ZMK initialize its underglow state before stopping its refresh timer. */
	k_work_schedule(&start_work, K_SECONDS(1));
	return 0;
}

SYS_INIT(rgb_led_test_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
