/***************************************************************************
 *   Copyright (C) 2006 by Anoosheh Zaerin   *
 *   anoosheh.zaerin@rub.de   *
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

#include <TSS.hh>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>

#include <qlineedit.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qtable.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qpixmap.h>

#include <kpassdlg.h>
#include <kmessagebox.h>

#include <PublicKey.hh>
#include <PublicKeyView.hh>

#include "tpmmanagerwidget.h"

using namespace std;

TPM_ManagerWidget::TPM_ManagerWidget(QWidget* parent, const char* name, WFlags fl) : 
  TPM_ManagerWidgetBase( parent,name,fl ),
  myTSS(),
  myTPM( 0 )
{
	try {
		// myTSS = new TSS;
		myTPM = new TPM( myTSS.getContextHandle() );
	} 
	catch ( TPMDriverNotFound &e ) {
		cout << e.what() << endl;
		driverFound->setText( "no" );
	}
	catch ( TSSSystemNotFound &e ) {
		cout << e.what() << endl;
		tssFound->setText( "no" );
	}

	listBox->setCurrentItem( 0 );

	if ( myTPM->isDisabled() && !myTPM->hasOwner() )
		KMessageBox::information( this, "The TPM is disabled and no owner is set. You have to enable the TPM in the BIOS to use the functions of the TPM, e.g., to take ownership.\n", "Information" );
}

TPM_ManagerWidget::~TPM_ManagerWidget()
{
	delete myTPM;  /// @todo another design without pointer to TPM/TSS
	// delete myTSS;
}

/**
 * Initialize methodes implementations
 */
void TPM_ManagerWidget::initStatus()
{
	/// Status
	driverFound->setText( "yes" );
	tssFound->setText( "yes" );

	/// TPM Enable/Active/Owner state
	/*if ( myTPM->isDisabled() )
		tpmEnabled->setText( "no" );
	else
		tpmEnabled->setText( "yes" );
	*/
	if ( myTPM->isEnabled() ) {
		tpmEnabled->setText( "yes" );
		if ( myTPM->isActivated() )
			tpmActivated->setText( "yes" );
		else
			tpmActivated->setText( "no" );
	}
	else {
		tpmEnabled->setText( "no" );
		tpmActivated->setText( "??" );
	}

	if ( myTPM->hasOwner() )
		ownerSet->setText( "yes" );
	else
		ownerSet->setText( "no" );

	if ( myTPM->hasEndorsementKey() )
		myEndorsementAvailable->setText( "yes" );
	else
		myEndorsementAvailable->setText( "no" );
}

void TPM_ManagerWidget::initStatusGroup()
{
	if ( myTPM->isEnabled() )
 		{
			myEnabledOkImage->show();
			myEnabledNokImage->hide();
		}
	else
		{
			myEnabledOkImage->hide();
			myEnabledNokImage->show();
		}

	if ( myTPM->isActivated() )
		{
			myActivatedOkImage->show();
			myActivatedNokImage->hide();
			myActivatedUnknown->hide();
		}
	else 
		if ( !myTPM->isDisabled() )
		{
			myActivatedOkImage->hide();
			myActivatedNokImage->show();
			myActivatedUnknown->hide();
		} else
		{
			myActivatedOkImage->hide();
			myActivatedNokImage->hide();
			myActivatedUnknown->setText("??");
		}
		

	if ( myTPM->hasOwner() )
		{
			myOwnerSetOkImage->show();
			myOwnerSetNokImage->hide();
		}
	else
		{
			myOwnerSetOkImage->hide();
			myOwnerSetNokImage->show();
		}
}

void TPM_ManagerWidget::initCapabilities()
{
	/// set Capabilities
	myCapabilities->setColumnStretchable( 0, true );
	myCapabilities->setNumRows( 1 );
	myCapabilities->adjustColumn( 0 );
	myCapabilities->adjustColumn( 1 );
	myCapabilities->setText(0, 0, "Number of PCRs");
	myCapabilities->setText(0, 1, QString("%1").arg( myTPM->getNumberOfPCR()) );
}

void TPM_ManagerWidget::initDetails()
{
	/// TSS Details
	tssVendor->setText( myTSS.getVendorName() );
	QString version = QString( myTSS.getVersion() ) + ", rev " + QString( myTSS.getRevision() );
	tssVersion->setText( version );


	/// TPM Details
	tpmVendor->setText( myTPM->getVendorName() );
	tpmVersion->setText( myTPM->getVersion() );
	tpmFirmware->setText( myTPM->getRevision() );
}

