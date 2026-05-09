#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
#include <stdint.h>

/* --- 1. Get LED information from Device Tree --- */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

/* --- 2. Hardware address definition (based on Address Editor) --- */
#define DPU_BASE_ADDR   0xA0000000
#define SFM_BASE_ADDR   0xA0001000
#define INTC_BASE_ADDR  0x80000000

/* Offset and bit definitions */
#define REG_AP_CTRL     0x00
#define INTC_REG_ISR    0x00  /* Interrupt Status Register */
#define IDLE_BIT        (1 << 2) /* Common to Xilinx IPs: Bit 2 is Idle */

int main(void) {
    int ret;
    bool blink_toggle = false;

    printk("\n--- KV260 HW System Monitor (Zephyr) ---\n");

    /* --- 3. Initialize all LEDs --- */
    const struct gpio_dt_spec *leds[] = {&led0, &led1, &led2};
    for (int i = 0; i < 3; i++) {
        if (!gpio_is_ready_dt(leds[i])) {
            printk("Error: LED %d is not ready\n", i);
            return -1;
        }
        ret = gpio_pin_configure_dt(leds[i], GPIO_OUTPUT_INACTIVE);
        if (ret < 0) return -1;
    }

    /* Set register pointers */
    volatile uint32_t *dpu_reg  = (uint32_t *)(DPU_BASE_ADDR + REG_AP_CTRL);
    volatile uint32_t *sfm_reg  = (uint32_t *)(SFM_BASE_ADDR + REG_AP_CTRL);
    volatile uint32_t *intc_isr = (uint32_t *)(INTC_BASE_ADDR + INTC_REG_ISR);

    printk("Monitoring: DPU(0xA0000000), SFM(0xA0001000), INTC(0x80000000)\n");

    while (1) {
        uint32_t dpu_val = *dpu_reg;
        uint32_t sfm_val = *sfm_reg;
        uint32_t isr_val = *intc_isr;

        blink_toggle = !blink_toggle;

        /* --- LED0: DPU status --- */
        if (dpu_val & IDLE_BIT) {
            gpio_pin_set_dt(&led0, (int)blink_toggle); // Blink if Idle
        } else {
            gpio_pin_set_dt(&led0, 1); // Turn on if active
        }

        /* --- LED1: SFM (Softmax) status --- */
        if (sfm_val & IDLE_BIT) {
            gpio_pin_set_dt(&led1, (int)blink_toggle); // Blink if Idle
        } else {
            gpio_pin_set_dt(&led1, 1); // Turn on if active
        }

        /* --- LED2: INTC (Interrupt) status --- */
        if (isr_val == 0) {
            gpio_pin_set_dt(&led2, (int)blink_toggle); // Blink if no interrupt
        } else {
            gpio_pin_set_dt(&led2, 1); // Turn on if interrupt detected (Pending)
        }

        /* Console output (for debugging) */
        if (blink_toggle) {
            printk("STAT -> DPU: 0x%02x | SFM: 0x%02x | INTC_ISR: 0x%02x\n", 
                    dpu_val & 0xFF, sfm_val & 0xFF, isr_val & 0xFF);
        }

        k_msleep(500); // 500ms delay
    }

    return 0;
}