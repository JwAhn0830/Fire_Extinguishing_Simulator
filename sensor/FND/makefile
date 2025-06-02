all : main.elf
	scp main.elf ecube@192.168.137.54:/home/ecube/sensors/FND
	rm -f fnd.o libMyPeri.a main.elf
	echo "4. send main.elf"
fnd.o : fnd.h fnd.c
	arm-linux-gnueabi-gcc -o fnd.o -c fnd.c 
	echo "1. make fnd.o"

libMyPeri.a : fnd.o
	arm-linux-gnueabi-ar rc libMyPeri.a fnd.o
	echo "2. make libMyPeri.a"
main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L.
	echo "3. make main.elf"