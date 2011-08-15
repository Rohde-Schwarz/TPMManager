/***************************************************************************
 *   Copyright (C) 2006-2009 Sirrix AG                                     *
 *   Authors:                                                              *
 *	 Anoosheh Zaerin <a.zaerin@sirrix.com>                                 *
 *   René Korthaus <r.korthaus@sirrix.com>                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
* @file TPMManager.cxx
*
* @brief TPM Manager Controller Class Implementation File
*
**/

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>

#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QTabWidget>
#include <QString>
#include <QDesktopServices>
#include <QUrl>
// 
#include <microtss/TSS.hxx>
#include <microtss/PublicKey.hxx>
#include <SetSRKView.hxx>
#include <PublicKeyView.hxx>
#include <PasswordDialog.hxx>
#include <NewPasswordDialog.hxx>

#include "TPMManager.hxx"

#define VERSION "0.8.1"
//
using namespace std;
using namespace microtss;

TPM_Manager::TPM_Manager( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f), 
	myTSS( 0 ),
  	myTPM( 0 ),
  	myOkImage ( ":images/images/ok.png" ),
  	myUnknownImage( ":images/images/unk.png" ),
  	myNokImage( ":images/images/nok.png" ),
	myTimer()
{
    setupUi(this);
	
    myProgramLabel->setText( QString::fromAscii("TPM Manager v") + QString::fromAscii(VERSION) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( close() ) );
   
    // connect our custom slots manually
    connect ( downloadLink, SIGNAL( linkActivated(const QString&) ), this, SLOT( slotProcessURL(const QString& ) ) );
    connect ( perseusLink, SIGNAL( linkActivated(const QString&) ), this, SLOT( slotProcessURL(const QString& ) ) );
    // we can connect all on_(QWidgetName)_(SIGNAL)(Arguments) slots automagically with one function call (req. Qt 4.4)
    QMetaObject::connectSlotsByName( parent );
   
    struct stat Status;
    /*return 0 if the file is found.
     *return -1 if the file is not found.
     */
    int tpmdriver = stat( "/dev/tpm", &Status);
    int tpm0driver = stat( "/dev/tpm0", &Status);
    if (tpmdriver == 0 || tpm0driver == 0)
        driverFound->setPixmap( myOkImage );
    else
        driverFound->setPixmap( myNokImage );

    try {
        myTSS = new TSS;
        myTPM = new TPM( myTSS->getContextHandle() );

        tssFound->setPixmap( myOkImage );

        if ( myTPM->isDisabled() && !myTPM->hasOwner() ) {
            QMessageBox::critical( this, "TPM Not Functional", "The TPM is disabled and no TPM Owner is set. You have to enable the TPM in the BIOS to use the functions of the TPM, e.g., to take ownership.\n" );
        }

    } catch ( TPMDriverNotFound &e ) {
        QMessageBox::critical( this, "No TPM Driver Found", "A TPM device driver could not be found.\nTPM Manager will quit." );
        driverFound->setPixmap( myNokImage );
        tssFound->setPixmap( myUnknownImage );
        exit(1);
    } catch ( TSSSystemNotFound &e ) {
        QMessageBox::critical( this, "No TSS Found", "A TCG Software Stack (TSS) could not be found. When using TrouSerS, make sure the TrouSerS daemon (tcsd) is running.\nTPM Manager will quit." );
        exit(1);
    } catch ( ... ) {
        QMessageBox::critical( this, "Unknown Error", "An unknown error occured during initialization." );
    }
	
    // Init status & first view
    initStatusGroup();
    initStatus();
    // let the PCRs update every second..
    connect( &myTimer, SIGNAL( timeout() ), this, SLOT( slotUpdatePCRs() ) );
    myTimer.start( 1000 );
}

TPM_Manager::~TPM_Manager()
{
	delete myTPM;  /// @todo another design without pointer to TPM/TSS
	delete myTSS;
}
//
/**
 * Initialize method implementations
 */
