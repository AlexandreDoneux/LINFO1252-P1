all: phil prod read
	bash caller.sh

phil: src/philosophe.c
	gcc src/philosophe.c -o src/phil

prod: src/producer_consumer.c
	gcc src/producer_consumer.c -o src/prod

read: src/reader_writer.c
	gcc src/reader_writer.c -o src/read

clean:
	rm -f src/phil src/prod src/read
