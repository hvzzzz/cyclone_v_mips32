deploy:
	scp bram.c cyclone:~/scripts/
	ssh cyclone "gcc ~/scripts/bram.c -o ~/scripts/bram -std=gnu99"

run: deploy
	ssh cyclone "~/scripts/bram"
