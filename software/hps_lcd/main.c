// ============================================================================
// Copyright (c) 2013 by Terasic Technologies Inc.
// ============================================================================
//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development
//   Kits made by Terasic.  Other use of this code, including the selling
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use
//   or functionality of this code.
//
// ============================================================================
//
//  Terasic Technologies Inc
//  9F., No.176, Sec.2, Gongdao 5th Rd, East Dist, Hsinchu City, 30070. Taiwan
//
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// ============================================================================

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// LCD Headers
#include "LCD_Lib.h"
#include "lcd_graphic.h"
#include "terasic_os_includes.h"

// ==========================================
// CUSTOM 5x7 MINI FONT DICTIONARY
// ==========================================
// This sparse array holds exactly the characters your debugger needs.
// Any character not defined here defaults to a blank space, preventing crashes.
const uint8_t mini_font[256][5] = {['0'] = {0x3E, 0x51, 0x49, 0x45, 0x3E},
                                   ['1'] = {0x00, 0x42, 0x7F, 0x40, 0x00},
                                   ['2'] = {0x42, 0x61, 0x51, 0x49, 0x46},
                                   ['3'] = {0x21, 0x41, 0x45, 0x4B, 0x31},
                                   ['4'] = {0x18, 0x14, 0x12, 0x7F, 0x10},
                                   ['5'] = {0x27, 0x45, 0x45, 0x45, 0x39},
                                   ['6'] = {0x3C, 0x4A, 0x49, 0x49, 0x30},
                                   ['7'] = {0x01, 0x71, 0x09, 0x05, 0x03},
                                   ['8'] = {0x36, 0x49, 0x49, 0x49, 0x36},
                                   ['9'] = {0x06, 0x49, 0x49, 0x29, 0x1E},
                                   ['A'] = {0x7E, 0x11, 0x11, 0x11, 0x7E},
                                   ['B'] = {0x7F, 0x49, 0x49, 0x49, 0x36},
                                   ['C'] = {0x3E, 0x41, 0x41, 0x41, 0x22},
                                   ['D'] = {0x7F, 0x41, 0x41, 0x22, 0x1C},
                                   ['E'] = {0x7F, 0x49, 0x49, 0x49, 0x41},
                                   ['F'] = {0x7F, 0x09, 0x09, 0x09, 0x01},
                                   ['P'] = {0x7F, 0x09, 0x09, 0x09, 0x06},
                                   ['I'] = {0x00, 0x41, 0x7F, 0x41, 0x00},
                                   ['R'] = {0x7F, 0x09, 0x19, 0x29, 0x46},
                                   ['O'] = {0x3E, 0x41, 0x41, 0x41, 0x3E},
                                   ['T'] = {0x01, 0x01, 0x7F, 0x01, 0x01},
                                   ['n'] = {0x7C, 0x08, 0x04, 0x04, 0x78},
                                   ['x'] = {0x44, 0x28, 0x10, 0x28, 0x44},
                                   ['['] = {0x00, 0x7F, 0x41, 0x41, 0x00},
                                   [']'] = {0x00, 0x41, 0x41, 0x7F, 0x00},
                                   ['-'] = {0x08, 0x08, 0x08, 0x08, 0x08},
                                   ['+'] = {0x08, 0x08, 0x3E, 0x08, 0x08},
                                   [':'] = {0x00, 0x36, 0x36, 0x00, 0x00},
                                   [' '] = {0x00, 0x00, 0x00, 0x00, 0x00}};

// Custom function to draw the tiny font pixel-by-pixel
void Draw_MiniString(LCD_CANVAS *canvas, int start_x, int start_y,
                     const char *str) {
  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    unsigned char c = str[i];
    for (int col = 0; col < 5; col++) {
      unsigned char column_data = mini_font[c][col];
      for (int row = 0; row < 8; row++) {
        if (column_data & (1 << row)) {
          // FIX: Use DRAW_Pixel instead of DRAW_Rect
          DRAW_Pixel(canvas, start_x + (i * 6) + col, start_y + row, LCD_BLACK);
        }
      }
    }
  }
}

// ==========================================
// HARDWARE MAPPING DEFINITIONS
// ==========================================
#define LCD_HW_REGS_BASE (ALT_STM_OFST)
#define LCD_HW_REGS_SPAN (0x04000000)
#define LCD_HW_REGS_MASK (LCD_HW_REGS_SPAN - 1)

#define MIPS_HW_REGS_BASE 0xFF200000
#define MIPS_HW_REGS_SPAN 0x00200000
#define MIPS_HW_REGS_MASK (MIPS_HW_REGS_SPAN - 1)

#define DATA_MEM_OFFSET 0x0000
#define INSTR_MEM_OFFSET 0x1000
#define PIO_STS_OFFSET 0x2020

#define DATA_MEM_WORDS 1024
#define INSTR_MEM_WORDS 1024

