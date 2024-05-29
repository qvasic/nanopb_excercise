all: server client

server: server.c excercise.pb.h excercise.pb.c
	gcc -o server server.c excercise.pb.c -Inanopb nanopb/pb_encode.c nanopb/pb_common.c

client: client.c excercise.pb.h excercise.pb.c
	gcc -o client client.c excercise.pb.c -Inanopb

clean:
	-rm server client excercise.pb.h excercise.pb.c

excercise.pb.h excercise.pb.c: excercise.proto
	python3 nanopb/generator/nanopb_generator.py excercise.proto
