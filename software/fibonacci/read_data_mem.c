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
  printf("Data memory contents!\n");

  for (int i = 0; i < 35; i++) {
    /* printf("Data Mem [%d]: 0x%08X\n", i, data_mem_ptr[i]); */
    printf("Data Mem [%d]: %d\n", i, data_mem_ptr[i]);
  }

  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
  }
  close(fd);

  return 0;
}
