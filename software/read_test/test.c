#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define DATA_MEM_WORDS 1024

#define HW_REGS_BASE 0xFF200000
#define HW_REGS_SPAN 0x00200000  // 2MB span covers the entire Lightweight bus
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

// Updated Offsets from Platform Designer
#define DATA_MEM_OFFSET 0x0000
#define INSTR_MEM_OFFSET 0x1000

#define PIO_WRITE_INFO_OFFSET 0x2000
#define PIO_PC_OFFSET 0x2020

int main() {
  int fd;
  void *virtual_base;

  // Pointers for our hardware components
  volatile uint32_t *data_mem_ptr;
  volatile uint32_t *instr_mem_ptr;
  volatile uint32_t *pio_pc_ptr;
  volatile uint32_t *pio_write_info;

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
  pio_write_info =
      (uint32_t *)(virtual_base +
                   ((HW_REGS_BASE + PIO_WRITE_INFO_OFFSET) & HW_REGS_MASK));
  pio_pc_ptr = (uint32_t *)(virtual_base +
                            ((HW_REGS_BASE + PIO_PC_OFFSET) & HW_REGS_MASK));

  printf("Bridge mapped successfully!\n\n");

  // ==========================================
  // 1. TEST INSTRUCTION MEMORY (WRITE & READ)
  // ==========================================

  uint32_t write_info = *pio_write_info;
  uint16_t pio_addr_idx = (uint16_t)(write_info & 0x3FFU);
  uint8_t pio_byteen = (uint8_t)((write_info >> 10) & 0x0FU);
  bool write_active = (pio_byteen != 0) && (pio_addr_idx < DATA_MEM_WORDS);

  printf("current_pc   = 0x%08X\n", *pio_pc_ptr);
  printf("PC 0x00: 0x%08X\n", instr_mem_ptr[0]);
  printf("PC 0x04: 0x%08X\n", instr_mem_ptr[1]);
  printf("PC 0x08: 0x%08X\n", instr_mem_ptr[2]);
  printf("PC 0x0C: 0x%08X\n", instr_mem_ptr[3]);
  printf("PC 0x10: 0x%08X\n", instr_mem_ptr[4]);
  printf("PC 0x14: 0x%08X\n", instr_mem_ptr[5]);

  printf("DATA 0x00: 0x%08X\n", data_mem_ptr[0]);
  printf("DATA 0x04: 0x%08X\n", data_mem_ptr[1]);
  printf("DATA 0x08: 0x%08X\n", data_mem_ptr[2]);
  printf("DATA 0x0C: 0x%08X\n", data_mem_ptr[3]);
  printf("DATA 0x10: 0x%08X\n", data_mem_ptr[4]);
  printf("DATA 0x14: 0x%08X\n", data_mem_ptr[5]);

  printf("write_info   = 0x%08X\n", write_info);
  printf("pio_addr_idx = 0x%04X\n", pio_addr_idx);
  printf("pio_byteen   = 0x%02X\n", pio_byteen);
  printf("write_active = 0x%X\n", write_active ? 1 : 0);

  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
  }
  close(fd);

  return 0;
}
