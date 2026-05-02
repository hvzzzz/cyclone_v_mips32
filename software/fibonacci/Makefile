deploy:
	scp fibo_load_inst.c cyclone:~/scripts/
	ssh cyclone "gcc ~/scripts/fibo_load_inst.c -o ~/scripts/fibo_load_inst -std=gnu99"

run: deploy
	ssh cyclone "~/scripts/fibo_load_inst"

read:
	scp read_data_mem.c cyclone:~/scripts/
	ssh cyclone "gcc ~/scripts/read_data_mem.c -o ~/scripts/read_data_mem -std=gnu99 && ~/scripts/read_data_mem"
