
all:	main.o STConnection.o STMessage.o STInputFormat.o LIST.o
	gcc -o s-talk main.o STConnection.o STMessage.o STInputFormat.o LIST.o

main.o:	main.c
	gcc -c main.c

STConnection.o:	STConnection.c
	gcc -c STConnection.c STConnection.h

STMessage.o: STMessage.c
	gcc -c STMessage.c STMessage.h

STInputFormat.o: STInputFormat.c
	gcc -c STInputFormat.c STInputFormat.h

LIST.o: LIST.c
	gcc -c LIST.c LIST.h

clean:	
	rm -f *.o *.s *.gch s-talk
