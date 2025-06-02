all : main.elf
	scp main.elf ecube@192.168.137.192:/home/ecube/sensors/LED
	rm -f led.o libMyPeri.a main.elf

led.o : led.h led.c
	arm-linux-gnueabi-gcc -o led.o -c led.c 

libMyPeri.a : led.o
	arm-linux-gnueabi-ar rc libMyPeri.a led.o

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L.