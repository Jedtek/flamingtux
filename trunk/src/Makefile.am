METASOURCES = AUTO
bin_PROGRAMS = flamingtux
flamingtux_SOURCES = Application.cpp BackendEvents.cpp BuddyListWin.cpp \
	CommonFunctions.cpp Config.cpp ConfigOptions.cpp ConvoWin.cpp DialogWin.cpp FireClient.cpp \
	GtkHelperFunctions.cpp Log.cpp LoginWin.cpp Main.cpp PreferencesWin.cpp
flamingtux_LDADD = @LIBGLADEMM_LIBS@ @GTKSPELL_LIBS@ @GTHREAD_LIBS@ @GTK_LIBS@ \
	@GTKMM_LIBS@ @LIBS@ -lxfirelib
AM_CFLAGS = @LIBS_CFLAGS@ @GTKMM_CFLAGS@ @GTK_CFLAGS@ @GTHREAD_CFLAGS@ \
	@LIBGLADEMM_CFLAGS@ @GTKSPELL_CFLAGS@
AM_CXXFLAGS = @GTKMM_CFLAGS@ @GTK_CFLAGS@ @GTHREAD_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GTKSPELL_CFLAGS@
