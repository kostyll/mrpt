/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#ifndef REACTIVENAVIGATIONDEMOMAIN_H
#define REACTIVENAVIGATIONDEMOMAIN_H

//(*Headers(ReactiveNavigationDemoFrame)
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include <mrpt/otherlibs/mathplot/mathplot.h>
#include "../wx-common/CMyRedirector.h"


class ReactiveNavigationDemoFrame: public wxFrame
{
    public:

        ReactiveNavigationDemoFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~ReactiveNavigationDemoFrame();

 //   private:

        //(*Handlers(ReactiveNavigationDemoFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnbtnStartClick(wxCommandEvent& event);
        void OnbtnPauseClick(wxCommandEvent& event);
        void OnbtnExitClick(wxCommandEvent& event);
        void OnbtnNavigateClick(wxCommandEvent& event);
        void OnplotMouseMove(wxMouseEvent& event);
        void OntimSimulateTrigger(wxTimerEvent& event);
        void OnbtnResetClick(wxCommandEvent& event);
        void OnbtnEditRobotParamsClick(wxCommandEvent& event);
        void OnbtnEditNavParamsClick(wxCommandEvent& event);
        void OnrbExtMapSelect(wxCommandEvent& event);
        void OncbInternalParamsClick(wxCommandEvent& event);
        void OncbInternalParamsClick1(wxCommandEvent& event);
        //*)

		void OnreactivenavTargetMenu(wxCommandEvent& event);

        //(*Identifiers(ReactiveNavigationDemoFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_CHECKBOX3;
        static const long ID_BUTTON3;
        static const long ID_CHECKBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON6;
        static const long ID_BUTTON7;
        static const long ID_CHECKBOX2;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL5;
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL6;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL4;
        static const long ID_BUTTON4;
        static const long ID_PANEL1;
        static const long ID_CUSTOM1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATUSBAR1;
        static const long ID_TIMER1;
        //*)

		static const long ID_MENUITEM_SET_reactivenav_TARGET;

        //(*Declarations(ReactiveNavigationDemoFrame)
        wxTextCtrl* edMapFile;
        wxPanel* Panel1;
        wxTextCtrl* edX;
        mpWindow* plot;
        wxTextCtrl* edY;
        wxStatusBar* StatusBar1;
        wxTextCtrl* edRobotCfgFile;
        wxButton* btnNavigate;
        wxButton* btnPause;
        wxCheckBox* cbExtMap;
        wxStaticText* StaticText3;
        wxButton* btnEditNavParams;
        wxButton* btnExit;
        wxTextCtrl* edLog;
        wxTextCtrl* edNavCfgFile;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText2;
        wxCheckBox* cbLog;
        wxStaticText* StaticText6;
        wxTimer timSimulate;
        wxButton* btnStart;
        wxCheckBox* cbInternalParams;
        wxButton* btnEditRobotParams;
        //*)


		CMyRedirector   *myRedirector;


        bool reloadMap();
		void reloadRobotShape();
		void tryConstructReactiveNavigator();


        DECLARE_EVENT_TABLE()

	public:
        mpBitmapLayer   *lyGridmap;
        mpPolygon       *lyVehicle, *lyTarget, *lyLaserPoints;

};

#endif // REACTIVENAVIGATIONDEMOMAIN_H
