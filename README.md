# TPM Manager

The goal of this project is to provide a TPM management software with an easy-to-use graphical user interface.
The TPM Manager targets Linux, but later releases should be usable with all operating systems providing a TSS API.

## Features

* TPM State
 * Show TPM state
 * Show TPM & TSS vendor info
 * Show TPM capabilities
 * Show Platform Configuration Registers (PCRs)
* TPM Owner Settings
 * Take ownership
 * Change owner password
 * Change Storage Root Key (SRK) password
 * Factory reset
 * Create maintenance backup (*)
* TPM Settings
 * Temporarily deactivate
 * Permanently disable
 * Manage Endorsement Key (EK)
 * Perform TPM selftest
 * Show Endorsement Key (EK)
* Advanced Settings
 * Disable maintenance feature (*)
 * Delete Endorsement Key (EK) (*)

(*) Requires support by TPM

![Screenshot](http://i.imgur.com/OwXH1Ye.png)

## Requirements

Since the TPM Manager is based entirely on the Qt UI framework, corresponding Qt header and library ﬁles should be in the library path.
On some Linux distributions you have to install the developer version of Qt to have the header files used by TPM Manager.

Requirements to build the TPM Manager: 

* Qt5 (Builds under 5.15.3) (http://qt.io)
* TrouSerS (http://trousers.sourceforge.net/)

Install the following packages on a Debian-based Linux, e.g., Ubuntu Linux:

```bash
sudo apt-get install build-essential libtspi-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```
For a Fedora based Linux install the following packages:

```bash
sudo dnf install make automake gcc gcc-c++ qt5-qtbase-devel trousers-devel
```

To use the features of the TPM Manager you need a running TrouSerS daemon.

## Building

This package is built using Qt's qmake. qmake automatically generates a makefile from the Qt project file.

```bash
qmake-qt5 (or simply: qmake)
make
```

## Installation

Make will create the TPM Manager application binary in the bin/ subdirectory.
You can start TPM Manager right from the /bin subdirectory or
you can run `install` to install TPM Manager in default binary path of your system.

```bash
# install to /usr/local/bin
sudo install bin/tpmmanager /usr/local/bin
```

## Usage

To start TPM Manager, type `tpmmanager` on your terminal or simply double-click the application icon in your file manager.
You may also add TPM Manager to your Application starter menu, e.g., in Gnome or KDE.
