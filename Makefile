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

bram-deploy:
	@echo "Deploying C code to HPS via SSH..."
	$(MAKE) -C software/bram_test deploy

bram-run:
	@echo "Executing C code on HPS..."
	$(MAKE) -C software/bram_test run

fibo-deploy:
	@echo "Deploying C code to HPS via SSH..."
	$(MAKE) -C software/fibonacci deploy

fibo-run:
	@echo "Executing C code on HPS..."
	$(MAKE) -C software/fibonacci run

fibo-read:
	@echo "Executing C code on HPS..."
	$(MAKE) -C software/fibonacci read

lcd_build:
	@echo "Executing C code on HPS..."
	cd software/hps_lcd && ~/intelFPGA/20.1/embedded/embedded_command_shell.sh make

lcd_deploy:
	@echo "Executing C code on HPS..."
	cd software/hps_lcd && scp hps_lcd cyclone:~/scripts/

lcd_run:
	@echo "Executing C code on HPS..."
	ssh cyclone "~/scripts/hps_lcd"
