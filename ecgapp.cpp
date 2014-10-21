#include "ecgapp.h"

DEFINE_RESPONSE_TABLE1(TMyApp, TApplication)
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_WM_DROPFILES,
  EV_COMMAND(CM_HELPABOUT, CmAbout),
END_RESPONSE_TABLE;

void TMyApp::InitMainWindow()
{
  // Construct the decorated frame window
  TDecoratedMDIFrame* frame = new TDecoratedMDIFrame("ECG Display", 0,
												*(Client = new TMDIClient), TRUE);

	TControlBar *cntrlBar = new TControlBar(frame);

	// Add gadgets to control bar
	cntrlBar->Insert(*new TButtonGadget(BMP_NEW, CM_FILENEW));
	cntrlBar->Insert(*new TButtonGadget(BMP_OPEN, CM_FILEOPEN));
	cntrlBar->Insert(*new TButtonGadget(BMP_SAVE, CM_FILESAVE));
	cntrlBar->Insert(*new TSeparatorGadget(10));
	cntrlBar->Insert(*new TButtonGadget(BMP_OPTIONS, CM_ECGOPTIONS));
	cntrlBar->Insert(*new TButtonGadget(BMP_COLOUR, CM_ECGSETCOLOUR));
	cntrlBar->Insert(*new TButtonGadget(BMP_PEN_SIZES, CM_ECGPENSIZES));
	cntrlBar->Insert(*new TButtonGadget(BMP_SETZERO, CM_ECGZVALUE));
	cntrlBar->Insert(*new TButtonGadget(BMP_SCALE, CM_ECGSCALE));
	cntrlBar->Insert(*new TButtonGadget(BMP_RCTHRESH, CM_ECGRCTHRESH));
	cntrlBar->Insert(*new TSeparatorGadget(10));
	cntrlBar->Insert(*new TButtonGadget(BMP_RTHRESH, CM_ECGRTHRESH));
	cntrlBar->Insert(*new TButtonGadget(BMP_RCHANGE, CM_ECGRCHANGE));
	cntrlBar->Insert(*new TButtonGadget(BMP_RUN_MEAN, CM_ECGCALCRUNMEAN));
	cntrlBar->Insert(*new TButtonGadget(BMP_FILTER, CM_ECGFILTER));
	cntrlBar->Insert(*new TButtonGadget(BMP_MIN_CALC, CM_ECGMINCALC));
	cntrlBar->Insert(*new TButtonGadget(BMP_MAX_CALC, CM_ECGMAXCALC));
	cntrlBar->Insert(*new TButtonGadget(BMP_S_CALC, CM_ECGSCALC));
	cntrlBar->Insert(*new TButtonGadget(BMP_S_CALC2, CM_ECGSCALC2));
	cntrlBar->Insert(*new TButtonGadget(BMP_Q_CALC, CM_ECGQCALC));
	cntrlBar->Insert(*new TButtonGadget(BMP_Q_CALC2, CM_ECGQCALC2));
	cntrlBar->Insert(*new TButtonGadget(BMP_CLEAR_ANN, CM_ECGCLEARANN));
	cntrlBar->Insert(*new TButtonGadget(BMP_CLEAR_VAL, CM_ECGCLEARVAL));
	cntrlBar->Insert(*new TButtonGadget(BMP_CLEAR_ALL, CM_ECGCLEARALL));
	cntrlBar->Insert(*new TButtonGadget(BMP_INVERT, CM_ECGINVERT));
	cntrlBar->Insert(*new TSeparatorGadget(10));
	cntrlBar->Insert(*new TButtonGadget(BMP_GOOD_BAD1, CM_ECGGOODBAD));

	cntrlBar->Insert(*new TSeparatorGadget(10));
	cntrlBar->Insert(*new TButtonGadget(BMP_HELPABOUT, CM_HELPABOUT));

	// Set Hint Mode
	cntrlBar->SetHintMode(TGadgetWindow::EnterHints);

	//Create Status bar
	TStatusBar *statBar = new TStatusBar(frame, TGadget::Embossed,
		TStatusBar::CapsLock | TStatusBar::NumLock | TStatusBar::Overtype);

  // Insert the status bar and control bar into the frame
  frame->Insert(*statBar, TDecoratedFrame::Bottom);
  frame->Insert(*cntrlBar, TDecoratedFrame::Top);

  // Set the applications Icon
  frame->SetIcon(this, APP_ICON);
  // Set the main window and its menu
  SetMainWindow(frame);
//  GetMainWindow()->SetMenuDescr(TMenuDescr("MENU_1",1,1,1,0,0,1));
  GetMainWindow()->SetMenuDescr(TMenuDescr("COMMANDS",1,1,0,0,1,1));

  // Install the document manager
  SetDocManager(new TDocManager(dmMDI | dmMenu));
  
  // Lets have nice cute 3d-dialog boxed s :-)
  EnableCtl3d();
}

void
TMyApp::InitInstance()
{
  TApplication::InitInstance();
  GetMainWindow()->DragAcceptFiles(TRUE);
}

void
TMyApp::EvDropFiles(TDropInfo dropInfo)
{
  int fileCount = dropInfo.DragQueryFileCount();
  for (int index = 0; index < fileCount; index++) {
	 int fileLength = dropInfo.DragQueryFileNameLen(index)+1;
	 char* filePath = new char [fileLength];
	 dropInfo.DragQueryFile(index, filePath, fileLength);
	 TDocTemplate* tpl = GetDocManager()->MatchTemplate(filePath);
	 if (tpl)
		tpl->CreateDoc(filePath);
	 delete filePath;
  }
  dropInfo.DragFinish();
}

void
TMyApp::EvNewView(TView& view)
{
  TMDIChild* child = new TMDIChild(*Client, 0, view.GetWindow());
  if (view.GetViewMenu())
	 child->SetMenuDescr(*view.GetViewMenu());
  child->SetIcon(this, MDI_ICON1);
  child->Create();
}

void
TMyApp::EvCloseView(TView& /*view*/)
{  // nothing needs to be done here for MDI
}

void TMyApp::CmAbout()
{
  TDialog(GetMainWindow(), IDD_ABOUT).Execute();
}

int OwlMain(int /*argc*/, char* /*argv*/ [])
{
  return TMyApp().Run();
}
