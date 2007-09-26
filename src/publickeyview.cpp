#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './publickeyview.ui'
**
** Created: Wed Sep 27 23:55:53 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "publickeyview.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a PublicKeyViewWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
PublicKeyViewWidget::PublicKeyViewWidget( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "PublicKeyViewWidget" );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 80, 50, 71, 21 ) );

    textLabel1_2_2 = new QLabel( this, "textLabel1_2_2" );
    textLabel1_2_2->setGeometry( QRect( 80, 80, 71, 21 ) );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 80, 20, 71, 21 ) );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setGeometry( QRect( 142, 211, 47, 24 ) );
    buttonOk->setPaletteForegroundColor( QColor( 0, 85, 127 ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );

    myType = new QLabel( this, "myType" );
    myType->setGeometry( QRect( 170, 50, 140, 21 ) );

    myVersion = new QLabel( this, "myVersion" );
    myVersion->setGeometry( QRect( 170, 20, 140, 21 ) );

    myAlgorithm = new QLabel( this, "myAlgorithm" );
    myAlgorithm->setGeometry( QRect( 170, 80, 140, 21 ) );

    myKeySize = new QLabel( this, "myKeySize" );
    myKeySize->setGeometry( QRect( 170, 110, 140, 21 ) );

    textLabel1_2_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2_2" );
    textLabel1_2_2_2_2_2->setGeometry( QRect( 21, 110, 130, 21 ) );

    textLabel1_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2" );
    textLabel1_2_2_2_2->setGeometry( QRect( 21, 170, 130, 21 ) );

    textLabel1_2_2_2 = new QLabel( this, "textLabel1_2_2_2" );
    textLabel1_2_2_2->setGeometry( QRect( 21, 140, 130, 21 ) );

    myEncScheme = new QLabel( this, "myEncScheme" );
    myEncScheme->setGeometry( QRect( 170, 140, 140, 21 ) );

    mySigScheme = new QLabel( this, "mySigScheme" );
    mySigScheme->setGeometry( QRect( 170, 170, 140, 21 ) );
    languageChange();
    resize( QSize(330, 247).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
PublicKeyViewWidget::~PublicKeyViewWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PublicKeyViewWidget::languageChange()
{
    setCaption( tr2i18n( "Public Key Info" ) );
    textLabel1_2->setText( tr2i18n( "<p align=\"right\">Type</p>" ) );
    textLabel1_2_2->setText( tr2i18n( "<p align=\"right\">Algorithm</p>" ) );
    textLabel1->setText( tr2i18n( "<p align=\"right\">Version</p>" ) );
    buttonOk->setText( tr2i18n( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    myType->setText( tr2i18n( "01010000" ) );
    myVersion->setText( tr2i18n( "01010000" ) );
    myAlgorithm->setText( tr2i18n( "01010000" ) );
    myKeySize->setText( tr2i18n( "01010000" ) );
    textLabel1_2_2_2_2_2->setText( tr2i18n( "<p align=\"right\">Key Size</p>" ) );
    textLabel1_2_2_2_2->setText( tr2i18n( "<p align=\"right\">Signature Scheme</p>" ) );
    textLabel1_2_2_2->setText( tr2i18n( "<p align=\"right\">Encryption Scheme</p>" ) );
    myEncScheme->setText( tr2i18n( "Unknown" ) );
    mySigScheme->setText( tr2i18n( "Unknown" ) );
}

#include "publickeyview.moc"
