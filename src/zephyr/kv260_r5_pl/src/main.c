
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <stdint.h> // For uint32_t

int main(void) {
    printk("\n--- Zephyr R5: Manual Bus Test Start ---\n");

    // 1. First, simple liveness check
    printk("Step 1: System is running.\n");

    // 2. Test manual reading of the PL region address (axi_gpio_0)
    // Note: It is assumed that status = "disabled" in the overlay
    uint32_t target_addr = 0xb0000000; 
    volatile uint32_t *pl_ptr = (uint32_t *)target_addr;

    printk("Step 2: Attempting to read from PL address: 0x%08x...\n", target_addr);
    
    /* 
     * If it hangs here, it is highly likely that the Cortex-R5 MPU
     * has not permitted access to this memory area.
     */
    uint32_t val = *pl_ptr; 

    // 3. This path is executed only if the read is successful
    printk("Step 3: Read Success! Value at 0x%08x = 0x%08x\n", target_addr, val);

    while (1) {
        printk("I am alive! Heartbeat... (val: 0x%08x)\n", val);
        k_msleep(2000);
    }

    return 0;
}