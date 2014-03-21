DEBUG?=0
ifeq ($(DEBUG),1)
        CFLAGS =-DDEBUG
else
        CFLAGS =-DNDEBUG
endif
maxWeight: maxWeightLib.o my402list.o maxWeight.o
	gcc $(CFLAGS) -o maxWeight -g -Wall maxWeightLib.o my402list.o maxWeight.o
maxWeight.o: maxWeight.c maxWeight.h
	gcc $(CFLAGS) -g -c maxWeight.c
maxWeightLib.o: maxWeightLib.c
	gcc $(CFLAGS) -g -c -Wall maxWeightLib.c

my402list.o: my402list.c my402list.h
	gcc $(CFLAGS) -g -c -Wall my402list.c
clean:
	rm -f *.o maxWeight 
