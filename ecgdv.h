#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <owl\listbox.h>
#include <owl\scroller.h>
#include <classlib\arrays.h>
#include "ecgdv.rc"
#include "ecgprint.rc"
#include "ecgclass.h"
#include "ecgdialg.h"
#include "ecganal.h"

const int vnRePaint = vnCustomBase + 0;
NOTIFY_SIG(vnRePaint, long)
#define EV_VN_REPAINT VN_DEFINE(vnRePaint, VnRePaint, long)


class _DOCVIEWCLASS TDrawDocument : public TFileDocument
{
public:
	 TDrawDocument(TDocument* parent = 0)
		  : TFileDocument(parent) { ecg = NULL; }
	~TDrawDocument() { delete ecg; }

	 // implement virtual methods of TDocument
	 BOOL   Open(int mode, const char far* path=0);
	 BOOL   Close();
	 BOOL   IsOpen() { return ecg != NULL; }
	 BOOL   Commit(BOOL force = FALSE);
	 BOOL   Revert(BOOL clear = FALSE);

	 void RePaintViews(TRect* rect = NULL);

	 // Data access functions
	 const Tecg* GetECG();

protected:
	 string FileInfo;
	Tecg* ecg;
};

class _DOCVIEWCLASS TDrawView : public TWindowView
{
  public:
	 TDrawView(TDrawDocument& doc, TWindow *parent = 0);
	~TDrawView() { }
	 static const char far* StaticName() {return "Draw View";}
	 const char far* GetViewName(){return StaticName();}

  protected:
	 TDrawDocument* DrawDoc;  // same as Doc member, but cast to derived class

	 // Message response functions
	 void EvLButtonDown(UINT, TPoint&);
	 void EvRButtonDown(UINT, TPoint&);
	 void EvMouseMove(UINT, TPoint&);
	 void EvLButtonUp(UINT, TPoint&);
	 void Paint(TDC&, BOOL, TRect&);

	 // Document notifications
	 BOOL VnCommit(BOOL force);
	 BOOL VnRevert(BOOL clear);
	 BOOL VnRePaint(long area = NULL);

	 // Event Response Stuff
	 void CmECGOptions();
	 void CmECGSetColour();
	 void CmECGPenSizes();
	 void CmECGScale();
	 void CmECGRCThresh();
	 void CmECGRThresh();
	 void CmECGRChange();
	 void CmECGMinCalc();
	 void CmECGMaxCalc();
	 void CmECGClearAnn();
	 void CmECGClearVal();
	 void CmECGClearAll();
	 void CmECGZValue();
	 void CmECGSCalc();
	 void CmECGQCalc();
	 void CmECGSCalc2();
	 void CmECGQCalc2();
	 void CmECGGoodBad();
	 void CmECGCalcRunMean();
	 void CmECGInvert();
	 void CmECGFilter();

	 // Command ewnablers.
	 void CmEnableECGScale(TCommandEnabler &commandEnabler);
	 void CmEnableECGRCThresh(TCommandEnabler &commandEnabler);
	 void CmEnableECGRThresh(TCommandEnabler &commandEnabler);
	 void CmEnableECGRChange(TCommandEnabler &commandEnabler);
	 void CmEnableECGMinCalc(TCommandEnabler &commandEnabler);
	 void CmEnableECGMaxCalc(TCommandEnabler &commandEnabler);
	 void CmEnableECGClearAnn(TCommandEnabler &commandEnabler);
	 void CmEnableECGClearVal(TCommandEnabler &commandEnabler);
	 void CmEnableECGClearAll(TCommandEnabler &commandEnabler);
	 void CmEnableECGZValue(TCommandEnabler &commandEnabler);
	 void CmEnableECGSetColour(TCommandEnabler &commandEnabler);
	 void CmEnableECGPenSizes(TCommandEnabler &commandEnabler);
	 void CmEnableECGOptions(TCommandEnabler &commandEnabler);
	 void CmEnableECGSCalc(TCommandEnabler &commandEnabler);
	 void CmEnableECGQCalc(TCommandEnabler &commandEnabler);
	 void CmEnableECGGoodBad(TCommandEnabler &commandEnabler);
	 void CmEnableECGCalcRunMean(TCommandEnabler &commandEnabler);
	 void CmEnableECGInvert(TCommandEnabler &commandEnabler);
	 void CmEnableECGFilter(TCommandEnabler &commandEnabler);

	 // Scrollbar responce
	 //void CmSetScroll();
	 //void SetScroller();

	 DECLARE_RESPONSE_TABLE(TDrawView);
};

class _DOCVIEWCLASS TDrawListView : public TListBox, public TView {
	public:
		TDrawListView(TDrawDocument& doc, TWindow* parent = 0);
		~TDrawListView(){}
		static const char far* StaticName() {return "DrawList View";}
		int CurIndex;

		// Overridden virtuals from TView
		const char far* GetViewName() {return StaticName();}
		TWindow* GetWindow() {return (TWindow*)this;}
		BOOL SetDocTitle(const char far* docname,int index)
								{return TListBox::SetDocTitle(docname, index);}

		// Overridden virtuals from TWindow
		BOOL CanClose() {return TListBox::CanClose() && Doc->CanClose();}
		BOOL Create();

	protected:
		TDrawDocument* DrawDoc;  // same as Doc member, but cast to derived class
		void LoadData();
		void FormatData(const Tecg* ecg, unsigned int index);

		// Message response functions
		// Again not suer if i need these
		//

		// Document notifications
		BOOL VnIsWindow(HWND hWnd) {return HWindow == hWnd;}
		BOOL VnCommit(BOOL force);
		BOOL VnRevert(BOOL clear);
//		BOOL VnAppend(unsigned int index);
//		BOOL VnDelete(unsigned int index);
		BOOL VnRePaint(long area = NULL);

	DECLARE_RESPONSE_TABLE(TDrawListView);
};
