#include "ecgdv.h"

DEFINE_DOC_TEMPLATE_CLASS(TDrawDocument, TDrawView,   DrawTemplate);
DEFINE_DOC_TEMPLATE_CLASS(TDrawDocument, TDrawListView, DrawListTemplate);
DrawTemplate drawTpl("ECG Data (*.TXT)","*.txt",0,"TXT",dtAutoDelete|dtUpdateDir);
DrawListTemplate drawListTpl("ECGs","  *.txt",0,"TXT",dtAutoDelete|dtHidden);

DEFINE_RESPONSE_TABLE1(TDrawView, TWindowView)
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_LBUTTONUP,
  EV_VN_COMMIT,
  EV_VN_REVERT,
  EV_VN_REPAINT,

  EV_COMMAND(CM_ECGOPTIONS, CmECGOptions),
  EV_COMMAND(CM_ECGSETCOLOUR, CmECGSetColour),
  EV_COMMAND(CM_ECGPENSIZES, CmECGPenSizes),
  EV_COMMAND(CM_ECGSCALE, CmECGScale),
  EV_COMMAND(CM_ECGRCTHRESH, CmECGRCThresh),
  EV_COMMAND(CM_ECGRTHRESH, CmECGRThresh),
  EV_COMMAND(CM_ECGRCHANGE, CmECGRChange),
  EV_COMMAND(CM_ECGMINCALC, CmECGMinCalc),
  EV_COMMAND(CM_ECGMAXCALC, CmECGMaxCalc),
  EV_COMMAND(CM_ECGCLEARANN, CmECGClearAnn),
  EV_COMMAND(CM_ECGCLEARVAL, CmECGClearVal),
  EV_COMMAND(CM_ECGCLEARALL, CmECGClearAll),
  EV_COMMAND(CM_ECGZVALUE, CmECGZValue),
  EV_COMMAND(CM_ECGSCALC, CmECGSCalc),
  EV_COMMAND(CM_ECGQCALC, CmECGQCalc),
  EV_COMMAND(CM_ECGSCALC2, CmECGSCalc2),
  EV_COMMAND(CM_ECGQCALC2, CmECGQCalc2),
  EV_COMMAND(CM_ECGGOODBAD, CmECGGoodBad),
  EV_COMMAND(CM_ECGCALCRUNMEAN, CmECGCalcRunMean),
  EV_COMMAND(CM_ECGINVERT, CmECGInvert),
  EV_COMMAND(CM_ECGFILTER, CmECGFilter),


  EV_COMMAND_ENABLE(CM_ECGSCALE, CmEnableECGScale),
  EV_COMMAND_ENABLE(CM_ECGOPTIONS, CmEnableECGScale),//can use ...
  EV_COMMAND_ENABLE(CM_ECGSETCOLOUR, CmEnableECGScale),//can use ...
  EV_COMMAND_ENABLE(CM_ECGPENSIZES, CmEnableECGScale),//can use ...
  EV_COMMAND_ENABLE(CM_ECGRCTHRESH, CmEnableECGScale),//can use ...
  EV_COMMAND_ENABLE(CM_ECGRTHRESH, CmEnableECGRThresh),
  EV_COMMAND_ENABLE(CM_ECGRCHANGE, CmEnableECGScale), // Can use ECGScales
  EV_COMMAND_ENABLE(CM_ECGMINCALC, CmEnableECGSCalc), // Can use ECGScales
  EV_COMMAND_ENABLE(CM_ECGMAXCALC, CmEnableECGSCalc), // Can use ECGScales
  EV_COMMAND_ENABLE(CM_ECGCLEARANN, CmEnableECGScale),   // CMEnable for these
  EV_COMMAND_ENABLE(CM_ECGCLEARVAL, CmEnableECGScale),   // CMEnable for these
  EV_COMMAND_ENABLE(CM_ECGCLEARALL, CmEnableECGScale),   // CMEnable for these
  EV_COMMAND_ENABLE(CM_ECGZVALUE, CmEnableECGScale),   // ...
  EV_COMMAND_ENABLE(CM_ECGSCALC, CmEnableECGSCalc),
  EV_COMMAND_ENABLE(CM_ECGQCALC, CmEnableECGQCalc),
  EV_COMMAND_ENABLE(CM_ECGSCALC2, CmEnableECGSCalc), // These can use the same
  EV_COMMAND_ENABLE(CM_ECGQCALC2, CmEnableECGQCalc), // enablers as S&Q Calc 1
  EV_COMMAND_ENABLE(CM_ECGGOODBAD, CmEnableECGSCalc), // Should have its own****
  EV_COMMAND_ENABLE(CM_ECGCALCRUNMEAN, CmEnableECGScale),
  EV_COMMAND_ENABLE(CM_ECGINVERT, CmEnableECGScale),
  EV_COMMAND_ENABLE(CM_ECGFILTER, CmEnableECGScale),

  // scroll responce
  //EV_COMMAND(CM_SETSCROLL, CmSetScroll),

