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


#ifndef _TPM_MANAGERWIDGET_H_
#define _TPM_MANAGERWIDGET_H_

#include "tpmmanagerwidgetbase.h"
#include <TPM.hh>
#include <TSS.hh>

class TPM_ManagerWidget : public TPM_ManagerWidgetBase
{
    Q_OBJECT

public:
    TPM_ManagerWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~TPM_ManagerWidget();
    /*$PUBLIC_FUNCTIONS$*/

public slots:
    /*$PUBLIC_SLOTS$*/
    // virtual void button_clicked();

protected:
	/// Initializes the Status tab.
	void initStatus();
	/// Intialize the TPM Status Group 
	void initStatusGroup();
	/// Initializes the Capability tab.
	void initCapabilities();
	/// Initialises the PCRs tab.
	void initPCRs();
	/// Initializes the Details tab.
	void initDetails();
	/// Initializes the Ownership tab.
	void initOwnership();
	/// Initializes the Backup tab.
	void initBackup();
	/// Initializes the Operational Modes tab.
	void initOperationalModes();
	/// Initializes the Identity tab
	void initIdentity();
	/// Initializes the TPM Certificate tab
	void initReadCertificate();
	/// Initializes the Delete Maintenance tab
	void initDisableMaintenance();
	/// Initializes the Revoke trust tab
	void initRevokeTrust();

protected slots:
	/// Take Ownership
	void slotTakeOwnership();
	/// Change password
	void slotChangePassword();
	/// Clear Ownership
	void slotClearOwnership();
	/// Self test result
	void slotSelfTestFull();
	/// Set temporarily deactivated
	void slotSetTempDeactivated();
	/// Change status of the TPM to enable/disable
	void slotSetEnabledDisabled();

	/// A Tab of the Info tabwidget is selected.
	void slotInfoTabWidgetChanged( QWidget* widget );
	/// A Tab of the Owner Setting tabwidget is selected.
	void slotOwnerTabWidgetChanged( QWidget* widget );
	/// A Tab of the TPM Setting tabwidget is selected.
	void slotTPMSettingsTabWidgetChanged( QWidget* widget );
	/// A Tab of the Advanced tabwidget is selected.
	void slotAdvancedTabWidgetChanged( QWidget* widget );
	/// If setTabsEnable = true then activate the tab dialogs in advanced area.
	void slotEnableAdvancedTabs( bool setTabsEnable );
	/// Will be read the Endorsement key(EK) if there exist an EK and that is not restricted.
	void slotShowEndorsementKey();
	/// Save Endorsement key and Key Info in File
   void slotSaveEndorsement();
	/// Restrict Endorsement key. Restricted TPM requires Owner password to read the Endorsement Key
	void slotRestrictEndorsement();
	/// Disable the functionality of creating a maintenance archive
	void slotDisableMaintenance();
	/// Delete Endorsement Key
	void slotDeleteEndorsement();
private:
	TPM  *myTPM;
	TSS  myTSS;
};

#endif
