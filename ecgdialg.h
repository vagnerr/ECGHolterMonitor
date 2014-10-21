//////////////////////////////////////////////////////
// Dialog classes (extra ones) for ecg application
//////////////////////////////////////////////////////

#ifndef __ECGDIALG_H
#define __ECGDIALG_H

#include <owl\dialog.h>
#include <owl\chooseco.h>
#include <owl\dc.h>
#include <owl\radiobut.h>
#include <owl\combobox.h>
#include <owl\edit.h>

#include "ecgprint.rc"
#include "ecgclass.h"




// Colour dialog class
class TDlgColour : public TDialog
{
public:
	TDlgColour(TWindow *parent, TResId resId, TColors *ecg_cols,
					TColors *cust_cols);

protected:
	// Button press responses
	void CmColPWav();
	void CmColQWav();
	void CmColRWav();
	void CmColSWav();
	void CmColTWav();
	void CmColUndef();

	void CmColZero();
	void CmColMean();
	void CmColRunMean();
	void CmColRThresh();

	void CmColGood();
	void CmColBad();
	void CmColStartEnd();

	void CallComDialog(TCol ColDlg);
	TColors *customColors;
  	TColors *temp_ecg_cols;

	DECLARE_RESPONSE_TABLE(TDlgColour);

};

//////////////////////////
// pensizes Dialog class
//////////////////////////

// Transfer buffer for dialog box
struct TBufPenSizes
{
	char		edit_p_thick[3];		// edit box for p thickness
	char		edit_q_thick[3];		// edit box for q thickness
	char		edit_r_thick[3];		// edit box for r thickness
	char		edit_s_thick[3];		// edit box for s thickness
	char		edit_t_thick[3];		// edit box for t thickness

	char		edit_undef_thick[3];		// udefined thickness
	char		edit_zero_thick[3];		// zero volt line
	char		edit_mean_thick[3];		// mean volt line
	char		edit_run_mean_thick[3];		// mean volt line

	char		edit_rthresh_thick[3];		// t threshold line
};

class TDlgPenSizes : public TDialog
{
public:
	TDlgPenSizes(TWindow *parent, TResId resId, TBufPenSizes *pensizes);

protected:

};


//////////////////////////
// options Dialog class
//////////////////////////

// Transfer buffer for dialog box
struct TBufOptions
{
	BOOL		zero_volt;	// check box for switching on zero volt analysis
	BOOL     zero_level;	// radio button for using zero level analysys
	BOOL		mean_level;	// radio button for using mean level analysis
	BOOL		run_mean_level;

	char		edit_xscale[4];		// edit box for x-scale
	char		edit_yscale[4];		// edit box for y scale

	char		edit_rchange[4];		// edit box for r-change threshold
	char		edit_rvolts[6];		// edit box for r-volts threshold
	char		edit_zvolts[6];		// edit box for manualy setting the zero level

	char		edit_points[4];		// one point in N
	BOOL		points_display;		// use 1 in N for display
	BOOL		points_analysis;		// use 1 in N for analysis

	BOOL		min_for_qwav;			// which wave to use inter Rwave minimum
	BOOL		min_for_swav;			// for
	BOOL		min_for_auto;			//
	BOOL		min_for_both;			//

	BOOL		max_for_qwav;			// which wave to use inter Rwave maximum
	BOOL		max_for_swav;			// for
	BOOL		max_for_auto;			//
	BOOL		max_for_both;			//
};

class TDlgOptions : public TDialog
{
public:
	TDlgOptions(TWindow *parent, TResId resId, TBufOptions *options);

protected:

};

//////////////////////////
// Running Mean Dialog class
//////////////////////////

// Transfer buffer for dialog box
struct TBufRunMean
{
	BOOL		full;			// radio button for full running mean
	BOOL		rtor;			// radio button for r-wave to r-wave
	BOOL		rtorwin;		// radio button for r to r windowed

	char		edit_winsize[3];	// size of r to r window
};

class TDlgRunMean : public TDialog
{
public:
	TDlgRunMean(TWindow *parent, TResId resId, TBufRunMean *rmbuff);

protected:

};

//////////////////////////
// Filter Dialog class
//////////////////////////

// Transfer buffer for dialog box
struct TBufFilter
{
	TComboBoxData	combo_filt_type;
	char				edit_optval[4]; // Optional argument value
};

class TDlgFilter : public TDialog
{
public:
	TDlgFilter(TWindow *parent, TResId resId, TBufFilter *filtbuff);

protected:

};

#endif // __ECGDIALG_H