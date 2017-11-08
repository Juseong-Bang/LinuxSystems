CC=gcc

OBJECT1 =ssu_vim.o
OBJECT2 =ssu_ofm.o

NAME1 = vim
NAME2 = ofm

Target : $(OBJECT1) $(OBJECT2)
	$(CC) -Wall -W -o $(NAME1) $(OBJECT1) -g
	$(CC) -Wall -W -o $(NAME2) $(OBJECT2) -g

clean : 
	rm -rf $(OBJECT1) $(OBJECT2) $(NAME1) $(NAME2)

ssu_vim.o :ssu_vim.c ssu_header.h
	gcc -c ssu_vim.c


ssu_ofm.o :ssu_ofm.c ssu_header.h
	gcc -c ssu_ofm.c