int main() {
  int fd;
  void *lcd_virtual_base, *mips_virtual_base;
  volatile uint32_t *data_mem_ptr, *instr_mem_ptr, *pio_pc_ptr;
  LCD_CANVAS LcdCanvas;
  char str_buffer[32];

  if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) return 1;

  lcd_virtual_base = mmap(NULL, LCD_HW_REGS_SPAN, (PROT_READ | PROT_WRITE),
                          MAP_SHARED, fd, LCD_HW_REGS_BASE);
  mips_virtual_base = mmap(NULL, MIPS_HW_REGS_SPAN, (PROT_READ | PROT_WRITE),
                           MAP_SHARED, fd, MIPS_HW_REGS_BASE);

  data_mem_ptr =
      (uint32_t *)(mips_virtual_base +
                   ((MIPS_HW_REGS_BASE + DATA_MEM_OFFSET) & MIPS_HW_REGS_MASK));
  instr_mem_ptr =
      (uint32_t *)(mips_virtual_base + ((MIPS_HW_REGS_BASE + INSTR_MEM_OFFSET) &
                                        MIPS_HW_REGS_MASK));
  pio_pc_ptr =
      (uint32_t *)(mips_virtual_base +
                   ((MIPS_HW_REGS_BASE + PIO_STS_OFFSET) & MIPS_HW_REGS_MASK));

  LcdCanvas.Width = LCD_WIDTH;
  LcdCanvas.Height = LCD_HEIGHT;
  LcdCanvas.BitPerPixel = 1;
  LcdCanvas.FrameSize = LcdCanvas.Width * LcdCanvas.Height / 8;
  LcdCanvas.pFrame = (void *)malloc(LcdCanvas.FrameSize);

  LCDHW_Init(lcd_virtual_base);
  LCDHW_BackLight(true);
  LCD_Init();

  // ==========================================
  // DEBUGGER STATE VARIABLES (Add these here!)
  // ==========================================
  uint32_t shadow_mem[DATA_MEM_WORDS] = {0};
  uint32_t last_pc = 0;
  int display_idx = 0;

  // ==========================================
  // MAIN DEBUGGER LOOP
  // ==========================================
  while (1) {
    DRAW_Clear(&LcdCanvas, LCD_WHITE);

    uint32_t current_pc = *pio_pc_ptr;
    uint32_t pc_idx = current_pc / 4;

    // 1. SEGFAULT PROTECTION
    if (pc_idx >= INSTR_MEM_WORDS) {
      sprintf(str_buffer, "PC: 0x%08X", current_pc);
      Draw_MiniString(&LcdCanvas, 0, 0, str_buffer);
      Draw_MiniString(&LcdCanvas, 0, 8, "- PC OOB ERR -");
      DRAW_Refresh(&LcdCanvas);
      usleep(100000);
      continue;
    }

    // 2. INSTRUCTION VIEW
    sprintf(str_buffer, "PC: 0x%08X", current_pc);
    Draw_MiniString(&LcdCanvas, 0, 0, str_buffer);

    if (pc_idx > 0)
      sprintf(str_buffer, "[-] 0x%08X", instr_mem_ptr[pc_idx - 1]);
    else
      sprintf(str_buffer, "[-] ----------");
    Draw_MiniString(&LcdCanvas, 0, 8, str_buffer);

    sprintf(str_buffer, "[C] 0x%08X", instr_mem_ptr[pc_idx]);
    Draw_MiniString(&LcdCanvas, 0, 16, str_buffer);

    sprintf(str_buffer, "[+] 0x%08X", instr_mem_ptr[pc_idx + 1]);
    Draw_MiniString(&LcdCanvas, 0, 24, str_buffer);

    Draw_MiniString(&LcdCanvas, 0, 32, "-ADDR------DATA------");

    // 3. SHADOW MEMORY SCANNER
    // If the PC drops, the user pressed reset.
    if (current_pc < last_pc) {
      display_idx = 0;  // Snap view to top

      // To make autoscrolling work, we MUST force a state change.
      // We wipe both the real hardware BRAM and our software shadow memory.
      // Now, when MIPS writes '34', the ARM sees '0' change to '34' and
      // scrolls!
      for (int i = 0; i < DATA_MEM_WORDS; i++) {
        data_mem_ptr[i] = 0;
        shadow_mem[i] = 0;
      }
    }
    last_pc = current_pc;

    // Compare real BRAM against our Shadow Memory to find new writes
    for (int i = 0; i < DATA_MEM_WORDS; i++) {
      if (data_mem_ptr[i] != shadow_mem[i]) {
        shadow_mem[i] = data_mem_ptr[i];  // Update our shadow copy
        display_idx = i;                  // Snap the view to this address
      }
    }

    // Compare real BRAM against our Shadow Memory to find new writes
    for (int i = 0; i < DATA_MEM_WORDS; i++) {
      if (data_mem_ptr[i] != shadow_mem[i]) {
        shadow_mem[i] = data_mem_ptr[i];  // Update our shadow copy
        display_idx = i;                  // Snap the view to this address
      }
    }

    // 4. DATA MEMORY VIEW (GDB Hex Format)
    // Create a sliding window so we always show 3 lines safely
    int start_idx = display_idx - 2;
    if (start_idx < 0) start_idx = 0;

    // Line 1
    sprintf(str_buffer, "0x%03X: %u", start_idx * 4, data_mem_ptr[start_idx]);
    Draw_MiniString(&LcdCanvas, 0, 40, str_buffer);

    // Line 2
    sprintf(str_buffer, "0x%03X: %u", (start_idx + 1) * 4,
            data_mem_ptr[start_idx + 1]);
    Draw_MiniString(&LcdCanvas, 0, 48, str_buffer);

    // Line 3
    sprintf(str_buffer, "0x%03X: %u", (start_idx + 2) * 4,
            data_mem_ptr[start_idx + 2]);
    Draw_MiniString(&LcdCanvas, 0, 56, str_buffer);

    // --- PUSH FRAME ---
    DRAW_Refresh(&LcdCanvas);
    usleep(50000);
  }
  free(LcdCanvas.pFrame);

  munmap(lcd_virtual_base, LCD_HW_REGS_SPAN);
  munmap(mips_virtual_base, MIPS_HW_REGS_SPAN);
  close(fd);
  return 0;
}
