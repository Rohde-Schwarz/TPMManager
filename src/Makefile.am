# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = PublicKeyView.hh SetSrkView.hh testappwidget.hh \
	tpmmanager.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/tpmmanager.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = tpmmanager

# the application source, library search path, and link libraries
tpmmanager_SOURCES = PublicKeyView.cpp SetSrkView.cpp \
	main.cpp publickeyview.ui setsrkdialog.ui testappwidget.cpp \
	testappwidgetbase.ui tpmmanager.cpp tpmmanagerwidget.cpp tpmmanagerwidgetbase.ui
tpmmanager_LDFLAGS = $(KDE_RPATH) $(all_libraries)
tpmmanager_LDADD = $(top_builddir)/src/microtss/libmicrotss.a -ltspi \
	$(LIB_KDEUI)

# this is where the desktop file will go 
# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/tpmmanager.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = tpmmanager

# the application source, library search path, and link libraries
tpmmanager_LDFLAGS = $(KDE_RPATH) $(all_libraries)
tpmmanager_LDADD = $(top_builddir)/src/microtss/libmicrotss.a -ltspi \
	$(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = tpmmanager.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/tpmmanager
shellrc_DATA = tpmmanagerui.rc

SUBDIRS = microtss
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = tpmmanager.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/tpmmanager
shellrc_DATA = tpmmanagerui.rc

SUBDIRS = microtss
