all : main.elf
	scp main.elf ecube@192.168.137.192:/home/ecube/sensors/BUZZER
	rm -f buzzer.o libMyPeri.a main.elf
	echo "4. send main.elf"

buzzer.o : buzzer.h buzzer.c
	arm-linux-gnueabi-gcc -o buzzer.o -c buzzer.c 
	echo "1. make buzzer.o"

libMyPeri.a : buzzer.o
	arm-linux-gnueabi-ar rc libMyPeri.a buzzer.o
	echo "2. make libMyperi.a"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L.
	echo "3. make main.elf"