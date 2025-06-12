all : main.elf
	scp main.elf ecube@192.168.137.54:/home/ecube/src/HW/
	echo "[send main.elf]"
	rm -f *.o *.elf
	echo "[remove files]"

ledtest.o : ledtest.h ledtest.c
	arm-linux-gnueabi-gcc -c ledtest.c -o ledtest.o
	echo "1. make ledtest.o"

buttontest.o : buttontest.h buttontest.c
	arm-linux-gnueabi-gcc -c buttontest.c -o buttontest.o
	echo "2. make buttontest.o"

buzzertest.o : buzzertest.h buzzertest.c
	arm-linux-gnueabi-gcc -c buzzertest.c -o buzzertest.o
	echo "3. make buzzertest.o"

fndtest.o : fndtest.h fndtest.c
	arm-linux-gnueabi-gcc -c fndtest.c -o fndtest.o
	echo "4. make fndtest.o"

textlcdtest.o : textlcdtest.h textlcdtest.c 
	arm-linux-gnueabi-gcc -c textlcdtest.c -o textlcdtest.o
	echo "5. make textlcdtest.o"

colorLEDtest.o : colorLEDtest.h colorLEDtest.c 
	arm-linux-gnueabi-gcc -c colorLEDtest.c -o colorLEDtest.o
	echo "6. make colorLEDtest.o"

temperatureTest.o : temperatureTest.h temperatureTest.c 
	arm-linux-gnueabi-gcc -c temperatureTest.c -o temperatureTest.o
	echo "7. make temperatureTest.o"

accelManGyroTest.o : accelManGyroTest.h accelManGyroTest.c 
	arm-linux-gnueabi-gcc -c accelManGyroTest.c -o accelManGyroTest.o
	echo "8. make accelManGyroTest.o"
	
libMyPeri.a : buttontest.o ledtest.o buzzertest.o fndtest.o textlcdtest.o colorLEDtest.o temperatureTest.o accelManGyroTest.o
	arm-linux-gnueabi-ar rc libMyPeri.a buttontest.o ledtest.o buzzertest.o fndtest.o textlcdtest.o colorLEDtest.o temperatureTest.o accelManGyroTest.o
	echo "[make libMyPeri.a]"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L. -lpthread
	echo "[make main.elf]"