CC=gcc

OBJECTS = proc_main.o proc_opt.o proc_f.o proc_t.o proc_c.o proc_n.o proc_m.o proc_w.o proc_e.o proc_l.o proc_v.o proc_r.o proc_s.o

NAME = lsproc

Target : $(OBJECTS)
	$(CC) -Wall -W -o $(NAME) $(OBJECTS) -g

clean : 
	rm -rf $(OBJECTS) $(NAME)

proc_main.o :proc_main.c proc_header.h
	gcc -c proc_main.c
proc_opt.o :proc_opt.c
	gcc -c proc_opt.c
proc_f.o :proc_f.c
	gcc -c proc_f.c
proc_t.o :proc_t.c
	gcc -c proc_t.c
proc_c.o :proc_c.c
	gcc -c proc_c.c
proc_n.o :proc_n.c
	gcc -c proc_n.c
proc_m.o :proc_m.c
	gcc -c proc_m.c
proc_w.o :proc_w.c
	gcc -c proc_w.c
proc_e.o :proc_e.c
	gcc -c proc_e.c
proc_l.o :proc_l.c
	gcc -c proc_l.c
proc_v.o :proc_v.c
	gcc -c proc_v.c
proc_r.o :proc_r.c
	gcc -c proc_r.c
proc_s.o :proc_s.c
	gcc -c proc_s.c