END_RESPONSE_TABLE;

DEFINE_RESPONSE_TABLE1(TDrawListView, TListBox)
//  EV_COMMAND(CM_SETSCALE, CmSetScale),
  EV_VN_REPAINT,
  EV_VN_ISWINDOW,
  EV_VN_COMMIT,
  EV_VN_REVERT,
END_RESPONSE_TABLE;

BOOL TDrawDocument::Commit(BOOL force)
{
  if (!IsDirty() && !force)
	 return TRUE;

  TOutStream* os = OutStream(ofWrite);
  if (!os)
	 return FALSE;

	 *os << *ecg;

  delete os;

  SetDirty(FALSE);
  return TRUE;
}

BOOL TDrawDocument::Revert(BOOL clear)
{
//  OutputDebugString("TdrawDocument::Revert Called");
  if (!/*TFileDocument*/TFileDocument::Revert(clear))
	 return FALSE;
  if (!clear)
  {
//		OutputDebugString("Tdrawdocument::Revert clear should be calling open");
		Open(0);
  }
  return TRUE;
}

void TDrawDocument::RePaintViews(TRect* rect)
{
	if(rect)
		NotifyViews(vnRePaint, (long) rect);
	else
		NotifyViews(vnRePaint);
}

BOOL TDrawDocument::Open(int /*mode*/, const char far* path)
{
//	if(ecg)
//		delete ecg;
	ecg = new Tecg();

  if (path)
	 SetDocPath(path);
  if (GetDocPath())
  {
	 TInStream* is = InStream(ofRead);
	 if (!is)
		return FALSE;

	*is >> *ecg;
	 delete is;
  }
  SetDirty(FALSE);
  return TRUE;
}

BOOL TDrawDocument::Close()
{
	if(ecg)
		delete ecg;
	ecg = NULL;
  return TRUE;
}

//BOOL TDrawDocument::Paint(TDC &dc)
//{
//	if(ecg)
// SHOULD NOT BE HERE, SEE DRAWVIEWS VERSION
//}

const Tecg* TDrawDocument::GetECG()
{
	if (!IsOpen() && !Open(ofRead | ofWrite))
		return 0;
	return ecg;
}

TDrawView::TDrawView(TDrawDocument& doc,TWindow *parent)
			: TWindowView(doc,parent), DrawDoc(&doc)
{
	// Add scroll bars
	Attr.Style |= WS_VSCROLL | WS_HSCROLL;
	// Create OWL scroller object
	Scroller = new TScroller(this, 1, 1, 640, 480);
	Scroller->XLine = 10;
	Scroller->YLine = 10;
}

void TDrawView::EvLButtonDown(UINT, TPoint& point)
{
	const Tecg* ecg;
	if ((ecg = DrawDoc->GetECG()) != 0)
	{
		const_cast<Tecg*>(ecg)->set_r_thresh(point.y + Scroller->YPos);
		DrawDoc->NotifyViews(vnRePaint);
	}
}

void TDrawView::EvRButtonDown(UINT, TPoint&)
{
}

void TDrawView::EvMouseMove(UINT, TPoint& /*point*/)
{
}

void TDrawView::EvLButtonUp(UINT, TPoint&)
{
}

