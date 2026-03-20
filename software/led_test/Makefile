deploy:
	scp led.c cyclone:~/scripts/
	ssh cyclone "gcc ~/scripts/led.c -o ~/led -std=gnu99"

run: deploy
	ssh cyclone "~/scripts/led"
