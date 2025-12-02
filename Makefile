all: phil prod read test1 test2 spinlock_phil spinlock_prod spinlock_read
	bash caller.sh

phil: src/posix/philosophe.c
	gcc src/posix/philosophe.c -o src/posix/phil

prod: src/posix/producer_consumer.c
	gcc src/posix/producer_consumer.c -o src/posix/prod

read: src/posix/reader_writer.c
	gcc src/posix/reader_writer.c -o src/posix/read

test1: src/spinlock/spinlock.c src/spinlock/test-and-set.c
	gcc -c src/spinlock/spinlock.c -o src/spinlock/spinlock.o
	gcc -c src/spinlock/test-and-set.c -o src/spinlock/test-and-set.o
	gcc -o src/spinlock/test1 src/spinlock/spinlock.o src/spinlock/test-and-set.o

test2: src/spinlock/spinlock.c src/spinlock/test-and-test-and-set.c
	gcc -c src/spinlock/spinlock.c -o src/spinlock/spinlock.o
	gcc -c src/spinlock/test-and-test-and-set.c -o src/spinlock/test-and-test-and-set.o
	gcc -o src/spinlock/test2 src/spinlock/spinlock.o src/spinlock/test-and-test-and-set.o

spinlock_phil: src/spinlock/spinlock.c src/spinlock/mysem.c src/spinlock/philosophe.c
	gcc -c src/spinlock/spinlock.c -o src/spinlock/spinlock.o
	gcc -c src/spinlock/mysem.c -o src/spinlock/mysem.o
	gcc -c src/spinlock/philosophe.c -o src/spinlock/philosophe.o
	gcc -o src/spinlock/phil src/spinlock/spinlock.o src/spinlock/mysem.o src/spinlock/philosophe.o

spinlock_prod: src/spinlock/spinlock.c src/spinlock/mysem.c src/spinlock/producer_consumer.c
	gcc -c src/spinlock/spinlock.c -o src/spinlock/spinlock.o
	gcc -c src/spinlock/mysem.c -o src/spinlock/mysem.o
	gcc -c src/spinlock/producer_consumer.c -o src/spinlock/producer_consumer.o
	gcc -o src/spinlock/prod src/spinlock/spinlock.o src/spinlock/mysem.o src/spinlock/producer_consumer.o

spinlock_read: src/spinlock/spinlock.c src/spinlock/mysem.c src/spinlock/reader_writer.c
	gcc -c src/spinlock/spinlock.c -o src/spinlock/spinlock.o
	gcc -c src/spinlock/mysem.c -o src/spinlock/mysem.o
	gcc -c src/spinlock/reader_writer.c -o src/spinlock/reader_writer.o
	gcc -o src/spinlock/read src/spinlock/spinlock.o src/spinlock/mysem.o src/spinlock/reader_writer.o

clean:
	rm -f src/posix/phil src/posix/prod src/posix/read src/posix/*.o src/spinlock/test1 src/spinlock/test2 src/spinlock/*.o
