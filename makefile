bitcoin: main.o senderHashTable.o receiverHashTable.o tree.o wallets.o functions.o handleInput.o
	gcc -g -Wall main.o senderHashTable.o receiverHashTable.o tree.o functions.o handleInput.o wallets.o -o bitcoin

main.o: main.c
	gcc -c -g main.c

senderHashTable.o: senderHashTable.c ./headers/sender.h
	gcc -c -g senderHashTable.c

receiverHashTable.o: receiverHashTable.c ./headers/receiver.h
	gcc -c -g receiverHashTable.c

tree.o: tree.c ./headers/tree.h
	gcc -c -g tree.c

functions.o: functions.c ./headers/functions.h
	gcc -c -g functions.c

handleInput.o: handleInput.c ./headers/handleInput.h
	gcc -c -g handleInput.c

wallets.o: wallets.c ./headers/wallets.h
	gcc -c -g wallets.c

clean:
	rm -rf *.o bitcoin
