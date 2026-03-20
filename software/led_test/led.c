#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#define HW_REGS_BASE 0xFF200000
#define HW_REGS_SPAN 0x00200000
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

#define PIO_LED_OFFSET 0x00  // Offset for pio_leds
#define PIO_STS_OFFSET 0x20  // Offset for mips_status

int main() {
  int fd;
  void *virtual_base;
  volatile unsigned int *hps_to_fpga_ptr;
  volatile unsigned int *fpga_to_hps_ptr;

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

  hps_to_fpga_ptr =
      (unsigned int *)(virtual_base +
                       ((HW_REGS_BASE + PIO_LED_OFFSET) & HW_REGS_MASK));
  fpga_to_hps_ptr =
      (unsigned int *)(virtual_base +
                       ((HW_REGS_BASE + PIO_STS_OFFSET) & HW_REGS_MASK));

  printf("Bridge mapped successfully! Toggling LED 0...\n");

  for (int i = 0; i < 40; i++) {
    int toggle_val = i;

    *hps_to_fpga_ptr = toggle_val;

    unsigned int status = *fpga_to_hps_ptr;

    printf("Wrote: %d | Read Status: %d\n", toggle_val, status);

    usleep(1000000);  // Wait 1 seconds
  }

  *hps_to_fpga_ptr = 0;  // Turn off LED before exiting
  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
  }
  close(fd);

  return 0;
}
