# Variables
VCOM = vcom
VSIM = vsim
WORK = work

# Setup the working directory
$(WORK):
	vlib $(WORK)
	vmap $(WORK) $(WORK)

# Compile RTL and Testbenches
compile: $(WORK)
	$(VCOM) ../rtl/core/alu.vhd
	$(VCOM) ../rtl/core/alu_ctrl.vhd
	$(VCOM) ../rtl/core/regist.vhd
	$(VCOM) alu_tb.vhd
	$(VCOM) alu_ctrl_tb.vhd
	$(VCOM) regist_tb.vhd

# Simulate ALU and open GTKWave
sim_alu: compile
	$(VSIM) -c -do "vcd file alu_waves.vcd; vcd add -r /*; run 60 ns; quit -f" $(WORK).alu_tb
	gtkwave alu_waves.vcd &

# Simulate ALU Controller and open GTKWave
sim_alu_ctrl: compile
	$(VSIM) -c -do "vcd file alu_ctrl_waves.vcd; vcd add -r /*; run 80 ns; quit -f" $(WORK).alu_ctrl_tb
	gtkwave alu_ctrl_waves.vcd &

sim_register: compile
	$(VSIM) -c -do "vcd file regist_waves.vcd; vcd add -r /*; run 220 ns; quit -f" $(WORK).regist_tb
	gtkwave regist_waves.vcd &

# Clean up generated files
clean:
	rm -rf $(WORK) transcript *.vcd *.wlf
