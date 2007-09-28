#ifndef SETSRKVIEW_HH
#define SETSRKVIEW_HH

/**
	@author Anoosheh Zaerin <anoosheh.zaerin@rub.de>
*/

#include <setsrkdialog.h>
#include <qradiobutton.h>
#include <kmessagebox.h>

class SetSRKView : public SetSRKDialog{

public:
    SetSRKView();
    ~SetSRKView();
	inline bool setManually();

protected slots:
	void slotSetSrk();	
	
private:
	bool myManuallySRKRadioButton;	
	bool myDefaultSRKRadioButton;
};

bool SetSRKView::setManually(){
	if ( myManuallySRKRadioButton )
		return true;
	else 
		return false;
}

#endif
