CGI_PATH	=	/usr/lib/cgi-bin/numismatic
OPTIONS		=	-Wall -Wextra -Wstack-protector -fPIE -fstack-protector-all -pedantic -pie
GCC		=	gcc
CP		=	cp
RM		=	rm
LINT		=	lint
CHOWN		=	chown -R www-data:www-data
MKDIR		=	mkdir -p

standard:	add \
		delete \
		edit \
		load_all \
		print

INCLUDES	=	common.h
LOAD_ALL_SRCS 	=	load_all.c
EDIT_SRCS 	= 	edit.c
DELETE_SRCS	=	delete.c
ADD_SRCS	=	add.c
PRINT_SRCS	=	print.c

INCLUDE_PATH	=	-I/usr/include -I/usr/local/include
LIB		=	-L/usr/lib -lmysqlclient

load_all:	$(INCLUDES) $(LOAD_ALL_SRCS)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o load_all \
		$(LOAD_ALL_SRCS) $(LIB)

edit:		$(INCLUDES) $(EDIT_SRCS)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o edit $(EDIT_SRCS) $(LIB)

delete:		$(INCLUDES) $(DELETE_SRCS)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o delete $(DELETE_SRCS) \
		$(LIB)

add:		$(INCLUDES) $(ADD_SRCS)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o add $(ADD_SRCS) $(LIB)

print:		$(INCLUDES) $(PRINT_SRCS)
		$(GCC) $(OPTIONS) $(INCLUDE_PATH) -o print $(PRINT_SRCS) \
		$(LIB)

install:
		$(MKDIR) $(CGI_PATH)
		$(CP) ./load_all $(CGI_PATH)/load_all.cgi
		$(CP) ./edit $(CGI_PATH)/edit.cgi
		$(CP) ./delete $(CGI_PATH)/delete.cgi
		$(CP) ./add $(CGI_PATH)/add.cgi
		$(CP) ./print $(CGI_PATH)/print.cgi
		$(CHOWN) $(CGI_PATH)

lint:	$(LOAD_ALL_SRCS) $(EDIT_SRCS) $(DELETE_SRCS) $(ADD_SRCS)
	$(LINT) $(INCLUDE_PATH) $>

clean:
	$(RM) -rf load_all
	$(RM) -rf edit
	$(RM) -rf delete
	$(RM) -rf add
	$(RM) -rf print
	$(RM) -rf *.core

purge:
	$(RM) -rf *~
