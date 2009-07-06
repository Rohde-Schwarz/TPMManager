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

#ifndef TPMMANAGER_H
#define TPMMANAGER_H

#include <QDialog>
#include <QResource>
#include <QTimer>
#include "ui_tpmmanagerbase.h"

#include <microtss/TPM.h>
#include <microtss/TSS.h>

/**
* @file tpmmanager.h
*
* @brief TPM Manager Controller Class Header File
*
**/

/**
* @class TPM_Manager
*
* @brief Encapsulates all the TPM Manager functionality (controller).
*
* This class implements all the TPM Manager functionality. In the MVC model, this would be the controller. The base class is automatically generated as "ui_tpmmanagerbase".
*
*/ 
class TPM_Manager : public QDialog, public Ui::TPM_ManagerBase
{
Q_OBJECT
public:
	/**
	* @brief Default constructor
	*
	* @param parent parent QWidget object
	* @param f window flags
	*
	* @return New TPM_Manager instance
	*
	*/
	TPM_Manager( QWidget * parent = 0, Qt::WFlags f = 0 );
	
	/**
	* @brief Default destructor
	*
	*/
	~TPM_Manager();
public slots:
	/* none */
protected:
	/**
	* @brief Initializes the "Status" tab
	*/
	void initStatus();	
	/**
	* @brief Initializes the "TPM Status" group view
	*/
	void initStatusGroup();
	/**
	* @brief Initializes the "Capabilities" tab
	*/
	void initCapabilities();
	/**
	* @brief Initializes the "PCRs" tab
	*/
	void initPCRs();
	/**
	* @brief Initializes the "Details" tab
	*/
	void initDetails();
	/**
	* @brief Initializes the "Ownership" tab
	*/
	void initOwnership();
	/**
	* @brief Initializes the "Backup" tab
	*/
	void initBackup();
	/**
	* @brief Initializes the "Operational Modes" tab
	*/
	void initOperationalModes();
	/**
	* @brief Initializes the "Identity" tab
	*/
	void initIdentity();
	/**
	* @brief Initializes the "Selftest" tab
	*/
	void initSelfTest();
	/**
	* @brief Initializes the "TPM Certificate" tab
	*/
	void initReadCertificate();
	/**
	* @brief Initializes the "Disable Maintenance" tab
	*/
	void initDisableMaintenance();
	/**
	* @brief Initializes the "Revoke Trust" tab
	*/
	void initRevokeTrust();

protected slots:
	/// Show PCRs, called from Timer repeatedly
	void slotUpdatePCRs();
	/// Take Ownership
	void on_myTakeOwnership_clicked();
	/// Change password
	void on_myChangePassword_clicked();
	/// Change SRK password
	void on_myChangeSRKPassword_clicked();
	/// Clear Ownership
	void on_myClearOwnership_clicked();
	/// Perform TPM Selftest
	void on_mySelfTest_clicked();
	/// Set temporarily deactivated
	void on_myDeactivate_clicked();
	/// Change status of the TPM to disabled/enabled
	void on_myDisable_clicked();
	
	/// Show appropriate tab widget
	void on_listBox_currentRowChanged(int index);
	
	/// A tab of the Info stack is selected
	void on_tabInfo_currentChanged(int index);
	/// A tab of the Owner Settings stack is selected
	void on_tabOwner_currentChanged(int index);
	/// A tab of the TPM Setting stack is selected
	void on_tabSetting_currentChanged(int index);
	/// A tab of the Advanced stack is selected.
	void on_tabAdvanced_currentChanged(int index);
		
	/**
	* @brief If setTabsEnable = true then activate the tab dialogs in advanced area
	* @note currently not in use
	*/
	void slotEnableAdvancedTabs();
	/// Read Endorsement Key and show details
	void on_myShowEndorsement_clicked();
	/// Save Endorsement key and key info in file
	void on_mySaveEndorsement_clicked();
	/// Restrict Endorsement Key to TPM Owner. Restricted TPM requires Owner password to read the Endorsement Key
	void on_myRestrictEndorsement_clicked();
	/// Disable the functionality of creating a maintenance archive
	void on_myDisableMaintenance_clicked();
	/// Delete Endorsement Key
	void on_myDeleteEndorsement_clicked();
	
	/// Process the URL and open it in a Browser
	/// @note: leave with the old format, because this function gets called from multiple objects
	void slotProcessURL( const QString& url );
	
	// @return TRUE is TSS was found.
	inline bool hasTSS() const;
	// @return TRUE if TPM was found.
	inline bool hasTPM() const;

private:
	microtss::TSS    *myTSS;
	microtss::TPM    *myTPM;
	QPixmap          myOkImage;
	QPixmap          myUnknownImage;
	QPixmap          myNokImage;
	QTimer		 myTimer;
};

bool TPM_Manager::hasTSS() const {
	return myTSS != 0;
}

bool TPM_Manager::hasTPM() const {
	return myTPM != 0;
}

#endif

