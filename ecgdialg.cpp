////////////////////////////////////////////////////////////
// Dialog Classes for ECG Display Program
////////////////////////////////////////////////////////////

#include "ecgdialg.h"

// Response table for Colour Dialog Buttons
DEFINE_RESPONSE_TABLE1(TDlgColour, TDialog)
	EV_COMMAND(IDC_PB_PWAVE, CmColPWav),
	EV_COMMAND(IDC_PB_QWAVE, CmColQWav),
	EV_COMMAND(IDC_PB_RWAVE, CmColRWav),
	EV_COMMAND(IDC_PB_SWAVE, CmColSWav),
	EV_COMMAND(IDC_PB_TWAVE, CmColTWav),
	EV_COMMAND(IDC_PB_UNDEF, CmColUndef),

	EV_COMMAND(IDC_PB_ZVOLT, CmColZero),
	EV_COMMAND(IDC_PB_MEAN, CmColMean),
	EV_COMMAND(IDC_PB_RUN_MEAN, CmColRunMean),
	EV_COMMAND(IDC_PB_RTHRESH, CmColRThresh),

	EV_COMMAND(IDC_PB_GOOD, CmColGood),
	EV_COMMAND(IDC_PB_BAD, CmColBad),
	EV_COMMAND(IDC_PB_STARTEND, CmColStartEnd),


END_RESPONSE_TABLE;

TDlgColour::TDlgColour(TWindow *parent, TResId resId, TColors *ecg_cols,
								TColors *cust_cols) : TDialog(parent, resId)
{
	// Copy the Color dialog's starting custom colors.
/*	TColors cColors = *cust_cols;
	for (int i=0; i<16; i++)
	{
		customColors[i] = cColors[i];
	}
*/
	customColors = cust_cols;
	temp_ecg_cols = ecg_cols;

}

void TDlgColour::CmColPWav()
{
	CallComDialog(COLPWAV);
}

void TDlgColour::CmColQWav()
{
	CallComDialog(COLQWAV);
}

void TDlgColour::CmColRWav()
{
	CallComDialog(COLRWAV);
}

void TDlgColour::CmColSWav()
{
	CallComDialog(COLSWAV);
}

void TDlgColour::CmColTWav()
{
	CallComDialog(COLTWAV);
}

void TDlgColour::CmColUndef()
{
	CallComDialog(COLUNDEF);
}

void TDlgColour::CmColZero()
{
	CallComDialog(COLZERO);
}

void TDlgColour::CmColMean()
{
	CallComDialog(COLMEAN);
}

void TDlgColour::CmColRunMean()
{
	CallComDialog(COLRUNMEAN);
}

void TDlgColour::CmColRThresh()
{
	CallComDialog(COLRTHRESH);
}

void TDlgColour::CmColGood()
{
	CallComDialog(COLGOOD);
}

void TDlgColour::CmColBad()
{
	CallComDialog(COLBAD);
}

void TDlgColour::CmColStartEnd()
{
	CallComDialog(COLSTARTEND);
}


void TDlgColour::CallComDialog(TCol ColDlg)
{
	// Define storage for an error message.
	char errorMsg[81];
	TColor cCols[16];
	TColors t_ecg_cols = *temp_ecg_cols;

	TColors cColors = *customColors;
	for (int i=0; i<16; i++)
	{
		cCols[i] = cColors[i];
	}

	// Declare the Color dialog's TData object.
	TChooseColorDialog::TData colorData;


	// Set the flags so that the Color dialog box fully
	// opens and so that the color choice displays black.
	colorData.Flags =  CC_RGBINIT;

	// Set the initial color choice.
	colorData.Color = t_ecg_cols[ColDlg];

	// Set the custom colors display.
	colorData.CustColors = cCols;

	// Create the Color dialog box.
	TChooseColorDialog *dialog =
	  new TChooseColorDialog(this, colorData);

	// Execute the Color dialog box.
	int result = dialog->Execute();

	// Respond to the dialog box's OK button.
	if (result == IDOK)
	{
		// Retrieve the user's chosen color.
		t_ecg_cols[ColDlg]=colorData.Color;

		for (int i=0; i<16; i++)
		{
			cColors[i] = colorData.CustColors[i];
		}
		*customColors = cColors;

		// Force the window to be repainted.
		//Invalidate();
	}
	// If an error occurred...
	else if (colorData.Error != 0)
	{
		// ...display an error message box.
		wsprintf(errorMsg,
			"Error #%ld occurred.", colorData.Error);
		MessageBox(errorMsg, "ERROR",
			MB_OK | MB_ICONEXCLAMATION);
	}
	*temp_ecg_cols=t_ecg_cols;
}

