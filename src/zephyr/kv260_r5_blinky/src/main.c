/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/sys_io.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define SW0_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec leds[] = {
	GPIO_DT_SPEC_GET(LED0_NODE, gpios),
	GPIO_DT_SPEC_GET(LED1_NODE, gpios),
	GPIO_DT_SPEC_GET(LED2_NODE, gpios),
};

static const struct gpio_dt_spec sw = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

int main(void)
{
	int ret;
	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	/* Initialize all LED pins and set as output */
	for (int i = 0; i < ARRAY_SIZE(leds); i++) {
		if (!gpio_is_ready_dt(&leds[i])) {
			printf("Error: LED %d is not ready\n", i);
			return 0;
		}
		/* Turn off all LEDs initially (INACTIVE state) */
		ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
		if (ret < 0) {
			return 0;
		}
	}
	
	/* Initialize switch (button) and set as input */
	if (!gpio_is_ready_dt(&sw)) {
		printf("Error: SW0 is not ready\n");
		return 0;
	}
	ret = gpio_pin_configure_dt(&sw, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	int current_led = 0;

	while (1) {
		/* Turn off all LEDs temporarily */
		for (int i = 0; i < ARRAY_SIZE(leds); i++) {
			gpio_pin_set_dt(&leds[i], 0);
		}

		/* Turn on only the current LED */
		gpio_pin_set_dt(&leds[current_led], 1);

		printf("LED %d is ON\n", current_led);

		/* Read and print the state of the switch */
		int sw_state = gpio_pin_get_dt(&sw);
		printf("SW0 state: %d\n", sw_state);
		
		/* For debugging: Dump actual AXI GPIO register values to check */
		printf("  [Debug] Data Reg (0xb0000000): 0x%08x\n", sys_read32(0xb0000000));
		printf("  [Debug] Dir  Reg (0xb0000004): 0x%08x\n", sys_read32(0xb0000004));

		/* Calculate the index of the next LED to turn on (0 -> 1 -> 2 -> 0 ...) */
		current_led = (current_led + 1) % ARRAY_SIZE(leds);

		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
