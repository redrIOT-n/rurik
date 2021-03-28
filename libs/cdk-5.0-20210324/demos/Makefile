# $Id: Makefile.in,v 1.21 2020/09/23 20:27:42 tom Exp $
#
# Makefile for the demos directory.
#
SHELL		= /bin/sh

prefix		= /usr/local
exec_prefix	= ${prefix}

srcdir		= .


CC		= gcc
RM		= rm -f
CTAGS		= 
ETAGS		= 
LINT		= 
LINT_OPTS	= 

LIBS		= -lcdk -lncurses 

LIBTOOL		=  
LIBTOOL_CLEAN	= 
LIBTOOL_LINK	= ${CC}
LINK		= $(LIBTOOL_LINK)

x		= 
o		= .o
LOCAL_LIBDIR	= .

CFLAGS		= -g -O2 
CPPFLAGS	= -DHAVE_CONFIG_H -I../include -I$(srcdir)/../include -I. -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500
LDFLAGS		= -L.. -L/lib  

BINS		= \
		fileview$x \
		rolodex$x \
		command$x \
		clock$x \
		stopSign$x \
		appointment$x \
		vinstall$x

# this needs configure-script work:
OTHER_BINS	= \
		serial$x

CDKSRC		= \
		fileview.c \
		rolodex.c \
		command.c \
		clock.c \
		stopSign.c \
		appointment.c \
		vinstall.c \
		serial.c

# If you have Sybase installed on your system, try making
# syb. It's a full screen interface to Sybase.
#
SYBROOT		= $(SYBASE)
SYBINCDIR	= -I$(SYBROOT)/include
SYBLIBDIR	= -L$(SYBROOT)/devlib
SYBLIBS		= -lnsl -lsybdb

.PHONY: all clean distclean lint

all : $(BINS)

#
# Standard .c to .o compile line.
#
.c.o:
	
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
#
# Most of the examples are built from a single .c file using the same libraries
#
LINKIT	= $(LINK) $(CFLAGS) $(CPPFLAGS) $? -o $@ $(LDFLAGS) $(LIBS)

# fileview    - Basic file selector/file viewer combination.
fileview$x :	fileview.o	; $(LINKIT)

# rolodex     - A fairly robust rolodex.
rolodex$x :	rolodex.o	; $(LINKIT)

# command     - A basic command interface with command history.
command$x :	command.o	; $(LINKIT)

# clock       - A simple little clock.
clock$x :	clock.o	; $(LINKIT)

# stopSign    - A simple demo I wrote after a couple of pints. :)
stopSign$x :	stopSign.o	; $(LINKIT)

# appointment - A basic appointment book/date keeper.
appointment$x :	appointment.o	; $(LINKIT)

# serial      - This probes the serial port.
serial$x :	serial.o	; $(LINKIT)

# keycheck    - This prints out the keys ASCII and Octal values.
keycheck$x :	keycheck.o	; $(LINKIT)

# vinstall    - CDK based installer.
vinstall$x :	vinstall.o	; $(LINKIT)

#
# A Sybase interface.
#
syb$x :		syb.c
	$(CC) $(CFLAGS) -o $@ $? $(CPPFLAGS) $(SYBINCDIR) $(LDFLAGS) $(SYBLIBDIR) $(LIBS) $(SYBLIBS)

#
# Standard clean directives.
#
clean::
	-$(LIBTOOL_CLEAN) $(RM) *.o core $(BINS) $(OTHER_BINS)

distclean::	clean
	-rm -f Makefile

#tags :
#	$(CTAGS) *.[ch] */*.[ch]

#TAGS :
#	$(ETAGS) *.[ch] */*.[ch]

lint: $(CDKSRC)
	$(LINT) $(LINT_OPTS) $(CPPFLAGS) $(CDKSRC)
