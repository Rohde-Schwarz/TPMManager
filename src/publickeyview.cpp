#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './publickeyview.ui'
**
** Created: Thu Dec 21 16:07:57 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "publickeyview.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
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
    PublicKeyViewWidgetLayout = new QVBoxLayout( this, 11, 6, "PublicKeyViewWidgetLayout"); 

    layout92 = new QVBoxLayout( 0, 16, 6, "layout92"); 

    layout89 = new QGridLayout( 0, 1, 1, 0, 6, "layout89"); 
    spacer80_5 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout89->addItem( spacer80_5, 4, 1 );
    spacer80 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout89->addItem( spacer80, 0, 1 );

    myKeySize = new QLabel( this, "myKeySize" );

    layout89->addWidget( myKeySize, 3, 2 );

    textLabel1_2_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2_2" );

    layout89->addWidget( textLabel1_2_2_2_2_2, 3, 0 );

    textLabel1_2_2 = new QLabel( this, "textLabel1_2_2" );

    layout89->addWidget( textLabel1_2_2, 2, 0 );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    textLabel1_2->setFrameShape( QLabel::NoFrame );
    textLabel1_2->setFrameShadow( QLabel::Plain );

    layout89->addWidget( textLabel1_2, 1, 0 );

    textLabel1_2_2_2 = new QLabel( this, "textLabel1_2_2_2" );
    textLabel1_2_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 1, 0, textLabel1_2_2_2->sizePolicy().hasHeightForWidth() ) );
    textLabel1_2_2_2->setAlignment( int( QLabel::AlignVCenter ) );

    layout89->addWidget( textLabel1_2_2_2, 4, 0 );

    mySigScheme = new QLabel( this, "mySigScheme" );

    layout89->addWidget( mySigScheme, 5, 2 );
    spacer80_3 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout89->addItem( spacer80_3, 2, 1 );

    myEncScheme = new QLabel( this, "myEncScheme" );

    layout89->addWidget( myEncScheme, 4, 2 );
    spacer80_2 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout89->addItem( spacer80_2, 1, 1 );
    spacer80_4 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout89->addItem( spacer80_4, 3, 1 );

    myVersion = new QLabel( this, "myVersion" );

    layout89->addWidget( myVersion, 0, 2 );

    myType = new QLabel( this, "myType" );

    layout89->addWidget( myType, 1, 2 );

    textLabel1_2_2_2_2 = new QLabel( this, "textLabel1_2_2_2_2" );
    textLabel1_2_2_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, textLabel1_2_2_2_2->sizePolicy().hasHeightForWidth() ) );

    layout89->addWidget( textLabel1_2_2_2_2, 5, 0 );

    myAlgorithm = new QLabel( this, "myAlgorithm" );

    layout89->addWidget( myAlgorithm, 2, 2 );
    spacer80_6 = new QSpacerItem( 16, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    layout89->addItem( spacer80_6, 5, 1 );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setEnabled( TRUE );

    layout89->addWidget( textLabel1, 0, 0 );
    layout92->addLayout( layout89 );
    spacer89 = new QSpacerItem( 54, 16, QSizePolicy::Minimum, QSizePolicy::Fixed );
    layout92->addItem( spacer89 );

    layout90 = new QHBoxLayout( 0, 0, 6, "layout90"); 
    spacer86 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout90->addItem( spacer86 );

    buttonOk = new QPushButton( this, "buttonOk" );
    layout90->addWidget( buttonOk );
    spacer88 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout90->addItem( spacer88 );
    layout92->addLayout( layout90 );
    PublicKeyViewWidgetLayout->addLayout( layout92 );
    languageChange();
    resize( QSize(370, 279).expandedTo(minimumSizeHint()) );
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
    myKeySize->setText( tr2i18n( "01010000" ) );
    textLabel1_2_2_2_2_2->setText( tr2i18n( "<p align=\"right\">Key Size</p>" ) );
    textLabel1_2_2->setText( tr2i18n( "<p align=\"right\">Algorithm</p>" ) );
    textLabel1_2->setText( tr2i18n( "<p align=\"right\">Type</p>" ) );
    textLabel1_2_2_2->setText( tr2i18n( "<p align=\"right\">Encryption Scheme</p>" ) );
    mySigScheme->setText( tr2i18n( "Unknown" ) );
    myEncScheme->setText( tr2i18n( "Unknown" ) );
    myVersion->setText( tr2i18n( "01010000" ) );
    myType->setText( tr2i18n( "01010000" ) );
    textLabel1_2_2_2_2->setText( tr2i18n( "<p align=\"right\">Signature Scheme</p>" ) );
    myAlgorithm->setText( tr2i18n( "01010000" ) );
    textLabel1->setText( tr2i18n( "<p align=\"right\">Version</p>" ) );
    buttonOk->setText( tr2i18n( "OK" ) );
}

#include "publickeyview.moc"
