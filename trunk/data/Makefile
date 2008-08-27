include ../Makefile.common

MKDIR = mkdir -p
CP = cp -p -f
RM = rm -r -f

install:force 
	${MKDIR} $(prefix)/share/flamingtux/glade/
	${MKDIR} $(prefix)/share/flamingtux/sounds/
	${CP} glade/* $(prefix)/share/flamingtux/glade/
	${CP} sounds/* $(prefix)/share/flamingtux/sounds/
force:

all:

clean:

distclean: force
	   ${RM} $(prefix)/share/flamingtux/glade/
	   ${RM} $(prefix)/share/flamingtux/sounds/