void TPM_ManagerWidget::initPCRs()
{
	ostringstream pcrStr;
	vector<ByteString> pcrValues = myTPM->getPCRValues();

	for( size_t i=0; i < pcrValues.size(); ++i)
	{
		pcrStr << "PCR[" << dec << setw( 2 ) << setfill( '0' ) << i << "] "; 
		
		for ( size_t j=0; j<pcrValues[i].size(); ++j) {
			if ((j % 4) == 0)
				pcrStr << " ";
			pcrStr << hex << setw( 2 ) << setfill( '0' ) << (int) pcrValues[i][j];	
		}

		myPCRs->insertItem( pcrStr.str() );
		pcrStr.str("");
	}
}

void TPM_ManagerWidget::initOwnership()
{
	myTakeOwnership->setEnabled( myTPM->isEnabled() && !myTPM->hasOwner() );
	
	myChangePassword->setEnabled( myTPM->isEnabled() && myTPM->isActivated() && myTPM->hasOwner() );
	myClearOwnership->setEnabled( myChangePassword->isEnabled() );
	
	myTakeOwnerText->setEnabled( myTakeOwnership->isEnabled() );
	myChangePassText->setEnabled( myChangePassword->isEnabled() );
	myClearOwnerText->setEnabled( myClearOwnership->isEnabled() );
}

void TPM_ManagerWidget::initBackup()
{
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

void TPM_ManagerWidget::initOperationalModes()
{
	myDisable->setEnabled( myTPM->hasOwner() );
	if ( myTPM->isEnabled() )
		myDisable->setText( "Disable" );
	else
		myDisable->setText( "Enable" );
	myDeactivate->setEnabled( myTPM->isActivated() );
}

void TPM_ManagerWidget::initIdentity()
{
	bool hasEK = myTPM->hasEndorsementKey();
	myCreatEndorsement->setEnabled( !hasEK && !myTPM->isDisabled() );
	myShowEndorsement->setEnabled( hasEK && !myTPM->isDisabled() && !myTPM->isDeactivated() );
	mySaveEndorsement->setEnabled( hasEK && !myTPM->isDisabled() && !myTPM->isDeactivated() );
	myRestrictEndorsement->setEnabled( hasEK && myTPM->hasOwner() 
													&& !myTPM->isEndorsementKeyResticted()
													&& !myTPM->isDisabled()  );
//	myReadCertificate->setEnabled( hasEK );
}

void TPM_ManagerWidget::initReadCertificate()
{
	myCertificateText->setEnabled( myTPM->hasEndorsementKey() );
	myReadCertificate->setEnabled( myTPM->hasEndorsementKey() );
	if ( myTPM->hasEndorsementKey() )
	{
		myCertificateIcon->hide();
		myCertificateText->hide();
	}
	else
	{
		myCertificateIcon->show();
		myCertificateText->show();
	}
}

void TPM_ManagerWidget::initDisableMaintenance()
{
	myDisableMaintenance->setEnabled( myTPM->hasMaintenance() );
	myDisableMaintenanceText->setEnabled( myTPM->hasMaintenance() );
	myDisableMaintenanceWarnIcon->setEnabled( myTPM->hasMaintenance() );
	myDisableMaintenanceWarnText->setEnabled( myTPM->hasMaintenance() );
	if ( myTPM->hasMaintenance() )
	{
		myDisableMaintenanceInfoText->hide();
		myDisableMaintenanceInfoIcon->hide();
	} else
	{
		myDisableMaintenanceInfoText->show();
		myDisableMaintenanceInfoIcon->show();
	}
}

void TPM_ManagerWidget::initRevokeTrust()
{
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

/**
 * Slots implementations
 */
void TPM_ManagerWidget::slotTakeOwnership()
{
	KPasswordDialog ownerDlg( KPasswordDialog::NewPassword, false, KDialogBase::Help );

	// Read the TPM owner password
	ownerDlg.setCaption( "Take Ownership: Set Owner Password" );
	ownerDlg.setPrompt( "Enter the TPM owner password. The owner has the right to perform special operations on the TPM." );
	ownerDlg.exec();

	if ( ownerDlg.result() != KPasswordDialog::Accepted )
		return;

	// Read the SRK password
	KPasswordDialog srkDlg( KPasswordDialog::NewPassword, false, KDialogBase::Help );
	
   srkDlg.setCaption( "Take Ownership: Set SRK Password" );
	srkDlg.setPrompt( "Enter the SRK password. It is used to authenticate the usage of the Storage Root Key (SRK)." );
	srkDlg.exec();

	if ( srkDlg.result() != KPasswordDialog::Accepted )
		return;

	try {
		myTPM->takeOwnership( ownerDlg.password(), srkDlg.password() );
	
		KMessageBox::information( this, "TPM owner is successfully created. ", "Information: Taking Ownership" );
	
		slotOwnerTabWidgetChanged( tabOwnership );
	} catch ( IsDeactivatedError &e )
	{
		KMessageBox::error( this, "Sorry. Could not Take Ownership in deactivated mode due to TSS bug. ", "Error: Taking Ownership" );
	}
}

void TPM_ManagerWidget::slotChangePassword()
{
	KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );

	passDlg.setCaption( "Enter Ownership password" );
	passDlg.setPrompt( "Enter owner password be able to set the new Password." );
	passDlg.exec();

	if ( passDlg.result() != KPasswordDialog::Accepted )
		return;

	KPasswordDialog newPassDlg( KPasswordDialog::NewPassword, false, KDialogBase::Help );
	newPassDlg.setCaption( "Change Ownership passwsord" );
	newPassDlg.setPrompt( "Enter new password and retype it to confirm." );
	newPassDlg.exec();

	if ( newPassDlg.result() != KPasswordDialog::Accepted )
		return;

	try {
		myTPM->changeOwnerPassword( passDlg.password(), newPassDlg.password() );

		KMessageBox::information( this, "TPM owner password successfully changed.\n", "Information" );

	} catch( TPMError &e )
	{
		KMessageBox::error( this, "Error while changing owner password.\n" + string( e.what() ) , "TPM Error" );
	}
	
}

void TPM_ManagerWidget::slotClearOwnership()
{
	KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );

	passDlg.setCaption( "Clear Ownership" );
	passDlg.setPrompt( "Enter owner password to clear the TPM ownership. This will clear the TPM state and restore the factory defaults." );
	passDlg.exec();

	if ( passDlg.result() != KPasswordDialog::Accepted )
		return;

	if ( KMessageBox::warningYesNo(  this, "Clearing the TPM owner will invalidate user-generated TPM keys and restore the factory defaults!\nDo you really want to clear the TPM ownership?", "Warning: Clear ownership" ) != KMessageBox::Yes)
		return;
   
	try {
		myTPM->clearOwnership( passDlg.password() );

		KMessageBox::information( this, "TPM ownership successfully cleared.\nPlease reboot to complete this operation. After reboot the TPM state will be: unowned, disabled and inactive.\n", "Information: Clear Ownership" );

		slotOwnerTabWidgetChanged( tabOwnership );

	} catch ( TPMError &e ) {
		KMessageBox::error( this, "Error while clearing ownership. \n" + string( e.what() ) , "Error at clearing Ownership" );
	}
}

