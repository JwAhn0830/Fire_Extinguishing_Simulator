LID_DIR = lib

all : main.elf
	scp main.elf ecube@192.168.219.65:/home/ecube/src/src/client
	echo "[send main.elf]"
	rm -f *.o *.a
	echo "[remove files]"

button.o : $(LID_DIR)/button.h $(LID_DIR)/button.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/button.c -o button.o
	echo "1. make button.o"

buzzer.o : $(LID_DIR)/buzzer.h $(LID_DIR)/buzzer.c
	arm-linux-gnueabi-gcc -c $(LID_DIR)/buzzer.c -o buzzer.o
	echo "2. make buzzer.o"

gyro.o : $(LID_DIR)/gyro.h $(LID_DIR)/gyro.c 
	arm-linux-gnueabi-gcc -c $(LID_DIR)/gyro.c -o gyro.o
	echo "3. make gyro.o"

client.o : $(LID_DIR)/client.h $(LID_DIR)/client.c 
	arm-linux-gnueabi-gcc -c $(LID_DIR)/client.c -o client.o
	echo "4. make client.o"

libMyPeri.a : button.o buzzer.o gyro.o client.o 
	arm-linux-gnueabi-ar rc libMyPeri.a button.o buzzer.o gyro.o client.o 
	echo "[make libMyPeri.a]"

main.elf : main.c libMyPeri.a
	arm-linux-gnueabi-gcc -o main.elf main.c -l MyPeri -L. -lpthread
	echo "[make main.elf]"