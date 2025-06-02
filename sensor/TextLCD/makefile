all : main.elf
	scp main.elf ecube@192.168.137.192:/home/ecube/sensors/TEXTLCD
	rm -f textlcd.o libMyPeri.a main.elf
	echo "4. send main.elf"

buzzer.o : textlcd.h textlcd.c
	arm-linux-gnueabi-gcc -o textlcd.o -c textlcd.c 
	echo "1. make textlcd.o"

libMyPeri.a : textlcd.o
	arm-linux-gnueabi-ar rc libMyPeri.a textlcd.o
	echo "2. make libMyperi.a"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L.
	echo "3. make main.elf"