.PHONY: fpga-build fpga-flash sw-deploy sw-run help

help:
	@echo "Cyclone V MIPS32 Build System"
	@echo "---------------------------------"
	@echo "make fpga-build  : Synthesize VHDL and generate .sof bitstream"
	@echo "make fpga-flash  : Program the FPGA fabric via JTAG (bypassing ARM)"
	@echo "make sw-deploy   : Send C code to the board and compile via GCC"
	@echo "make sw-run      : Deploy code and execute immediately on the HPS"

# --- Hardware Targets (Quartus CLI) ---

fpga-build:
	@echo "Starting Quartus Command Line Build..."
	cd fpga && quartus_sh -t build.tcl

fpga-flash:
	@echo "Programming Cyclone V FPGA (Device 2)..."
	cd fpga && quartus_pgm -m jtag -c 1 -o "p;mips_32.sof@2"

# --- Software Targets (ARM HPS via SSH) ---

sw-deploy:
	@echo "Deploying C code to HPS via SSH..."
	$(MAKE) -C software/led_test deploy

sw-run:
	@echo "Executing C code on HPS..."
	$(MAKE) -C software/led_test run
