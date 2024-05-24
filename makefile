all: server client

server: server.c
	gcc -o server server.c

client: client.c
	gcc -o client client.c

clean:
	-rm server client excercise.pb.h excercise.pb.c

excercise.pb.h excercise.pb.c: excercise.proto
	python3 nanopb/generator/nanopb_generator.py excercise.proto
