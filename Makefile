all: phil prod read
	bash caller.sh

phil: src/posix/philosophe.c
	gcc src/posix/philosophe.c -o src/posix/phil

prod: src/posix/producer_consumer.c
	gcc src/posix/producer_consumer.c -o src/posix/prod

read: src/posix/reader_writer.c
	gcc src/posix/reader_writer.c -o src/read

test1: src/spinlock/spinlock.c src/spinlock/test-and-set.c
	gcc -c src/spinlock/spinlock.c
	gcc -c src/spinlock/test-and-set.c
	gcc -o src/spinlock/test1 spinlocks.o test-and-set.o

clean:
	rm -f src/phil src/prod src/read
