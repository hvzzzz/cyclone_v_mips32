#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>  // Added for exit() and strtoul
#include <sys/mman.h>
#include <unistd.h>

#define HW_REGS_BASE 0xFF200000
#define HW_REGS_SPAN 0x00200000
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

#define INSTR_MEM_OFFSET 0x1000

int main(int argc, char *argv[]) {
  int fd;
  void *virtual_base;
  volatile uint32_t *instr_mem_ptr;

  if (argc < 2) {
    printf("Usage: %s <hex_file>\n", argv[0]);
    return 1;
  }

  // 1. Open the .hex file
  FILE *hex_file = fopen(argv[1], "r");
  if (!hex_file) {
    perror("ERROR: Could not open hex file");
    return 1;
  }

  // 2. Open /dev/mem for hardware access
  if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
    printf("ERROR: could not open \"/dev/mem\"...\n");
    fclose(hex_file);
    return 1;
  }

  // 3. Map the hardware bridge
  virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED,
                      fd, HW_REGS_BASE);
  if (virtual_base == MAP_FAILED) {
    printf("ERROR: mmap() failed...\n");
    close(fd);
    fclose(hex_file);
    return 1;
  }

  // Map the instruction memory pointer
  instr_mem_ptr =
      (uint32_t *)(virtual_base +
                   ((HW_REGS_BASE + INSTR_MEM_OFFSET) & HW_REGS_MASK));

  printf("Bridge mapped successfully! Loading: %s\n\n", argv[1]);

  // 4. Read file and "Burn" to memory
  char line[256];
  uint32_t machine_code;
  int index = 0;

  while (fgets(line, sizeof(line), hex_file)) {
    // sscanf stops at the first space, effectively ignoring our "// comments"
    if (sscanf(line, "%x", &machine_code) == 1) {
      instr_mem_ptr[index] = machine_code;
      printf("Loaded PC 0x%02X: 0x%08X\n", index * 4, machine_code);
      index++;
    }
  }

  // ... (Loading logic from previous step) ...

  printf("\nSuccessfully loaded %d instructions.\n", index);
  fclose(hex_file);  // Close file before verification

  // ==========================================
  // 2. VERIFICATION PASS (READ BACK)
  // ==========================================
  printf("\n--- Starting Verification Read-Back ---\n");
  int errors = 0;

  // We loop back through the addresses we just wrote
  for (int i = 0; i < index; i++) {
    uint32_t hardware_value = instr_mem_ptr[i];

    // In a real-world scenario, you could even store the expected values
    // in an array to do an automated comparison here.
    printf("Read PC 0x%02X: 0x%08X", i * 4, hardware_value);

    // Quick visual check: does it look like 0 or FFFFFFFF?
    // That usually means a mapping error.
    if (hardware_value == 0x00000000 || hardware_value == 0xFFFFFFFF) {
      printf("  <-- [WARNING: Potential Empty/Static Memory]");
    }
    printf("\n");
  }

  printf(
      "\nVerification complete. If values match your .hex file, you're ready "
      "to run!\n");

  // Clean up
  if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
    printf("ERROR: munmap() failed...\n");
  }
  close(fd);

  return 0;
}
