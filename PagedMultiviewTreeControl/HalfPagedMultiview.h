// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#if !defined(HalfPagedMultiview_IncludeGuard)
#define HalfPagedMultiview_IncludeGuard

#include "MultiviewPrecompile.h"

#ifndef __AFXWIN_H__
#error "include 'MultiviewPrecompile.h' before including this file for PCH"
#endif
#include "../OtherFunctions/MFCMacros.h"

#include "resource.h"       // main symbols

#include "MultiViewDoc.h"
#include "MFView.h"
#include "AboutDlg.h"

#include "../VariableLists/VariableList.h"

/// <summary>
/// Multi-view features based on https://www.codeproject.com/Articles/7686/Using-Multiview
/// Implements the <see cref="CWinAppEx" />
/// </summary>
/// <seealso cref="CWinAppEx" />
template <typename ViewType01, typename ViewType02, typename Frame01>
class HalfPagedMultiview : public CWinAppEx
{
    /// <summary>
    /// The main view
    /// </summary>
    CView* MainView;
    /// <summary>
    /// The List holding one or more Alternative Views
    /// </summary>
    VariableList<ViewType02*> AltView;
public:
    /// <summary>
    /// The using alt view
    /// </summary>
    bool UsingAltView;

    /// <summary>
    /// Return Main view as non-CView pointer
    /// </summary>
    /// <returns>ViewType01 *.</returns>
    ViewType01* GetMainView()
    {
        if (MainView == nullptr)
        {
            return nullptr;
        }
        else
        {
            return dynamic_cast<ViewType01*>(MainView);
        }
    }
    /// <summary>
    /// Switches to alt view.
    /// </summary>
    /// <param name="Num">The number.</param>
    void SwitchToAltView(unsigned int Num = 1000000000)
    {
        if (UsingAltView == false)
        {
            UINT temp = ::GetWindowLong(AltView[CurrentAltView]->m_hWnd, GWL_ID);
            ::SetWindowLong(AltView[CurrentAltView]->m_hWnd, GWL_ID, ::GetWindowLong(MainView->m_hWnd, GWL_ID));
            ::SetWindowLong(MainView->m_hWnd, GWL_ID, temp);

            MainView->ShowWindow(SW_HIDE);
        }
        if(Num== 1000000000)//Use Current loaded AltView
        {
            AltView[CurrentAltView]->ShowWindow(SW_SHOW);
        }
        else
        {
            AltView[CurrentAltView]->ShowWindow(SW_HIDE);
            CurrentAltView = Num;
            AltView[CurrentAltView]->ShowWindow(SW_SHOW);
        }
        ((Frame01*)m_pMainWnd)->SetActiveView(AltView[CurrentAltView]);
        ((Frame01*)m_pMainWnd)->RecalcLayout();
        AltView[CurrentAltView]->Invalidate();
        UsingAltView = true;
    }

    /// <summary>
    /// Initializes a new instance of the <see cref="HalfPagedMultiview" /> class.
    /// </summary>
    /// ///////////////////////////////////////////////////////////////////////////
    /// HalfPagedMultiview construction
    HalfPagedMultiview()
    {
        UsingAltView = false;
        // TODO: add construction code here,
        // Place all significant initialization in InitInstance
    }

    //~virtual HalfPagedMultiview()
    //{
    //}


/// <summary>
/// Edit this virtual function inside Derived Class with method void InitializationCode() defined to run edit code thats run just before displays main view on InitInstance()
/// </summary>
    virtual void InitializationCode()
    {
    }
    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMultiViewApp)
