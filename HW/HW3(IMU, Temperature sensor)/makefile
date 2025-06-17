LID_DIR = ..

all : main.elf
	scp main.elf ecube@192.168.137.207:/home/ecube/src/HW/HW3
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

fndtest.o : $(LID_DIR)/fndtest.h $(LID_DIR)/fndtest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/fndtest.c -o fndtest.o
	echo "4. make fndtest.o"

textlcdtest.o : $(LID_DIR)/textlcdtest.h $(LID_DIR)/textlcdtest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/textlcdtest.c -o textlcdtest.o
	echo "5. make textlcdtest.o"

colorLEDtest.o : $(LID_DIR)/colorLEDtest.h $(LID_DIR)/colorLEDtest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/colorLEDtest.c -o colorLEDtest.o
	echo "6. make colorLEDtest.o"

accelManGyroTest.o : $(LID_DIR)/accelManGyroTest.h $(LID_DIR)/accelManGyroTest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/accelManGyroTest.c -o accelManGyroTest.o
	echo "7. make accelManGyroTest.o"

temperatureTest.o : $(LID_DIR)/temperatureTest.h $(LID_DIR)/temperatureTest.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/temperatureTest.c -o temperatureTest.o
	echo "8. make temperatureTest.o"	

libMyPeri.a : ledtest.o buttontest.o buzzertest.o fndtest.o textlcdtest.o colorLEDtest.o accelManGyroTest.o temperatureTest.o
	arm-linux-gnueabi-ar rc libMyPeri.a ledtest.o buttontest.o buzzertest.o fndtest.o textlcdtest.o colorLEDtest.o accelManGyroTest.o temperatureTest.o
	echo "[make libMyPeri.a]"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L. -lpthread
	echo "[make main.elf]"