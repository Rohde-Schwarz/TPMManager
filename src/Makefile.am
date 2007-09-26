# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = PublicKeyView.hh

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
tpmmanager_SOURCES = 	tpmmanagerwidgetbase.ui \
			publickeyview.ui \
			main.cpp \
			tpmmanagerwidget.cpp 	 \
			PublicKeyView.cpp

tpmmanager_LDFLAGS = $(KDE_RPATH) $(all_libraries) -L$(top_builddir)/src/lib
tpmmanager_LDADD = $(LIB_KDEUI) -lTPMmanager -ltspi

# this is where the desktop file will go 
shelldesktopdir = $(kde_appsdir)/Utilities
shelldesktop_DATA = tpmmanager.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir = $(kde_datadir)/tpmmanager
shellrc_DATA = tpmmanagerui.rc

SUBDIRS = lib

AM_CXXFLAGS = -I$(top_builddir)/src/lib/
