main:main.o update_imu.o 
	gcc -o main main.o update_imu.o
main.o:main.c
	gcc -c main.c
update_imu.o:update_imu.c update_imu.h
	gcc -c update_imu.c
clean:
	rm -f *.o main