void TPM_ManagerWidget::slotSelfTestFull()
{
	QString result = myTPM->selfTestFull();
	
	if ( result = "0xffff" )
		KMessageBox::information( this, "TPM full Selftest is succesfully done!", "TPM Selftest Result" );
	else
		KMessageBox::error( this, "Error ocuured at TPM full Selftest. \nResult of the test: " + result,
									"TPM Selftest Result");
}

void TPM_ManagerWidget::slotSetTempDeactivated()
{
	if ( KMessageBox::warningYesNo(  this, "TPM will be deactivated until the next boot. Are you sure to deactivate the TPM now?", "Warning: Deactivating TPM" ) != KMessageBox::Yes)
		return;
	myTPM->setTempDeactivated();
	// try ... catch
	KMessageBox::information( this, "TPM is deactivated now!", "Information: TPM deactivated" );
	slotTPMSettingsTabWidgetChanged( tabOperationalModes );
}

void TPM_ManagerWidget::slotInfoTabWidgetChanged( QWidget* widget )
{	
	if( widget == tabStatus )
		initStatus();
	else if( widget == tabCapabilities )
		initCapabilities();
	else if ( widget == tabDetails )
		initDetails();
	else if ( widget == tabPCRs )
		initPCRs();
	
	initStatusGroup();
}

void TPM_ManagerWidget::slotOwnerTabWidgetChanged( QWidget* widget )
{
	if( widget == tabOwnership )
		initOwnership();
	else if ( widget == tabBackup )
		initBackup();

	initStatusGroup();
}

void TPM_ManagerWidget::slotTPMSettingsTabWidgetChanged( QWidget* widget )
{
	if( widget == tabOperationalModes )
		initOperationalModes();
	else if ( widget == tabIdentity )
		initIdentity();

	initStatusGroup();
}

void TPM_ManagerWidget::slotAdvancedTabWidgetChanged( QWidget* widget )
{
	if( widget == tabDisableMaintenance )
		initDisableMaintenance();
	else if ( widget == tabRevokeTrust )
		initRevokeTrust();

	initStatusGroup();
}

