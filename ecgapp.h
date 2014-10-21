#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\decmdifr.h>
#include <owl\dialog.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\statusba.h>
#include <owl\docmanag.h>
//#include <owl\docview.h>
#include <stdlib.h>
#include <string.h>
#include "ecgapp.rc"
#include "ecgdv.rc"
#include "ecgprint.rc"
#include "ecgdialg.h"

class TMyApp : public TApplication {
  public:
    TMyApp() : TApplication() {}

  protected:
    // Override methods of TApplication
    void InitInstance();
    void InitMainWindow();

    // Event handlers
    void EvNewView  (TView& view);
    void EvCloseView(TView& view);
    void EvDropFiles(TDropInfo dropInfo);
    void CmAbout();

    TMDIClient* Client;
  DECLARE_RESPONSE_TABLE(TMyApp);
};



