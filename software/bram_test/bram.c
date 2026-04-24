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
  pio_in_port_ptr =
      (uint32_t *)(virtual_base +
                   ((HW_REGS_BASE + PIO_RESET_OFFSET) & HW_REGS_MASK));
  pio_out_port_ptr =
      (uint32_t *)(virtual_base +
                   ((HW_REGS_BASE + PIO_STS_OFFSET) & HW_REGS_MASK));

  printf("Bridge mapped successfully!\n\n");

  // ==========================================
  // 1. TEST INSTRUCTION MEMORY (WRITE & READ)
  // ==========================================
  printf("Loading dummy instructions into Instruction Memory...\n");

  // Example: Manually encoding some MIPS machine code
  // Let's pretend 0x20080005 is "addi $t0, $zero, 5"
  // 0. add $1, $0, $0   (R-Type: $1 = 0 + 0)
  instr_mem_ptr[0] = 0x00000820;  // MIPS PC = 0x00

  // 1. sw $1, 4($0)     (sw: Store the 0 from $1 into Data Memory address 4)
  instr_mem_ptr[1] = 0xAC010004;  // MIPS PC = 0x04

  // 2. lw $2, 4($0)     (lw: Load the 0 from Data Memory address 4 into $2)
  instr_mem_ptr[2] = 0x8C020004;  // MIPS PC = 0x08

  // 3. beq $1, $2, 1    (beq: If $1 == $2, skip forward 1 instruction)
  instr_mem_ptr[3] = 0x10220001;  // MIPS PC = 0x0C

  // 4. add $3, $1, $2   (R-Type: This is the instruction we skip!)
  instr_mem_ptr[4] = 0x00221820;  // MIPS PC = 0x10

  // 5. beq $0, $0, -1   (beq trap: Infinite loop to safely halt the CPU)
  instr_mem_ptr[5] = 0x1000FFFF;  // MIPS PC = 0x14

  // Read back to verify
  printf("Verification Readback:\n");
  printf("PC 0x00: 0x%08X\n", instr_mem_ptr[0]);
  printf("PC 0x04: 0x%08X\n", instr_mem_ptr[1]);
  printf("PC 0x08: 0x%08X\n", instr_mem_ptr[2]);
  printf("PC 0x0C: 0x%08X\n", instr_mem_ptr[2]);
  printf("PC 0x10: 0x%08X\n", instr_mem_ptr[2]);
  printf("PC 0x14: 0x%08X\n", instr_mem_ptr[2]);

  // ==========================================
  // 2. TEST DATA MEMORY (WRITE & READ)
  // ==========================================
  printf("Testing Data Memory...\n");
  data_mem_ptr[0] = 0xDEADBEEF;  // Address 0
  data_mem_ptr[1] = 0xCAFEBABE;  // Address 4

  printf("Data Mem [0]: 0x%08X\n", data_mem_ptr[0]);
  printf("Data Mem [1]: 0x%08X\n\n", data_mem_ptr[1]);

  /* for (int i = 0; i < 40; i++) { */
  /*   int toggle_val = i; */

  /*   *pio_in_port_ptr = toggle_val; */

  /*   unsigned int status = *pio_out_port_ptr; */

  /*   printf("Wrote: %d | Read Status: %d\n", toggle_val, status); */

  /*   usleep(1000000);  // Wait 1 seconds */
  /* } */

  /* *pio_in_port_ptr = 0; */

  // ==========================================
  // 3. MIPS CORE CONTROL (Future use)
  // ==========================================
  // When your MIPS core is ready, you will assert reset, load the memory, then
  // release reset: *pio_reset_ptr = 1; // Hold MIPS in reset
  // load_binary_file_to_memory();
  // *pio_reset_ptr = 0; // Release reset, MIPS starts executing!

  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
  }
  close(fd);

  return 0;
}
