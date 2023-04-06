FLAGS = -g -Wall -pthread

all:
	gcc $(FLAGS) src/client.c src/chat.c -Iinc -o bin/chatClient
	gcc $(FLAGS) src/server.c src/chat.c -Iinc -o bin/chatServer

clean:
	rm bin/*