void TPM_ManagerWidget::slotSetEnabledDisabled()
{
	KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );

	if ( !myTPM->isDisabled() )
	{	
		passDlg.setCaption( "Disable the TPM" );
		passDlg.setPrompt( "Enter owner password to disable the TPM. " );
		passDlg.exec();
	
		if ( passDlg.result() != KPasswordDialog::Accepted )
			return;
	
		if ( KMessageBox::warningYesNo(  this, "TPM resources will not be useable until the TPM is enabled again.\nDo you really want to disable the TPM?", "Warning: Disable the TPM" ) != KMessageBox::Yes)
			return;
		
		myTPM->setDisabled( passDlg.password() );
		
		KMessageBox::information( this, "TPM successfully disabled.", "Information: Disable the TPM" );
	}
	else
	{
		passDlg.setCaption( "Enable the TPM" );
		passDlg.setPrompt( "Enter owner password to enable the TPM. " );
		passDlg.exec();
	
		if ( passDlg.result() != KPasswordDialog::Accepted )
			return;
		
		// TPM will be enabled, if an owner	exist and owner password is correct.
		myTPM->setEnabled( passDlg.password() );
		
		KMessageBox::information( this, "TPM successfully enabled.", "Information: Enable the TPM" );
	}
	slotTPMSettingsTabWidgetChanged( tabOperationalModes );
}

void TPM_ManagerWidget::slotEnableAdvancedTabs( bool setTabsEnable )
{
	///tabDisableMaintenance->setEnabled();
}

void TPM_ManagerWidget::slotShowEndorsementKey()
{
	string password = "";

	if ( myTPM->isEndorsementKeyResticted() )
	{
		KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );
		passDlg.setCaption( "Password Entry" );
		passDlg.setPrompt( "Endorsement key is in restricted mode.  Please enter your owner password to read the public key. " );
		passDlg.exec();
	
		if ( passDlg.result() != KPasswordDialog::Accepted )
			return;
		password = passDlg.password();
	}
	try {
		PublicKey pk = myTPM->getEndorsementPublicKey( password );

		PublicKeyView pkview( pk );
		pkview.exec();
	} catch ( TPMError &e ) {
		KMessageBox::error( this, "Authentication failure. \n" + string( e.what() ) , "Error at show Endorsement Key" );
	}
}

void TPM_ManagerWidget::slotSaveEndorsement()
{
	string password = "";

	if ( myTPM->isEndorsementKeyResticted() )
	{
		KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );
		passDlg.setCaption( "Password Entry" );
		passDlg.setPrompt( "Endorsement key is in restricted mode.  Please enter your owner password to access to public key. " );
		passDlg.exec();
	
		if ( passDlg.result() != KPasswordDialog::Accepted )
			return;
		password = passDlg.password();
	}	
	try{
		PublicKey pk = myTPM->getEndorsementPublicKey( password );
	
		QFileDialog filedlg;
		string filename = filedlg.getSaveFileName(
							"/home",
							"Key (*.key);;All Files (*.*)" ,
							this,
							"save file dialog"
							"Choose a filename to save under" );
		ofstream fs( filename.c_str(), ios::trunc );
		fs << pk;
		fs.close();
	} catch ( TPMError &e ) {
		KMessageBox::error( this, "Authentication failure. \n" + string( e.what() ) , "Error at save Endorsement Key" );
	}

}

void TPM_ManagerWidget::slotRestrictEndorsement()
{
	string password = "";

	KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );

	passDlg.setCaption( "Restrict Endorsement Key" );
	passDlg.setPrompt( "Enter owner password to restrict viewing public endorsement key informations without owner autorization. " );
	passDlg.exec();

	if ( passDlg.result() != KPasswordDialog::Accepted )
		return;
	password = passDlg.password();

	try{
		myTPM->restrictEK( password );

		KMessageBox::information( this, "Endorsement Key is successfully restricted.", "Information: Restrict Endorsement Key" );

	} catch ( TPMError &e ) {
		KMessageBox::error( this, "Error while restrict Endorsement Key. \n" + string( e.what() ) , "Error at restrict Endorsement Key" );
	}

	initIdentity();
}

void TPM_ManagerWidget::slotDisableMaintenance()
{
	KPasswordDialog passDlg( KPasswordDialog::Password, false, KDialogBase::Help );

	passDlg.setCaption( "Kill Maintenance" );
	passDlg.setPrompt( "Enter owner password to disable Meintanence archive." );
	passDlg.exec();

	if ( passDlg.result() != KPasswordDialog::Accepted )
		return;

	if ( KMessageBox::warningYesNo(  this, "This will  disable the functionality of creating a maintenance archive!\nDo you really want to clear the disable?", "Warning: Disable Maintenance" ) != KMessageBox::Yes)
		return;
   
	try {
		myTPM->killMaintenance( passDlg.password() );

		KMessageBox::information( this, "Maintenance archive is successfully disabled.\nThis functionality can only be enabled again by releasing the TPM ownership.", "Information: Disable Maintenance" );

	} catch ( TPMError &e ) {
		KMessageBox::error( this, "Error while disabling Maintenance Archive. \n" + string( e.what() ) , "Error at Disable Maintenance Archive" );
	}
}

void TPM_ManagerWidget::slotDeleteEndorsement()
{

}

#include "tpmmanagerwidget.moc"
