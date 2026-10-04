/**
 ******************************************************************************
 * @file           : assembly.c
 * @author         : julia jiang
 * @brief          : turns on the blue led using assembly
 ******************************************************************************
 */


int main(void) {
    __asm__ volatile (
        // clock constants
        ".equ RCC_AHB2ENR_ADDR, 0x40021000\n\t"
        ".equ RCC_AHB2ENR_OFFSET, 0x4C\n\t"

        // gpio constants
        ".equ GPIOB_BASE_ADDR, 0x48000400\n\t"
        ".equ GPIOB_MODER, GPIOB_BASE_ADDR\n\t"
        ".equ GPIOB_OTYPER_OFFSET, 0x04\n\t"
        ".equ GPIOB_BSRR_OFFSET, 0x18\n\t"

        // enable the clock
        "MOVW r0, #:lower16:RCC_AHB2ENR_ADDR\n\t"
        "MOVT r0, #:upper16:RCC_AHB2ENR_ADDR\n\t" // store the addr into r0
        "ADD r0, r0, #0x4C\n\t"                   // r0 = 0x4002104C
        "LDR r1, [r0]\n\t"                         // read into r1
        "ORR r1, r1, #0x2\n\t"                    // enable for gpio b only
        "STR r1, [r0]\n\t"                         // write updated value back

        // set pin mode to output type
        "MOVW r0, #:lower16:GPIOB_MODER\n\t"
        "MOVT r0, #:upper16:GPIOB_MODER\n\t"       // store the addr into r0
        "LDR r1, [r0]\n\t"
        "BIC r1, r1, #0xC000\n\t"                  // clear the bits
        "ORR r1, r1, #0x4000\n\t"                  // 14th and 15th bits correspond to pin 7
        "STR r1, [r0]\n\t"

        // configure push pull
        "MOVW r0, #:lower16:GPIOB_BASE_ADDR\n\t"
        "MOVT r0, #:upper16:GPIOB_BASE_ADDR\n\t"   // store the addr into r0
        "ADD r0, r0, #GPIOB_OTYPER_OFFSET\n\t"
        "LDR r1, [r0]\n\t"
        "BIC r1, r1, #0x80\n\t"
        "STR r1, [r0]\n\t"

        // turn the LED on
        "MOVW r0, #:lower16:GPIOB_BASE_ADDR\n\t"
        "MOVT r0, #:upper16:GPIOB_BASE_ADDR\n\t"   // store the addr into r0
        "ADD r0, r0, #GPIOB_BSRR_OFFSET\n\t"
        "MOV r1, #0x80\n\t"                        // turn the LED on
        "STR r1, [r0]\n\t"
        :
        :
        : "r0", "r1", "memory"
    );

    while (1);
}