public:
    /// <summary>
    /// Initializes the instance.
    /// </summary>
    /// <returns>BOOL.</returns>
    virtual BOOL InitInstance()
    {
        AfxEnableControlContainer();

        //LoadStdProfileSettings();  // Load standard INI file options (including MRU)

        // Register the application's document templates.  Document templates
        //  serve as the connection between documents, frame windows and views.

        CSingleDocTemplate* pDocTemplate;
        pDocTemplate = new CSingleDocTemplate(
            IDR_MAINFRAME,
            RUNTIME_CLASS(MultiViewDoc),
            RUNTIME_CLASS(Frame01),       // main SDI frame window
            RUNTIME_CLASS(ViewType01));
        AddDocTemplate(pDocTemplate);

        // Parse command line for standard shell commands, DDE, file open
        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);

        // Dispatch commands specified on the command line
        if (!ProcessShellCommand(cmdInfo))
        {
            return FALSE;//Fails to create empty document here
        }

        //CView* pActiveView = ((Frame01*)m_pMainWnd)->GetActiveView();
        //MainView = static_cast<ViewType01*>(pActiveView);
        CView* pActiveView = ((CFrameWnd*)m_pMainWnd)->GetActiveView();
        MainView = pActiveView;
        AltView.Add((ViewType02*) new ViewType02);

        CDocument* pDoc = ((Frame01*)m_pMainWnd)->GetActiveDocument();

        CCreateContext context;
        context.m_pCurrentDoc = pDoc;

        UINT m_ID = AFX_IDW_PANE_FIRST + 1;
        CRect rect;

        AltView[CurrentAltView]->Create(NULL, NULL, WS_CHILD, rect, m_pMainWnd, m_ID, &context);

        InitializationCode();
        // The one and only window has been initialized, so show and update it.
        m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
        m_pMainWnd->UpdateWindow();

        return TRUE;
    }
    //virtual int ExitInstance()
    //{
    //	return TRUE;
    //}
    //}}AFX_VIRTUAL


/// <summary>
/// App command to run the dialog
/// </summary>
/// Implementation
/// {{AFX_MSG(CMultiViewApp)
    afx_msg void OnAppAbout()
    {
        AboutDlg aboutDlg;
        aboutDlg.DoModal();
    }
    /// <summary>
    /// Called when [view otherview].
    /// </summary>
    afx_msg void OnViewOtherview()
    {
        SwitchToAltView();
    }
    /// <summary>
    /// Called when [view firstview].
    /// </summary>
    afx_msg void OnViewFirstview()
    {
        // TODO: Add your command handler code here

        UINT temp = ::GetWindowWord(AltView[CurrentAltView]->m_hWnd, GWL_ID);
        ::SetWindowWord(AltView[CurrentAltView]->m_hWnd, GWL_ID, ::GetWindowWord(MainView->m_hWnd, GWL_ID));
        ::SetWindowWord(MainView->m_hWnd, GWL_ID, temp);

        AltView[CurrentAltView]->ShowWindow(SW_HIDE);
        MainView->ShowWindow(SW_SHOW);

        //((FrameWindowType*)m_pMainWnd)->SetActiveView(MainView);
        //((FrameWindowType*)m_pMainWnd)->RecalcLayout();
        ((Frame01*)m_pMainWnd)->SetActiveView(MainView);
        ((Frame01*)m_pMainWnd)->RecalcLayout();
        MainView->Invalidate();
        UsingAltView = false;
    }
    //}}AFX_MSG

    BEGIN_AltMESSAGE_MAP()
        ON_COMMAND(ID_APP_ABOUT, &OnAppAbout)
        ON_COMMAND(ID_VIEW_OTHERVIEW, &OnViewOtherview)
        ON_COMMAND(ID_VIEW_FIRSTVIEW, &OnViewFirstview)
        //ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
        //ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
        //ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
    END_AltMESSAGE_MAP(CWinAppEx)
    /// <summary>
    /// The current alt view
    /// </summary>
    unsigned int CurrentAltView = 0;
    //IniDataV2 IniSettings;
    //bool m_IsLocked;
    //BOOL  m_bHiColorIcons;
    //CMultiDocTemplate * m_pDocTemplate;
};

//extern MultiviewApp<ViewType01, ViewType02 theApp;
#endif