void TPM_Manager::initStatus()
{
	if ( !hasTSS() || !hasTPM() ) {
	  tssFound->setPixmap( myNokImage );
	  tpmActivated->setPixmap( myUnknownImage );
	  tpmEnabled->setPixmap( myUnknownImage );
	  ownerSet->setPixmap( myUnknownImage );
	  myEndorsementAvailable->setPixmap( myUnknownImage );
	  return;
   }

	/// TPM Enable/Active/Owner state
	if ( myTPM->isEnabled() ) {
		tpmEnabled->setPixmap( myOkImage );
		if ( myTPM->isActivated() )
			tpmActivated->setPixmap( myOkImage );
		else
			tpmActivated->setPixmap( myNokImage );
	}
	else {
		tpmEnabled->setPixmap( myNokImage );
		tpmActivated->setPixmap( myUnknownImage );
	}

	if ( myTPM->hasOwner() )
		ownerSet->setPixmap( myOkImage );
	else
		ownerSet->setPixmap( myNokImage );

	if ( myTPM->hasEndorsementKey() )
		myEndorsementAvailable->setPixmap( myOkImage );
	else
		myEndorsementAvailable->setPixmap( myNokImage );
}

void TPM_Manager::initStatusGroup()
{
	if ( !hasTPM() ) {
		myEnabledLabel->setPixmap( myUnknownImage );
		myActivatedLabel->setPixmap( myUnknownImage );
		myOwnerLabel->setPixmap( myUnknownImage );	
		return;
   }

  	if ( myTPM->isEnabled() ) {
		myEnabledLabel->setPixmap( myOkImage );
		if ( myTPM->isActivated() )
			myActivatedLabel->setPixmap( myOkImage );
		else
			myActivatedLabel->setPixmap( myNokImage );
	}
	else {
		myEnabledLabel->setPixmap( myNokImage );
		myActivatedLabel->setPixmap( myUnknownImage );
	}

	if ( myTPM->hasOwner() )
		myOwnerLabel->setPixmap( myOkImage );
	else
		myOwnerLabel->setPixmap( myNokImage );
}

void TPM_Manager::initCapabilities()
{
	/// set Capabilities
	myCapabilities->clear(); // clear table
	myCapabilities->setRowCount( 2 );

	if ( hasTPM() ) {
		QTableWidgetItem * item1 = new QTableWidgetItem( "Number of PCRs" );
		myCapabilities->setItem(0, 0, item1);
		QTableWidgetItem * item2 = new QTableWidgetItem( QString("%1").arg( myTPM->getNumberOfPCR() ) );
		myCapabilities->setItem(0, 1, item2);

		QTableWidgetItem * item3 = new QTableWidgetItem( "Number of 2048-bit RSA keys that can be loaded" );
		myCapabilities->setItem(1, 0, item3);
		QTableWidgetItem * item4 = new QTableWidgetItem ( QString("%1").arg( myTPM->getKeyLoadCount()) );
		myCapabilities->setItem(1, 1, item4 );
	}
	myCapabilities->resizeColumnToContents( 0 );
	myCapabilities->verticalHeader()->stretchLastSection();
	
	
	
	
}

void TPM_Manager::initDetails()
{
	/// TSS Details
	if ( hasTSS() ) {
	  tssVendor->setText( QString::fromStdString( myTSS->getVendorName() ) );
	  QString version = QString::fromStdString( myTSS->getVersion() ) + ", rev " 
                                       + QString::fromStdString( myTSS->getRevision() );
	  tssVersion->setText( version );
	}

	/// TPM Details
	if ( hasTPM() ) {
	  tpmVendor->setText( QString::fromStdString( myTPM->getVendorName() ) );
	  tpmVersion->setText( QString::fromStdString( myTPM->getVersion() ) );
	  tpmFirmware->setText( QString::fromStdString( myTPM->getRevision() ) );
   }
}

void TPM_Manager::initPCRs()
{
	if ( !hasTPM() )
	  return;

	ostringstream pcrStr;
	vector<ByteString> pcrValues = myTPM->getPCRValues();

	myPCRs->clear();

	for( size_t i=0; i < pcrValues.size(); ++i)
	{
		pcrStr << "PCR[" << dec << setw( 2 ) << setfill( '0' ) << i << "] "; 
		
		for ( size_t j=0; j<pcrValues[i].size(); ++j) {
			if ((j % 4) == 0)
				pcrStr << " ";
			pcrStr << hex << setw( 2 ) << setfill( '0' ) << (int) pcrValues[i][j];
			
		}
		myPCRs->insertItem(i, QString::fromStdString( pcrStr.str() ) );	
		pcrStr.str("");
	}
}