/////////////////////////////////////////
// PenSize dialog box
/////////////////////////////////////////


TDlgPenSizes::TDlgPenSizes(TWindow *parent, TResId resId, TBufPenSizes *pensizes)
	: TDialog(parent, resId)
{
	// Construct OWL objects for each control
	new TEdit(this, IDC_P_THICK, sizeof(pensizes->edit_p_thick));
	new TEdit(this, IDC_Q_THICK, sizeof(pensizes->edit_q_thick));
	new TEdit(this, IDC_R_THICK, sizeof(pensizes->edit_r_thick));
	new TEdit(this, IDC_S_THICK, sizeof(pensizes->edit_s_thick));
	new TEdit(this, IDC_T_THICK, sizeof(pensizes->edit_t_thick));
	new TEdit(this, IDC_UNDEF_THICK, sizeof(pensizes->edit_undef_thick));
	new TEdit(this, IDC_ZERO_THICK, sizeof(pensizes->edit_zero_thick));
	new TEdit(this, IDC_MEAN_THICK, sizeof(pensizes->edit_mean_thick));
	new TEdit(this, IDC_RUNMEAN_THICK, sizeof(pensizes->edit_run_mean_thick));
	new TEdit(this, IDC_RTHRESH_THICK, sizeof(pensizes->edit_rthresh_thick));

	// set up the transfer buffer
	TransferBuffer = pensizes;
}

/////////////////////////////////////////
// OPtions dialog box
/////////////////////////////////////////


TDlgOptions::TDlgOptions(TWindow *parent, TResId resId, TBufOptions *options)
	: TDialog(parent, resId)
{
	// Construct OWL objects for each control
	new TCheckBox(this, IDC_OPT_CB_ZEROVAN);
	new TRadioButton(this, IDC_OPT_RB_ZLEVEL, 0);
	new TRadioButton(this, IDC_OPT_RB_MEAN, 0);
	new TRadioButton(this, IDC_OPT_RB_RUNMEAN, 0);
	new TEdit(this, IDC_OPT_X_SCALE, sizeof(options->edit_xscale));
	new TEdit(this, IDC_OPT_Y_SCALE, sizeof(options->edit_yscale));
	new TEdit(this, IDC_OPT_R_CHANGE, sizeof(options->edit_rchange));
	new TEdit(this, IDC_OPT_R_VOLTS, sizeof(options->edit_rvolts));
	new TEdit(this, IDC_OPT_Z_VOLTS, sizeof(options->edit_zvolts));
	new TEdit(this, IDC_OPT_POINTS, sizeof(options->edit_points));
	new TCheckBox(this, IDC_OPT_POINTS_DISPLAY);
	new TCheckBox(this, IDC_OPT_POINTS_ANALYSIS);
	new TRadioButton(this, IDC_OPT_MIN_QWAVE, 0);
	new TRadioButton(this, IDC_OPT_MIN_SWAVE, 0);
	new TRadioButton(this, IDC_OPT_MIN_AUTO, 0);
	new TRadioButton(this, IDC_OPT_MIN_BOTH, 0);
	new TRadioButton(this, IDC_OPT_MAX_PWAVE, 0);
	new TRadioButton(this, IDC_OPT_MAX_TWAVE, 0);
	new TRadioButton(this, IDC_OPT_MAX_AUTO, 0);
	new TRadioButton(this, IDC_OPT_MAX_BOTH, 0);


	// set up the transfer buffer
	TransferBuffer = options;
}


/////////////////////////////////////////
// Running Mean dialog box
/////////////////////////////////////////


TDlgRunMean::TDlgRunMean(TWindow *parent, TResId resId, TBufRunMean *rmbuff)
	: TDialog(parent, resId)
{
	// Construct OWL objects for each control
	new TRadioButton(this, IDC_RM_RB_FULL, 0);
	new TRadioButton(this, IDC_RM_RB_RTR, 0);
	new TRadioButton(this, IDC_RM_RB_RTRWIN, 0);

	new TEdit(this, IDC_RM_EB_WINSIZE, sizeof(rmbuff->edit_winsize));


	// set up the transfer buffer
	TransferBuffer = rmbuff;
}

/////////////////////////////////////////
// Filter dialog box
/////////////////////////////////////////


TDlgFilter::TDlgFilter(TWindow *parent, TResId resId, TBufFilter *filtbuff)
	: TDialog(parent, resId)
{
	// Construct OWL objects for each control
	new TComboBox(this, IDC_CMB_FILTMODE, 20); // dont know the last argument
   new TEdit(this, IDC_ED_FILTOPTVAL, sizeof(filtbuff->edit_optval));
	// set up the transfer buffer
	TransferBuffer = filtbuff;
}


