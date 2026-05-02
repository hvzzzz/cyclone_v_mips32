#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define HW_REGS_BASE 0xFF200000
#define HW_REGS_SPAN 0x00200000  // 2MB span covers the entire Lightweight bus
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

// Updated Offsets from Platform Designer
#define DATA_MEM_OFFSET 0x0000
#define INSTR_MEM_OFFSET 0x1000

#define PIO_RESET_OFFSET 0x2000
#define PIO_STS_OFFSET 0x2020

int main() {
  int fd;
  void *virtual_base;

  // Pointers for our hardware components
  volatile uint32_t *data_mem_ptr;
  volatile uint32_t *instr_mem_ptr;
  volatile uint32_t *pio_in_port_ptr;
  volatile uint32_t *pio_out_port_ptr;

  if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
    printf("ERROR: could not open \"/dev/mem\"...\n");
    return 1;
  }

  virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED,
                      fd, HW_REGS_BASE);
  if (virtual_base == MAP_FAILED) {
    printf("ERROR: mmap() failed...\n");
    close(fd);
    return 1;
  }

  // Map the specific pointers using the offsets
  data_mem_ptr = (uint32_t *)(virtual_base + ((HW_REGS_BASE + DATA_MEM_OFFSET) &
                                              HW_REGS_MASK));
  instr_mem_ptr =
      (uint32_t *)(virtual_base +
                   ((HW_REGS_BASE + INSTR_MEM_OFFSET) & HW_REGS_MASK));

  printf("Bridge mapped successfully!\n\n");

  // ==========================================
  // 1. TEST INSTRUCTION MEMORY (WRITE & READ)
  // ==========================================
  printf("Loading the fibonacci instructions into Instruction Memory...\n");

  instr_mem_ptr[0] = 0x00000820;  // add $1, $0, $0 # $1 = 0 (Current Fib)
  instr_mem_ptr[1] = 0x20020001;  // addi $2, $0, 1 # $2 = 1 (Next Fib)
  instr_mem_ptr[2] =
      0x00001820;  // add $3, $0, $0 # $3 = 0 (Memory Address Pointer)
  instr_mem_ptr[3] = 0x20040028;  // addi $4, $0, 40 # $4 = 40 (Loop Limit: 10
                                  // numbers * 4 bytes)
  instr_mem_ptr[4] = 0x10640006;  // loop: beq $3, $4, 6 # If Pointer == 40,
                                  // jump forward 6 instructions to 'exit'
  instr_mem_ptr[5] =
      0xAC610000;  // sw $1, 0($3) # Store Current Fib to Data Memory
  instr_mem_ptr[6] = 0x00222820;   // add $5, $1, $2 # Temp = Current + Next
  instr_mem_ptr[7] = 0x00020820;   // add $1, $0, $2 # Current = Next
  instr_mem_ptr[8] = 0x00051020;   // add $2, $0, $5 # Next = Temp
  instr_mem_ptr[9] = 0x20630004;   // addi $3, $3, 4 # Pointer = Pointer + 4
  instr_mem_ptr[10] = 0x1000FFF9;  // beq $0, $0, -7 # Unconditional jump
                                   // backward 7 instructions to 'loop'
  instr_mem_ptr[11] = 0x1000FFFF;  // exit: beq $0, $0, -1 # Infinite trap

  // Read back to verify
  printf("Verification Readback:\n");
  printf("PC 0x00: 0x%08X\n", instr_mem_ptr[0]);
  printf("PC 0x04: 0x%08X\n", instr_mem_ptr[1]);
  printf("PC 0x08: 0x%08X\n", instr_mem_ptr[2]);
  printf("PC 0x0C: 0x%08X\n", instr_mem_ptr[3]);
  printf("PC 0x10: 0x%08X\n", instr_mem_ptr[4]);
  printf("PC 0x14: 0x%08X\n", instr_mem_ptr[5]);
  printf("PC 0x18: 0x%08X\n", instr_mem_ptr[6]);
  printf("PC 0x1C: 0x%08X\n", instr_mem_ptr[7]);
  printf("PC 0x20: 0x%08X\n", instr_mem_ptr[8]);
  printf("PC 0x24: 0x%08X\n", instr_mem_ptr[9]);
  printf("PC 0x28: 0x%08X\n", instr_mem_ptr[10]);
  printf("PC 0x2C: 0x%08X\n", instr_mem_ptr[11]);

  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
  }
  close(fd);

  return 0;
}
