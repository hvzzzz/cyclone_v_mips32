deploy:
	scp flasher.c cyclone:~/scripts/
	scp ../../scripts/program.hex cyclone:~/scripts/
	ssh cyclone "gcc ~/scripts/flasher.c -o ~/scripts/flasher -std=gnu99"

run:
	ssh cyclone "~/scripts/flasher ~/scripts/program.hex"
