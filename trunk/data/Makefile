include ../Makefile.common

MKDIR = mkdir -p
CP = cp -p -f
RM = rm -r -f

install:force 
	${MKDIR} $(prefix)/share/flamingtux/glade/
	${CP} glade/* $(prefix)/share/flamingtux/glade/

force:

all:

clean:

distclean: force
	   $(RM) $(prefix)/share/flamingtux/glade/