void TPM_Manager::initOwnership()
{
	if ( !hasTPM() )
	  return;

	myTakeOwnership->setEnabled( myTPM->isEnabled() && !myTPM->hasOwner() );
	
	myChangePassword->setEnabled( myTPM->isEnabled() && myTPM->isActivated() && myTPM->hasOwner() );
        myChangeSRKPassword->setEnabled( myTPM->isEnabled() && myTPM->isActivated() && myTPM->hasOwner() );
	myClearOwnership->setEnabled( myChangePassword->isEnabled() );
	
	myTakeOwnerText->setEnabled( myTakeOwnership->isEnabled() );
	myChangePassText->setEnabled( myChangePassword->isEnabled() );
        myChangeSRKPassText->setEnabled( myChangeSRKPassword->isEnabled() );
	myClearOwnerText->setEnabled( myClearOwnership->isEnabled() );
}

void TPM_Manager::initBackup()
{
	if ( !hasTPM() )
	  return;

	myCreateMaintenance->setEnabled( myTPM->hasMaintenance() );
	myCreateMaintenanceText->setEnabled( myTPM->hasMaintenance() );

	myLoadMaintenance->setEnabled( myTPM->hasMaintenance() );
	myLoadMaintenanceText->setEnabled( myTPM->hasMaintenance() );
	
	if ( myTPM->hasMaintenance() ) {
		myMaintenanceInfoText->hide();
		myMaintenanceInfoIcon->hide();
	}
   else {
		myMaintenanceInfoText->show();
		myMaintenanceInfoIcon->show();
	}
}

void TPM_Manager::initOperationalModes()
{
	if ( !hasTPM() ) {
	  	return;
  	}
  		
	myDisable->setEnabled( myTPM->hasOwner() );

	if ( myTPM->isEnabled() ) {
		myDisable->setText( "Disable" );
		myDisableLabel->setText( "Disable the TPM" );
    myDeactivate->setEnabled( myTPM->isActivated() );
	} 
   else {
		myDisable->setText( "Enable" );
		myDisableLabel->setText( "Enable the TPM" );
    myDeactivate->setDisabled( myTPM->isActivated() );
   }

//	myDeactivate->setEnabled( myTPM->isActivated() );
}

void TPM_Manager::initIdentity()
{
	if ( !hasTPM() )
	  return;

	bool hasEK = myTPM->hasEndorsementKey();
	myCreateEndorsement->setEnabled( !hasEK && !myTPM->isDisabled() );
	myShowEndorsement->setEnabled( hasEK && !myTPM->isDisabled() && !myTPM->isDeactivated() );
	mySaveEndorsement->setEnabled( hasEK && !myTPM->isDisabled() && !myTPM->isDeactivated() );
	myRestrictEndorsement->setEnabled( hasEK && myTPM->hasOwner() 
													&& !myTPM->isEndorsementKeyResticted()
													&& !myTPM->isDisabled()  );
//	myReadCertificate->setEnabled( hasEK );
}

void TPM_Manager::initSelfTest() {
	if ( !hasTPM() )
	  return;

	mySelfTest->setEnabled( true );
}

void TPM_Manager::initReadCertificate()
{
	if ( !hasTPM() )
	  return;
	/*
	myReadCertificateText->setEnabled( myTPM->hasEndorsementKey() );
	myReadCertificate->setEnabled( myTPM->hasEndorsementKey() );
	if ( myTPM->hasEndorsementKey() )
	{
		myReadCertificateInfoIcon->hide();
		myReadCertificateInfoText->hide();
	}
	else
	{
		myReadCertificateInfoIcon->show();
		myReadCertificateInfoText->show();
	}
	*/
}

void TPM_Manager::initDisableMaintenance()
{
	if ( !hasTPM() )
	  return;

	myDisableMaintenance->setEnabled( myTPM->hasMaintenance() );
	myDisableMaintenanceText->setEnabled( myTPM->hasMaintenance() );
	
	if( myTPM->hasMaintenance() ) {
		myDisableMaintenanceInfoIcon->hide();
		myDisableMaintenanceInfoText->hide();
	} else {
		myDisableMaintenanceInfoIcon->show();
		myDisableMaintenanceInfoText->show();
	}
}

void TPM_Manager::initRevokeTrust()
{
	if ( !hasTPM() )
	  return;

	/*myDeleteEndorsement->setEnabled( myTPM->hasEndorsementKey() );
	myDisableMaintenanceText->setEnabled( myTPM->hasEndorsementKey() );
	myRevokeWarnIcon->setEnabled( myTPM->hasEndorsementKey() );
	myRevokeWarnText->setEnabled( myTPM->hasEndorsementKey() );
	if ( myTPM->hasEndorsementKey() )
	{
		myRevokeInfoIcon->hide();
		myRevokeInfoText->hide();
	} else
	{
		myRevokeInfoIcon->show();
		myRevokeInfoText->show();
	}*/
}

