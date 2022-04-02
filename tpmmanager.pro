CONFIG += build_all \
 console \
 qt \
 release \
 warn_on
DEPENDPATH += . microtss src
DESTDIR = bin
FORMS = ui/NewPasswordDialog.ui \
 ui/PasswordDialog.ui \
 ui/PublicKeyView.ui \
 ui/SetSRKDialog.ui \
 ui/TPMManagerBase.ui
HEADERS = src/NewPasswordDialog.hxx \
 src/PublicKeyView.hxx \
 src/SetSRKView.hxx \
 src/microtss/PublicKey.hxx \
 src/microtss/TPM.hxx \
 src/microtss/TSS.hxx \
 src/TPMManager.hxx \
 src/PasswordDialog.hxx
INCLUDEPATH += . microtss src
LIBS += -ltspi
MOC_DIR = build
OBJECTS_DIR = build
QT += core gui widgets
SOURCES = src/NewPasswordDialog.cxx \
 src/PublicKeyView.cxx \
 src/SetSRKView.cxx \
 src/Main.cxx \
 src/microtss/PublicKey.cxx \
 src/microtss/TPM.cxx \
 src/microtss/TSS.cxx \
 src/TPMManager.cxx \
 src/PasswordDialog.cxx
RESOURCES += ui/resources.qrc
TEMPLATE = app
UI_DIR = build
