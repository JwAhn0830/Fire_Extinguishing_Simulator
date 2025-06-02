all : button.elf
	scp button.elf ecube@192.168.137.192:/home/ecube/sensors/BUTTON
	rm -f button.o libMyPeri.a button.elf
	echo "4. send and remove"

button.o : button.h button.c
	arm-linux-gnueabi-gcc -c button.c -o button.o
	echo "1. make button.o"

libMyPeri.a : button.o
	arm-linux-gnueabi-ar rc libMyPeri.a button.o
	echo "2. make libMyPeri.a"

button.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o button.elf main.c -l MyPeri -L. -lpthread
	echo "3. make button.elf"