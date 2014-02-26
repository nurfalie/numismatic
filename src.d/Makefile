CHOWN		= chown -R www-data:www-data
GCC		= gcc
OPTIONS		= -Wall -Werror -Wextra -Wformat \
		  -Wstack-protector \
		  -fPIE -fstack-protector-all \
		  -fwrapv -pedantic -pie

all:		add \
		delete \
		edit \
		load_all \
		print

CGI_PATH	= /usr/lib/cgi-bin/numismatic
INCLUDES	= common.h

ADD_SRCS	= add.c
DELETE_SRCS	= delete.c
EDIT_SRCS 	=  edit.c
LOAD_ALL_SRCS 	= load_all.c
PRINT_SRCS	= print.c

INCLUDE_PATH	= -I/usr/include -I/usr/local/include
LIBS		= -lmysqlclient

add:		$(ADD_SRCS) $(INCLUDES)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o add $(ADD_SRCS) $(LIBS)

delete:		$(DELETE_SRCS) $(INCLUDES)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o delete $(DELETE_SRCS) \
		$(LIBS)

edit:		$(EDIT_SRCS) $(INCLUDES)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o edit $(EDIT_SRCS) \
		$(LIBS)

load_all:	$(LOAD_ALL_SRCS) $(INCLUDES)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o load_all \
		$(LOAD_ALL_SRCS) $(LIBS)

print:		$(PRINT_SRCS) $(INCLUDES)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o print $(PRINT_SRCS) \
		$(LIBS)

clean:
	rm -rf add
	rm -rf delete
	rm -rf edit
	rm -rf load_all
	rm -rf print
	rm -rf *.core

distclean: clean purge

install:
	mkdir -p $(CGI_PATH)
	cp -p ./add $(CGI_PATH)/add.cgi
	cp -p ./delete $(CGI_PATH)/delete.cgi
	cp -p ./edit $(CGI_PATH)/edit.cgi
	cp -p ./load_all $(CGI_PATH)/load_all.cgi
	cp -p ./print $(CGI_PATH)/print.cgi
	$(CHOWN) $(CGI_PATH)

purge:
	rm -rf *~
