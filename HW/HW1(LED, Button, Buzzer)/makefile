LID_DIR = ..

all : main.elf
	scp main.elf ecube@192.168.137.207:/home/ecube/src/HW/HW1
	echo "[send main.elf]"
	rm -f *.o *.a
	echo "[remove files]"

ledtest.o : $(LID_DIR)/ledtest.h $(LID_DIR)/ledtest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/ledtest.c -o ledtest.o
	echo "1. make ledtest.o"

buttontest.o : $(LID_DIR)/buttontest.h $(LID_DIR)/buttontest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/buttontest.c -o buttontest.o
	echo "2. make buttontest.o"

buzzertest.o : $(LID_DIR)/buzzertest.h $(LID_DIR)/buzzertest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/buzzertest.c -o buzzertest.o
	echo "3. make buzzertest.o"

libMyPeri.a : ledtest.o buttontest.o buzzertest.o 
	arm-linux-gnueabi-ar rc libMyPeri.a ledtest.o buttontest.o buzzertest.o
	echo "[make libMyPeri.a]"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L. -lpthread
	echo "[make main.elf]"