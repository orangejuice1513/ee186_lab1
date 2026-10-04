/**
 ******************************************************************************
 * @file           : leds.c
 * @author         : julia jiang
 * @brief          : turns on 3 LED lights in sequence
 ******************************************************************************
 */

#include <stdint.h>

#define RCC_BASE_ADDR  0x40021000U
#define GPIOB_BASE_ADDR   0x48000400U
#define GPIOC_BASE_ADDR   0x48000800U

static uint32_t *gpioc_moder = (uint32_t *)GPIOC_BASE_ADDR;
static uint32_t *gpioc_otyper = (uint32_t *)(GPIOC_BASE_ADDR + 0x04U);
static uint32_t *gpioc_bsrr = (uint32_t *)(GPIOC_BASE_ADDR + 0x18U);

static uint32_t *gpiob_moder = (uint32_t *)GPIOB_BASE_ADDR;
static uint32_t *gpiob_otyper = (uint32_t *)(GPIOB_BASE_ADDR + 0x04U);
static uint32_t *gpiob_bsrr = (uint32_t *)(GPIOB_BASE_ADDR + 0x18U);

/* this function initializes all the pins */
void init_pins(){
	/**** green LED PC7 ****/

	// configure mode to general purpose output mode
	*gpioc_moder &= ~(3U << 14); // clear the bits
	*gpioc_moder |=  (1U << 14); // the 14th and 15th bits correspond to pin 7

	*gpioc_otyper &= ~(1U << 7);	// configure output type to push pull


	/**** blue LED PB7 ****/

	// configure mode to general purpose output mode
	*gpiob_moder &= ~(3U << 14); // clear the bits
	*gpiob_moder |=  (1U << 14); // the 14th and 15th bits correspond to pin 7

	*gpiob_otyper &= ~(1U << 7); 	// configure output type to push pull


	/**** red LED PB14 ****/

	// configure mode to general purpose output mode
	*gpiob_moder &= ~(3U << 28); // clear the bits
	*gpiob_moder |=  (1U << 28); // the 14th and 15th bits correspond to pin 7

	*gpiob_otyper &= ~(1U << 14); 	// configure output type to push pull

	return;
}

/* enables the clock for rcc ahb2 */
void enable_clock(){
	uint32_t *rcc_ahb2enr = (uint32_t*) (RCC_BASE_ADDR + 0x4C);
	*rcc_ahb2enr = *rcc_ahb2enr | 0x00000006; //enable the GPIO B and C
	return;
}


void wait(void){
    volatile uint32_t counter = 100000U;
    while (counter > 0U) {
        counter--;
    }
}



int main() {
	enable_clock();
	init_pins();


	// turn the LEDS on in green, blue, red order
	while (1) {
		*gpioc_bsrr = 0x00000080; // turn green LED (PC7) on
		wait();
		*gpioc_bsrr = 0x00800000;// turn green LED off
		*gpiob_bsrr = 0x00000080;// turn blue LED on (PB7)
		wait();
		*gpiob_bsrr = 0x00800000;// turn blue LED off
		*gpiob_bsrr = 0x00004000;// turn red LED on (PB14)
		wait();
		*gpiob_bsrr = 0x40000000; // turn red LED off
	}

}