void TDrawView::Paint(TDC& dc, BOOL, TRect&)
{
//	DrawDoc->Paint(dc);
	const Tecg* ecg;
	if ((ecg = DrawDoc->GetECG()) != 0)
		ecg->Paint(dc);
}

BOOL TDrawView::VnCommit(BOOL /*force*/)
{
  // nothing to do here, no data held in view
  return TRUE;
}

BOOL TDrawView::VnRevert(BOOL /*clear*/)
{
  Invalidate();  // force full repaint
  return TRUE;
}

BOOL TDrawView::VnRePaint(long area)
{
	TRect* rect = (TRect*) area;

	if(rect)
		InvalidateRect(*rect);
	else
		Invalidate();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGOptions()
//
// Responds to CM_ECGOPTIONS message generated by the menu item or button.
// Opening up the dialog box.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGOptions()
{
	TBufOptions options;
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();


	options.zero_volt = ecg->get_zvanal_on();
	options.zero_level = (ecg->get_which_level()==LEVELIS_ZERO);
	options.mean_level = (ecg->get_which_level()==LEVELIS_MEAN);
	options.run_mean_level = (ecg->get_which_level()==LEVELIS_RUNMEAN);

	wsprintf(options.edit_xscale, "%d", ecg->get_x_scale());
	wsprintf(options.edit_yscale, "%d", ecg->get_y_scale());
	wsprintf(options.edit_rchange, "%d", ecg->get_r_change());
	wsprintf(options.edit_rvolts, "%d", ecg->get_r_thresh());
	wsprintf(options.edit_zvolts, "%d", ecg->get_zero_level());
	wsprintf(options.edit_points, "%d", ecg->get_one_in_n());
	options.points_display = ecg->get_one_in_n_display();
	options.points_analysis = ecg->get_one_in_n_analysis();
	options.min_for_qwav = (ecg->get_min_for()==MINIS_QWAV);
	options.min_for_swav = (ecg->get_min_for()==MINIS_SWAV);
	options.min_for_auto = (ecg->get_min_for()==MINIS_AUTO);
	options.min_for_both = (ecg->get_min_for()==MINIS_BOTH);
	options.max_for_qwav = (ecg->get_max_for()==MAXIS_PWAV);
	options.max_for_swav = (ecg->get_max_for()==MAXIS_TWAV);
	options.max_for_auto = (ecg->get_max_for()==MAXIS_AUTO);
	options.max_for_both = (ecg->get_max_for()==MAXIS_BOTH);


	TDialog *dialog = new TDlgOptions( this, IDD_ECG_OPTIONS, &options);
	int result = dialog->Execute();
	if (result == IDOK)
	{
		const_cast<Tecg*>(ecg)->set_zvanal_on( options.zero_volt );
		if (options.zero_level)
			const_cast<Tecg*>(ecg)->set_which_level( LEVELIS_ZERO );
		else if (options.mean_level)
			const_cast<Tecg*>(ecg)->set_which_level( LEVELIS_MEAN );
		else if (options.run_mean_level)
			const_cast<Tecg*>(ecg)->set_which_level( LEVELIS_RUNMEAN );

		const_cast<Tecg*>(ecg)->set_x_scale(atoi(options.edit_xscale));
		const_cast<Tecg*>(ecg)->set_y_scale(atoi(options.edit_yscale));
		const_cast<Tecg*>(ecg)->set_r_change(atoi(options.edit_rchange));
		const_cast<Tecg*>(ecg)->set_r_thresh_real(atoi(options.edit_rvolts));
		const_cast<Tecg*>(ecg)->set_zero_level(atoi(options.edit_zvolts));

		const_cast<Tecg*>(ecg)->set_one_in_n(atoi(options.edit_points));
		const_cast<Tecg*>(ecg)->set_one_in_n_display(options.points_display);
		const_cast<Tecg*>(ecg)->set_one_in_n_analysis(options.points_analysis);

		if (options.min_for_qwav)
			const_cast<Tecg*>(ecg)->set_min_for( MINIS_QWAV );
		else if (options.min_for_swav)
			const_cast<Tecg*>(ecg)->set_min_for( MINIS_SWAV );
		else if (options.min_for_auto)
			const_cast<Tecg*>(ecg)->set_min_for( MINIS_AUTO );
		else if (options.min_for_both)
			const_cast<Tecg*>(ecg)->set_min_for( MINIS_BOTH );

		if (options.max_for_qwav)
			const_cast<Tecg*>(ecg)->set_max_for( MAXIS_PWAV );
		else if (options.max_for_swav)
			const_cast<Tecg*>(ecg)->set_max_for( MAXIS_TWAV );
		else if (options.max_for_auto)
			const_cast<Tecg*>(ecg)->set_max_for( MAXIS_AUTO );
		else if (options.max_for_both)
			const_cast<Tecg*>(ecg)->set_max_for( MAXIS_BOTH );


		Scroller->SetRange((ecg->length()/ecg->get_x_scale()) + 21,
				  (ecg->get_y_max() - ecg->get_y_min())/ecg->get_y_scale() +20);

		// make sure things are re-drawn
		DrawDoc->RePaintViews();
	}
}


/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGSetColour()
//
// Responds to CM_ECGSETCOLOUR message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGSetColour()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TColors temp_cols;
	TColors cust_cols;
	// take copy of colours
	temp_cols=ecg->get_colors();
	cust_cols=ecg->get_cust_colors();
	TDialog *dialog = new TDlgColour(this, IDD_COLOUR, &temp_cols, &cust_cols);
	int result = dialog->Execute();
	if (result == IDOK)
	{  // Copy new colours back
		const_cast<Tecg*>(ecg)->set_colors( temp_cols );
		const_cast<Tecg*>(ecg)->set_cust_colors( cust_cols );
		DrawDoc->RePaintViews();
	}
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGPenSizes()
//
// Responds to CM_ECGPENSIZES message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGPenSizes()
{
	TBufPenSizes pensizes;
	const Tecg* ecg;
	TPenSizes psizes;
	ecg = DrawDoc->GetECG();
	psizes = ecg->get_pen_sizes();

	wsprintf(pensizes.edit_p_thick, "%d", psizes[COLPWAV]);
	wsprintf(pensizes.edit_q_thick, "%d", psizes[COLQWAV]);
	wsprintf(pensizes.edit_r_thick, "%d", psizes[COLRWAV]);
	wsprintf(pensizes.edit_s_thick, "%d", psizes[COLSWAV]);
	wsprintf(pensizes.edit_t_thick, "%d", psizes[COLTWAV]);
	wsprintf(pensizes.edit_undef_thick, "%d", psizes[COLUNDEF]);
	wsprintf(pensizes.edit_zero_thick, "%d", psizes[COLZERO]);
	wsprintf(pensizes.edit_mean_thick, "%d", psizes[COLMEAN]);
	wsprintf(pensizes.edit_run_mean_thick, "%d", psizes[COLRUNMEAN]);
	wsprintf(pensizes.edit_rthresh_thick, "%d", psizes[COLRTHRESH]);
OutputDebugString(pensizes.edit_r_thick);
	TDialog *dialog = new TDlgPenSizes( this, IDD_PENSIZES, &pensizes);
	int result = dialog->Execute();
	if (result == IDOK)
	{  // Copy new values back
OutputDebugString(pensizes.edit_r_thick);

		psizes[COLPWAV] = atoi(pensizes.edit_p_thick);
		psizes[COLQWAV] = atoi(pensizes.edit_q_thick);
		psizes[COLRWAV] = atoi(pensizes.edit_r_thick);
		psizes[COLSWAV] = atoi(pensizes.edit_s_thick);
		psizes[COLTWAV] = atoi(pensizes.edit_t_thick);
		psizes[COLUNDEF] = atoi(pensizes.edit_undef_thick);
		psizes[COLZERO] = atoi(pensizes.edit_zero_thick);
		psizes[COLMEAN] = atoi(pensizes.edit_mean_thick);
		psizes[COLRUNMEAN] = atoi(pensizes.edit_run_mean_thick);
		psizes[COLRTHRESH] = atoi(pensizes.edit_rthresh_thick);

		const_cast<Tecg*>(ecg)->set_pen_sizes(psizes);
		// make sure things are re-drawn
		DrawDoc->RePaintViews();
	}
}


/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGScale()
//
// Responds to CM_ECGSCALE message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGScale()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	char inputText[6];

	wsprintf(inputText, "%d", ecg->get_x_scale()); // Realy should be both scales here******
//	OutputDebugString("CmECGScale : about to create and execute dialog box");
	if (TInputDialog(this, "ECG Scale",
								"Input a value for scale",
								inputText,
								sizeof(inputText)).Execute() == IDOK){
		if (atoi(inputText)>0){
			int result = atoi(inputText);
			const_cast<Tecg*>(ecg)->set_x_scale(result);
			const_cast<Tecg*>(ecg)->set_y_scale(result);
		}else{
			const_cast<Tecg*>(ecg)->set_x_scale(1);
			const_cast<Tecg*>(ecg)->set_y_scale(1);
		}
	}
	Scroller->SetRange((ecg->length()/ecg->get_x_scale()) + 21,
							  (ecg->get_y_max() - ecg->get_y_min())/ecg->get_y_scale() +20);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmEnableECGScale()
//
// command enabler for ECGSCALE button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmEnableECGScale(TCommandEnabler &commandEnabler)
{
	// Enable the scakle command if an ECG exists.
	if (DrawDoc->IsOpen())
		commandEnabler.Enable(TRUE);	// if pointer to ecgdata=NULL then disable
	else
		commandEnabler.Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGRCThresh()
//
// Responds to CM_ECGRCTHRESH message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGRCThresh()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	char inputText[6];

	wsprintf(inputText, "%d", ecg->get_r_change()); // Realy should be both scales here******
	if (TInputDialog(this, "ECG R Change Threshold",
								"Input Y change minumum for RWAVE",
								inputText,
								sizeof(inputText)).Execute() == IDOK){
		if (atoi(inputText)>0){
			int result = atoi(inputText);
			const_cast<Tecg*>(ecg)->set_r_change(result);
		}else{
			const_cast<Tecg*>(ecg)->set_r_change(1);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGRThresh()
//
// Responds to CM_ECGRTHRESH message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGRThresh()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_R_Thresh analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(R_THRESH);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmEnableECGRThresh()
//
// command enabler for RTHRESH button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmEnableECGRThresh(TCommandEnabler &commandEnabler)
{
	// Enable the scakle command if an ECG exists.
	if (DrawDoc->IsOpen())
	{
		if ((DrawDoc->GetECG())->get_r_thresh()!=-1)
			commandEnabler.Enable(TRUE);
		else
			commandEnabler.Enable(FALSE);
	}
	else
		commandEnabler.Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGRChange()
//
// Responds to CM_ECGRCHANGE message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGRChange()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_R_Change analyse(ecg);
	analyse.Execute();
  	//const_cast<Tecg*>(ecg)->AnalyseData(R_CHANGE);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGMinCalc()
//
// Responds to CM_ECGMINCALC message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGMinCalc()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Min_Calc_1 analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(MIN_CALC_1);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGMaxCalc()
//
// Responds to CM_ECGMAXCALC message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGMaxCalc()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Max_Calc_1 analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(MAX_CALC_1);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGClearAnn()
//
// Responds to CM_ECGCLEARANN message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGClearAnn()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Clear_Ann analyse(ecg);
	analyse.Execute();
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGClearVal()
//
// Responds to CM_ECGCLEARVAL message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGClearVal()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Clear_Val analyse(ecg);
	analyse.Execute();
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGClearAll()
//
// Responds to CM_ECGCLEARALL message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGClearAll()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Clear_All analyse(ecg);
	analyse.Execute();
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGZValue()
//
// Responds to CM_ECGZVALUE message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGZValue()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	char inputText[6];

	wsprintf(inputText, "%d", ecg->get_zero_level());
	if (TInputDialog(this, "Zero Value",
								"Input a value for zero Volts",
								inputText,
								sizeof(inputText)).Execute() == IDOK){
		if (atoi(inputText)>0){
			int result = atoi(inputText);
			const_cast<Tecg*>(ecg)->set_zero_level(result);
		}else{
			const_cast<Tecg*>(ecg)->set_zero_level(2048); // ("default" value)
		}
	}
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGSCalc()
//
// Responds to CM_ECGSCALC message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGSCalc()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_S_Calc_1 analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(S_CALC_1);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGSCalc2()
//
// Responds to CM_ECGSCALC2 message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGSCalc2()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_S_Calc_2 analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(S_CALC_2);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmEnableECGSCalc()
//
// command enabler for SCALC button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmEnableECGSCalc(TCommandEnabler &commandEnabler)
{
	// enable if ecg exists, && it has already been analysed (so we have rwaves)
	if (DrawDoc->IsOpen())
	{
		if ((DrawDoc->GetECG())->IsAnalysed())
			commandEnabler.Enable(TRUE);
		else
			commandEnabler.Enable(FALSE);
	}
	else
		commandEnabler.Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGQCalc()
//
// Responds to CM_ECGQCALC message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGQCalc()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Q_Calc_1 analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(Q_CALC_1);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGQCalc2()
//
// Responds to CM_ECGQCALC2 message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGQCalc2()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Q_Calc_2 analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(Q_CALC_2);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmEnableECGQCalc()
//
// command enabler for QCALC button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmEnableECGQCalc(TCommandEnabler &commandEnabler)
{
	// enable if ecg exists, && it has already been analysed (so we have rwaves)
	if (DrawDoc->IsOpen())
	{
		if ((DrawDoc->GetECG())->IsAnalysed())
			commandEnabler.Enable(TRUE);
		else
			commandEnabler.Enable(FALSE);
	}
	else
		commandEnabler.Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGGoodBad()
//
// Responds to CM_ECGGOODBAD message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGGoodBad()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TAnalyse_Good_Or_Bad analyse(ecg);
	analyse.Execute();
	//const_cast<Tecg*>(ecg)->AnalyseData(GOOD_OR_BAD_1);
	DrawDoc->RePaintViews();
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGCalcRunMean()
//
// Responds to CM_ECGCALCRUNMEAN message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGCalcRunMean()
{
	TBufRunMean rmbuff;
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();

	rmbuff.full = (ecg->get_run_mean_calc() == TRMC_FULL);
	rmbuff.rtor = (ecg->get_run_mean_calc() == TRMC_RTR);
	rmbuff.rtorwin = (ecg->get_run_mean_calc() == TRMC_RTRWIN);
	wsprintf(rmbuff.edit_winsize, "%d", ecg->get_run_mean_winsize());

	TDialog *dialog = new TDlgRunMean( this, IDD_RUNNINGMEAN, &rmbuff);
	int result = dialog->Execute();
	if (result == IDOK)
	{
		if (rmbuff.full)
			const_cast<Tecg*>(ecg)->set_run_mean_calc( TRMC_FULL );
		else if (rmbuff.rtor)
			const_cast<Tecg*>(ecg)->set_run_mean_calc( TRMC_RTR );
		else if (rmbuff.rtorwin)
			const_cast<Tecg*>(ecg)->set_run_mean_calc( TRMC_RTRWIN );

		const_cast<Tecg*>(ecg)->set_run_mean_winsize(atoi(rmbuff.edit_winsize));

		TAnalyse_Run_Mean analyse(ecg);
		analyse.Execute();
		//const_cast<Tecg*>(ecg)->AnalyseData(CALC_RUN_MEAN_1);
		DrawDoc->RePaintViews();
	}
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGInvert()
//
// Responds to CM_ECGINVERT message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGInvert()
{
	if ( MessageBox("Are you sure?", "Invert ECG", MB_YESNO) == IDYES)
	{
		const Tecg* ecg;
		ecg = DrawDoc->GetECG();
		TAnalyse_Invert analyse(ecg);
		analyse.Execute();
		//const_cast<Tecg*>(ecg)->AnalyseData(INVERT);
		DrawDoc->RePaintViews();
	}
}

/////////////////////////////////////////////////////////////////////////////
// TDrawView::CmECGFilter()
//
// Responds to CM_ECGFILTER message generated by the menu item or button.
/////////////////////////////////////////////////////////////////////////////
void TDrawView::CmECGFilter()
{
	const Tecg* ecg;
	ecg = DrawDoc->GetECG();
	TModule* moduleid = /*GetWindow()->*/GetModule();
	TBufFilter filtbuff;
	char far *buffer = new char[100];
	for (int line=IDS_FILT_FIRST+1 ; line < IDS_FILT_LAST ; line++)
	{
		moduleid->LoadString(line, buffer, 100);
		filtbuff.combo_filt_type.AddString(buffer);
	}
	filtbuff.combo_filt_type.Select(ecg->get_cur_filt_data().Mode-IDS_FILT_FIRST-1);
	delete buffer; // dont want to be leeking
	wsprintf(filtbuff.edit_optval, "%d", ecg->get_cur_filt_data().Opt_Val); // set initial optional value (not always used)

	TDialog *dialog = new TDlgFilter( this, IDD_FILTER, &filtbuff);

	int result = dialog->Execute();
	if (result == IDOK)
	{
		const_cast<Tecg*>(ecg)->set_cur_filt_data(filtbuff.combo_filt_type.GetSelIndex() +IDS_FILT_FIRST +1
                              , atoi(filtbuff.edit_optval));
		TAnalyse_Filter analyse(ecg);
		analyse.Set_Filter_Mode(filtbuff.combo_filt_type.GetSelIndex() +IDS_FILT_FIRST +1
										, atoi(filtbuff.edit_optval));
		analyse.Execute();
		//const_cast<Tecg*>(ecg)->AnalyseData(INVERT);
		DrawDoc->RePaintViews();
	}
}

//########################################################################
// DrawList View Stuff (text window)
//########################################################################

TDrawListView::TDrawListView(TDrawDocument& doc,TWindow *parent)
		 : TView(doc), TListBox(parent, GetNextViewId(), 0,0,0,0), DrawDoc(&doc)
{
  Attr.Style &= ~(WS_BORDER | LBS_SORT);
// not sure what this does so i am going to avoid it :-)
//  Attr.AccelTable = IDA_DRAWLISTVIEW;
// puts the original main menu back
//  SetViewMenu(new TMenuDescr(IDM_DRAWLISTVIEW,0,1,0,0,0,0));
}

BOOL TDrawListView::Create()
{
  TListBox::Create();
  LoadData();
  return TRUE;
}

void TDrawListView::LoadData()
{
  ClearList();
  const Tecg* ecg;
  if ((ecg = DrawDoc->GetECG()) != 0)
	 FormatData(ecg, 0);

  SetSelIndex(0);
}

void TDrawListView::FormatData(const Tecg* ecg,  int unsigned index)
{
	// Large number for now :-)
  char buf1[80];
  char buf2[80];
  char buf3[80];
  char buf4[80];
  char buf5[80];

//  TColor color(line->QueryColor());
  wsprintf(buf1, "No. Of Points in ECG = (%d)",
			  ecg->length());
  wsprintf(buf2, "(x,y) Scales         = (%d,%d)",
			  ecg->get_x_scale(), ecg->get_y_scale());
  wsprintf(buf3, "R Hight threshold    = (%d)",
			  ecg->get_r_thresh());
  wsprintf(buf4, "R Change threshold   = (%d)",
			  ecg->get_r_change());
  wsprintf(buf5, "Y (min,max)          = (%d,%d)",
			  ecg->get_y_min(), ecg->get_y_max());
  wsprintf(buf5, "Y mean               = (%d)",
			  ecg->get_y_mean());

//  DeleteString(index);
  ClearList();
  AddString(ecg->ECG_Data_String1.c_str());
  AddString(ecg->ECG_Data_String2.c_str());
  AddString(ecg->ECG_Data_String3.c_str());
  AddString(buf1);
  AddString(buf2);
  AddString(buf3);
  AddString(buf4);
  AddString(buf5);

  SetSelIndex(index);
}

BOOL TDrawListView::VnCommit(BOOL /*force*/)
{
  return TRUE;
}

BOOL TDrawListView::VnRevert(BOOL /*clear*/)
{
  LoadData();
  return TRUE;
}

BOOL TDrawListView::VnRePaint(long /*area*/ )
{
	const Tecg* ecg = DrawDoc->GetECG();
	FormatData(ecg, 0);
	return TRUE;
}




