/* Linker command file */

/* Memory organization for the product TM4C123GH6PM controller */

/* FLASH address - 0x0000.0000 to 0x0003.FFFF : Total ROM size - 256KB */
/* RAM address   - 0x2000.0000 to 0x2000.7FFF : Total RAM size - 32KB */
/* Peripheral    - 0x4000.0000 */
/* SRAM BitBand alias - 0x2200.0000 */
/* Peripheral BitBand alias - 0x4200.0000 */

/* Memory Layout */
/*
 * ------------------- FLASH -----------------------
 * Main Stack Pointer
 * NVIC Vector Table
 * Bootloader code
 * Program code
 * User data
 * ------------------- RAM -------------------------
 * Instruction memory - 0x2000.0000 to 0x2000.0FFF : size 4KB : This area is used for storing fast access codes
 * Data Memory  - 0x2000.1000 to 0x2000.
 * BSS Memory   - 
 * Heap Memory  -
 * Stack Memory - 
*/

/* The memory region size can be varied accordingly, but the linker will make sure that it is aligned to 16 bytes always */

/* define the RAM section length */
_iram_length = 0x00001000;
_dram_length = 
_heap_lenth  = 
_stack_legth = 0x00004000
MEMORY {
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00040000
    IRAM  (rwx) : ORIGIN = 0x20
    DRAM  (rwx) : ORIGIN = 
    HEAP  (rwx) : ORIGIN = 
    STACK (rwx) : ORIGIN = 
}