CONFIG += build_all \
 console \
 qt \
 release \
 warn_on
DEPENDPATH += . microtss src
DESTDIR = bin
FORMS = ui/NewPasswordDialog.ui \
 ui/PasswordDialog.ui \
 ui/publickeyview.ui \
 ui/setsrkdialog.ui \
 ui/tpmmanagerbase.ui
HEADERS = src/NewPasswordDialog.h \
 src/PublicKeyView.h \
 src/SetSRKView.h \
 src/microtss/PublicKey.h \
 src/microtss/TPM.h \
 src/microtss/TSS.h \
 src/tpmmanager.h \
 src/PasswordDialog.h
INCLUDEPATH += . microtss src
LIBS += -ltspi
MOC_DIR = build
OBJECTS_DIR = build
QT = core gui
SOURCES = src/NewPasswordDialog.cpp \
 src/PublicKeyView.cpp \
 src/SetSRKView.cpp \
 src/main.cpp \
 src/microtss/PublicKey.cpp \
 src/microtss/TPM.cpp \
 src/microtss/TSS.cpp \
 src/tpmmanager.cpp \
 src/PasswordDialog.cpp
RESOURCES += ui/resources.qrc
TEMPLATE = app
UI_DIR = build
