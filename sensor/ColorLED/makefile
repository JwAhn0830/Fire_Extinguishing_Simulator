all : main.elf
	scp main.elf ecube@192.168.137.54:/home/ecube/sensors/COLORLED
	rm -f colorled.o libMyPeri.a main.elf
	echo "4. send main.elf"

colorled.o : colorLED.h colorLED.c
	arm-linux-gnueabi-gcc -o colorled.o -c colorLED.c 
	echo "1. make colorled.o"

libMyPeri.a : colorled.o
	arm-linux-gnueabi-ar rc libMyPeri.a colorled.o
	echo "2. make libMyperi.a"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L.
	echo "3. make main.elf"