void TPM_Manager::slotUpdatePCRs()
{
	initPCRs();
}

void TPM_Manager::slotProcessURL( const QString& url )
{
	QUrl myURL = QUrl::QUrl(url, QUrl::StrictMode);
	if ( !( QDesktopServices::openUrl(myURL) ) )
		QMessageBox::critical( this, "Error: Opening a URL", "Could not open your favourite browser. " );
}

/**
 * Slot implementations
 */
void TPM_Manager::on_myTakeOwnership_clicked()
{
	bool wellKnownSecret = false;
	string srkPassword;
	SetSRKView srkradiodialog;
	
	/* temporarily disable Take button */
	myTakeOwnership->setEnabled( false );

	NewPasswordDialog ownerDlg( this );

	// Read the TPM owner password
	ownerDlg.setPrompt( "Enter New Owner Password" );
	ownerDlg.setDescription( "Enter the new TPM owner password. The owner has the right to perform special operations on the TPM." );
	
	if ( ownerDlg.exec() == QDialog::Rejected ) {
		myTakeOwnership->setEnabled( true );		
		return; // the user cancelled
	}
	
	if( srkradiodialog.exec() == QDialog::Rejected ) {
		myTakeOwnership->setEnabled( true );
		return;
	}
		
	if ( srkradiodialog.setManually() == true )
	{
		// Read the new SRK password
		NewPasswordDialog srkDlg( this );
		
		srkDlg.setPrompt( "Enter New SRK Password" );
		srkDlg.setDescription( "Enter the new Storage Root Key (SRK) password. It is used authenticate usage of the SRK." );
	
		if ( srkDlg.exec() == QDialog::Rejected ) {
			myTakeOwnership->setEnabled( false );
			return; // the user cancelled
		}
			
		srkPassword = ( srkDlg.password() ).toStdString();
	}		
	else
		wellKnownSecret = true;

	try {
		QMessageBox::information( this, "Taking Ownership" , "Taking TPM Ownership. This can take a while, please be patient." );
		
		myTPM->takeOwnership( ( ownerDlg.password()).toStdString(), srkPassword, wellKnownSecret );
	
		QMessageBox::information( this, "Taking Ownership" , "TPM owner successfully created."  );
	
        } catch ( IsDeactivatedError &e ) {
            QMessageBox::critical( this, "Taking Ownership Failed" , "Sorry. Could not Take Ownership in deactivated mode due to TSS bug. " );
        } catch ( UnknownError &e ) {
            QMessageBox::critical( this, "Taking Ownership Failed" , QString( "Could not Take Ownership for the following reason: " ).append( QString::fromStdString( e.what() ) ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured during Taking Ownership." );
        }
	
	/* finally re-enable to Take button (may get disabled by initOwnership() again afterwards) */
	myTakeOwnership->setEnabled( true );	
	// update GUI
	initStatus();
	initStatusGroup();
	initOwnership();
}

void TPM_Manager::on_myChangePassword_clicked()
{
	PasswordDialog passDlg( this );

	passDlg.setPrompt( "Enter Owner Password" );
	passDlg.setDescription( "Enter the current owner password to change owner authorization." );

	if ( passDlg.exec() == QDialog::Rejected )
		return; // user cancelled
	
	NewPasswordDialog newPassDlg( this );
	newPassDlg.setPrompt( "Enter new Owner Password" );
	newPassDlg.setDescription( "Enter new owner password and retype it to confirm." );

	if ( newPassDlg.exec() == QDialog::Rejected )
		return; // user cancelled

	try {
		myTPM->changeOwnerPassword( ( passDlg.password() ).toStdString(), ( newPassDlg.password() ).toStdString() );

		QMessageBox::information( this, "Changing TPM Owner Password" , "TPM owner password successfully changed." );

        } catch( TPMError &e ) {
		string err = "Error while changing owner password.\n" + string( e.what() );
		QMessageBox::critical( this, "TPM Error", QString::fromStdString( err ));
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to change TPM Owner password." );
        }

	// update GUI
	initOwnership();
}

void TPM_Manager::on_myChangeSRKPassword_clicked()
{
	bool wellKnownSecret = false;
	string srkPassword;
	SetSRKView srkradiodialog;
	PasswordDialog ownerDlg( this );

	ownerDlg.setPrompt( "Enter Owner Password" );
	ownerDlg.setDescription( "Enter the owner password to change SRK authorization." );

	if ( ownerDlg.exec() == QDialog::Rejected )
		return; // user cancelled
	
	if( srkradiodialog.exec() == QDialog::Rejected ) {
		myTakeOwnership->setEnabled( true );
		return;
	}
		
	if ( srkradiodialog.setManually() == true )
	{
		// Read the new SRK password
		NewPasswordDialog newSRKDlg( this );
		
		newSRKDlg.setPrompt( "Enter New SRK Password" );
		newSRKDlg.setDescription( "Enter the new SRK password and retype it to confirm." );
	
		if ( newSRKDlg.exec() == QDialog::Rejected )
			return; // the user cancelled
			
		srkPassword = ( newSRKDlg.password() ).toStdString();
	}		
	else
		wellKnownSecret = true;

	try {
		myTPM->changeSRKPassword( ownerDlg.password().toStdString(), srkPassword, wellKnownSecret );
		QMessageBox::information( this, "Changing SRK Password" , "SRK password successfully changed." );

	} catch( AuthenticationFailure &e ) {
		string err = "Authentication failure while changing SRK password: \n" + string( e.what() );
		QMessageBox::critical( this, "TPM Error", QString::fromStdString( err ));
	} catch ( TPMError &e ) {
		string err = "Unknown error while changing SRK password: \n" + string( e.what() );
		QMessageBox::critical( this, "TPM Error", QString::fromStdString( err ));
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to change SRK password." );
        }
	// update GUI
	initOwnership();
}

void TPM_Manager::on_myClearOwnership_clicked()
{
	PasswordDialog passDlg( this );

	passDlg.setPrompt( "Enter Owner Password");
	passDlg.setDescription( "Enter owner password to clear the TPM ownership. This will clear the TPM state and restore the factory defaults." );

	if ( passDlg.exec() == QDialog::Rejected )
		return;

	if ( QMessageBox::question( this, "Warning: Clear Ownership" , "Clearing the TPM owner will invalidate user-generated TPM keys and restore the factory defaults!\nDo you really want to clear TPM ownership?", QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)
		return;
   
	try {
		myTPM->clearOwnership( ( passDlg.password() ).toStdString() );

		QMessageBox::information( this, "Clear Ownership" , "TPM ownership successfully cleared.\nPlease reboot to complete this operation. After reboot the TPM state will be: unowned, disabled and inactive.\n" );

	} catch ( TPMError &e ) {
		string err = "Error while clearing ownership. \n" + string( e.what() );
		QMessageBox::critical( this, "Error: Clear Ownership", QString::fromStdString( err ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to clear TPM Ownership." );
        }
	// update GUI
	initStatus();
	initStatusGroup();
	initOwnership();
}

void TPM_Manager::on_mySelfTest_clicked()
{
	bool executed = myTPM->selfTestFull();
	
	string result = ( myTPM->selfTestResult() );
	/* remove last garbage character */
	result = result.erase( result.size() - 1 );
	if ( executed == true ) {
		string ok = "TPM Selftest successful.\nSelftest Result: " + string(result);
		QMessageBox::information( this,  "TPM Selftest" , QString::fromStdString( ok ) );
	} else {
		string err = "Error occured at TPM full Selftest. \nSelftest Result: " + string( result );
		QMessageBox::critical( this, "TPM Selftest", QString::fromStdString( err ) );
	}
	// refresh TPM status view
	initStatusGroup();
}

void TPM_Manager::on_myDeactivate_clicked()
{
	/*
	// prepare for 1.2 TPMs
	PasswordDialog passDlg( this );

	passDlg.setPrompt( "Enter Operator Password");
	passDlg.setDescription( "Enter operator password to temporarily deactivate the TPM." );

	if ( passDlg.exec() == QDialog::Rejected )
		return;
	*/
		
	if ( QMessageBox::question(  this, "Warning: Deactivating TPM" , "TPM will be deactivated until the next boot. Are you sure to deactivate the TPM now?", QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)
		return;
		
	try {
		// myTPM->setTempDeactivated( ( passDlg.password() ).toStdString() );
		myTPM->setTempDeactivated();
		
		QMessageBox::information( this, "TPM deactivated" , "The TPM is deactivated now!" );
	} catch ( TPMError &e ) {
		string err = "Error while deactivating TPM. \n" + string( e.what() );
		QMessageBox::critical( this, "Error: Deactivate TPM", QString::fromStdString( err ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to deactivate TPM." );
        }
	
	// refresh TPM status view & options
	initStatusGroup();
	initStatus();
	initOperationalModes();
}


void TPM_Manager::on_listBox_currentRowChanged( int index )
{
	// first: change stackedView 'Tab'
	stackedView->setCurrentIndex( index );
	// second: init currently active stackedView 'Tab' tab
	switch (index) {
		case 0:
			on_tabInfo_currentChanged( tabInfo->currentIndex() );
			break;
		case 1:
			on_tabOwner_currentChanged( tabOwner->currentIndex() );
			break;
		case 2:
			on_tabSetting_currentChanged( tabSetting->currentIndex() );
			break;
		case 3:
			on_tabAdvanced_currentChanged( tabSetting->currentIndex() );
			break;
		default:
			on_tabInfo_currentChanged( tabInfo->currentIndex() );
	}
}

void TPM_Manager::on_tabInfo_currentChanged( int index )
{	
	switch (index) {
		case 0:	
			initStatus();
			break;
		case 1:
			initDetails();
			break;
		case 2:
			initCapabilities();
			break;
		case 3:
			initPCRs();
			break;
		default:
			// this should never happen
			initStatus();
	}
		
	initStatusGroup();
}

void TPM_Manager::on_tabOwner_currentChanged( int index )
{
	switch (index) {
		case 0:
			initOwnership();
			break;
		case 1:
			initBackup();
			break;
		default:
			initOwnership();
	}
	
	initStatusGroup();
}

void TPM_Manager::on_tabSetting_currentChanged( int index )
{
	switch (index) {
		case 0:
			initOperationalModes();
			break;
		case 1:
			initIdentity();
			break;
		case 2:
			initSelfTest();
			break;
		case 3:
			initReadCertificate();
			break;
		default:
			initOperationalModes();
	}

	initStatusGroup();
}

void TPM_Manager::on_tabAdvanced_currentChanged( int index )
{
	switch (index) {
		case 1:
			initDisableMaintenance();
			break;
		case 2:
			initRevokeTrust();
			break;
		default:
			initDisableMaintenance();
	}
	
	initStatusGroup();
}

void TPM_Manager::on_myDisable_clicked()
{
	PasswordDialog passDlg(this);

	if ( !myTPM->isDisabled() )
	{
		// disable TPM
		passDlg.setPrompt( "Enter Owner Password" );		
		passDlg.setDescription( "Enter owner password to disable the TPM. " );
	
		if ( passDlg.exec() == QDialog::Rejected )
			return; // user cancelled
	
		if ( QMessageBox::question(  this,  "Warning: Disabling the TPM", "TPM resources will not be useable until the TPM is enabled again.\nDo you really want to disable the TPM?", QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)
			return;
		try{		
			myTPM->setDisabled( true, ( passDlg.password() ).toStdString() );
		
			QMessageBox::information( this, "Disabling the TPM", "TPM successfully disabled." );
		} catch ( TPMError &e ) {
                    string err = "Authentication failure. \n" + string( e.what() );
                    QMessageBox::critical( this, "Error when disabling the TPM" , QString::fromStdString( err ) );
                } catch ( ... ) {
                    QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to disable TPM." );
                }
	}
	else
	{
		// enable TPM
		passDlg.setPrompt( "Enter Owner Password" );
		passDlg.setDescription( "Enter owner password to enable the TPM." );
	
		if ( passDlg.exec() == QDialog::Rejected )
			return; // user cancelled
		
		try{		
			// TPM will be enabled, if an owner	exist and owner password is correct.
			myTPM->setDisabled( false, ( passDlg.password() ).toStdString() );
			QMessageBox::information( this, "Enabling the TPM" , "TPM successfully enabled." );
		} catch ( TPMError &e ) {
			string err = "Authentication failure. \n" + string( e.what() );
			QMessageBox::critical( this, "Error when enabling the TPM", QString::fromStdString( err ) );
                } catch ( ... ) {
                    QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to enable TPM." );
                }
	}
	// refresh TPM status view & options
	initStatusGroup();
	initStatus();
	initOperationalModes();
}

void TPM_Manager::slotEnableAdvancedTabs()
{
	///tabDisableMaintenance->setEnabled();
}

void TPM_Manager::on_myShowEndorsement_clicked()
{
	string password = "";

	if ( myTPM->isEndorsementKeyResticted() )
	{
		PasswordDialog passDlg( this );
		passDlg.setPrompt( "Enter Owner Password" );
		passDlg.setDescription( "Endorsement key is in restricted mode. Please enter your owner password to read the public key." );
	
		if ( passDlg.exec() == QDialog::Rejected )
			return; // user cancelled
			
		password = ( passDlg.password() ).toStdString();
	}
	try {
		PublicKey pk = myTPM->getEndorsementPublicKey( password );
		PublicKeyView pkview( this );
		pkview.setPublicKey( pk );
		pkview.exec();
	} catch ( TPMError &e ) {
		string err = "Authentication failure. \n" + string( e.what() );
		QMessageBox::critical( this, "Error when trying to get Endorsement Key" , QString::fromStdString( err ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to retrieve TPM Endorsement Key." );
        }
}

void TPM_Manager::on_mySaveEndorsement_clicked()
{
	string password = "";

	if ( myTPM->isEndorsementKeyResticted() )
	{
		PasswordDialog passDlg( this );
		passDlg.setPrompt( "Enter Owner Password" );
		passDlg.setDescription( "Endorsement key is in restricted mode. Please enter your owner password to access to public key." );
	
		if ( passDlg.exec() == QDialog::Rejected )
			return; // user cancelled
		password = ( passDlg.password() ).toStdString();
	}	
	try{
		PublicKey pk = myTPM->getEndorsementPublicKey( password );
	
		QString filename = QFileDialog::getSaveFileName(
							this,
							"Save File",
							".key",
							"Key (*.key);;All Files (*.*)");
		ofstream fs( ( ( filename ).toStdString() ).c_str(), ios::trunc );
		fs << pk;
		fs.close();
	} catch ( TPMError &e ) {
		string err = "Authentication failure. \n" + string( e.what() );
                QMessageBox::critical( this, "Error when trying to store TPM Endorsement Key", QString::fromStdString( err ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to store TPM Endorsement Key." );
        }

}

void TPM_Manager::on_myRestrictEndorsement_clicked()
{
	string password = "";

	PasswordDialog passDlg( this );
	passDlg.setPrompt( "Enter Owner Password" );
        passDlg.setDescription( "Enter owner password to restrict viewing public TPM Endorsement Key information without owner authorization. " );

	if ( passDlg.exec() == QDialog::Rejected )
		return; // user cancelled
	password = ( passDlg.password() ).toStdString();

	try{
		myTPM->restrictEK( password );

		QMessageBox::information( this, "Restrict Endorsement Key", "Endorsement Key is successfully restricted." );

	} catch ( TPMError &e ) {
		string err =  "Error: Restrict Endorsement Key. \n" + string( e.what() );
		QMessageBox::critical( this, "Error at restrict Endorsement Key", QString::fromStdString( err ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to restrict access to TPM Endorsement Key." );
        }

	initIdentity();
}

void TPM_Manager::on_myDisableMaintenance_clicked()
{
	PasswordDialog passDlg( this );
	passDlg.setPrompt( "Enter Owner Password" );
	passDlg.setDescription( "Enter owner password to disable Maintenance archive." );

	if ( passDlg.exec() == QDialog::Rejected )
		return; // user cancelled

	if ( QMessageBox::question(  this, "Warning: Disable Maintenance" , "This will  disable the functionality of creating a maintenance archive!\nDo you really want to clear the disable?", QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No)
		return;
   
	try {
		myTPM->killMaintenance( ( passDlg.password() ).toStdString() );

		QMessageBox::information( this, "Disable Maintenance" ,  "Maintenance archive is successfully disabled.\nThis functionality can only be enabled again by releasing the TPM ownership." );

	} catch ( TPMError &e ) {
		string err = "Error while disabling Maintenance Archive. \n" + string( e.what() );
		QMessageBox::critical( this,  "Error: Disable Maintenance Archive" , QString::fromStdString( err ) );
        } catch ( ... ) {
            QMessageBox::critical( this, "Unknown Error", "An unknown error occured when trying to disable TPM Maintenance archive." );
        }
}

void TPM_Manager::on_myDeleteEndorsement_clicked()
{

}
