#include "ecgclass.h"

Tecg::Tecg() : TPoints()
{
	x_scale = 5;
	y_scale = 5;
	r_thresh = -1;
	r_change = 10;
	zero_level = 2048;
	z_evaluation_on = FALSE;
	use_which_level = LEVELIS_ZERO;
	one_in_n = 1;
	one_in_n_disp = FALSE;
	one_in_n_ann = FALSE;
	data_analysed = FALSE;
	good_bad_checked = FALSE;

	set_min_for(MINIS_BOTH);
	set_max_for(MAXIS_BOTH);
	rm_calc = TRMC_FULL;
	rmc_winsize = 1;

	ECGCustColors[0] = 0xC0DCC0L;		// these three are also the colours
	ECGCustColors[1] = 0x8080FFL;		// for GOOD, BAD & STARTEND
	ECGCustColors[2] = 0x89EFF1L;		//
	ECGCustColors[3] = 0x303030L;
	ECGCustColors[4] = 0x404040L;
	ECGCustColors[5] = 0x505050L;
	ECGCustColors[6] = 0x606060L;
	ECGCustColors[7] = 0x707070L;
	ECGCustColors[8] = 0x808080L;
	ECGCustColors[9] = 0x909090L;
	ECGCustColors[10] = 0xA0A0A0L;
	ECGCustColors[11] = 0xB0B0B0L;
	ECGCustColors[12] = 0xC0C0C0L;
	ECGCustColors[13] = 0xD0D0D0L;
	ECGCustColors[14] = 0xE0E0E0L;
	ECGCustColors[15] = 0xF0F0F0L;


	// set default colors
	ECGColors[COLPWAV] = TColor::LtCyan;
	ECGColors[COLQWAV] = TColor::Gray;
	ECGColors[COLRWAV] = TColor::LtRed;
	ECGColors[COLSWAV] = TColor::LtYellow;
	ECGColors[COLSWAV] = TColor::LtYellow;
	ECGColors[COLTWAV] = TColor::LtMagenta;
	ECGColors[COLUNDEF] = TColor::Black;
	ECGColors[COLZERO] = TColor::LtGreen;
	ECGColors[COLMEAN] = TColor::LtBlue;
	ECGColors[COLRUNMEAN] = TColor::LtCyan;
	ECGColors[COLRTHRESH] = TColor::Black;

	ECGColors[COLGOOD] 	  = 0xC0DCC0L;	//was TColor::LtGreen;
	ECGColors[COLBAD] 	  = 0x8080FFL;	//was TColor::LtRed;
	ECGColors[COLSTARTEND] = 0x89EFF1L;	//was TColor::LtYellow;

	ECGPenSizes[COLPWAV] = 1;
	ECGPenSizes[COLQWAV] = 1;
	ECGPenSizes[COLRWAV] = 1;
	ECGPenSizes[COLSWAV] = 1;
	ECGPenSizes[COLSWAV] = 1;
	ECGPenSizes[COLTWAV] = 1;
	ECGPenSizes[COLUNDEF] = 1;
	ECGPenSizes[COLZERO] = 1;
	ECGPenSizes[COLMEAN] = 1;
	ECGPenSizes[COLRUNMEAN] = 1;
	ECGPenSizes[COLRTHRESH] = 1;

	// Filter defaults
	Cur_Filt_Data.Mode = IDS_FILT_FIRST+1;
	Cur_Filt_Data.Opt_Val = 2;
}



BOOL Tecg::Paint(TDC &dc) const
{
//	char debugstring[255];
	//OutputDebugString("Painting ECG\n");
	// Set pen for the dc to the values for this ecg (well just use the
	// default color for now :-) )
	// TPen pen(Colour, PenSize);
	// dc.SelectObject(pen);
	TPen Upen(ECGColors[COLUNDEF], ECGPenSizes[COLUNDEF]);  // Colour pen for Undefined
	TPen Ppen(ECGColors[COLPWAV], ECGPenSizes[COLPWAV]);		// Colour pen for P-Wave
	TPen Qpen(ECGColors[COLQWAV], ECGPenSizes[COLQWAV]);		// Colour pen for Q-Wave
	TPen Rpen(ECGColors[COLRWAV], ECGPenSizes[COLRWAV]);		// Colour pen for R-Wave
	TPen Spen(ECGColors[COLSWAV], ECGPenSizes[COLSWAV]);		// Colour pen for S-Wave
	TPen Tpen(ECGColors[COLTWAV], ECGPenSizes[COLTWAV]);		// Colour pen for T-Wave
	TPen Zpen(ECGColors[COLZERO], ECGPenSizes[COLZERO]);	// Colour pen for zero-volt line
	TPen Mpen(ECGColors[COLMEAN], ECGPenSizes[COLMEAN]);	// Colour pen for mean average line
	TPen RMpen(ECGColors[COLRUNMEAN], ECGPenSizes[COLRUNMEAN]);	// Colour pen for mean average line
	TPen RTpen(ECGColors[COLRTHRESH], ECGPenSizes[COLRTHRESH]); // Colour pen for R-threshhold line
	TPen Gpen(ECGColors[COLGOOD], 1); // Colour pen for Good wave
	TBrush Gbrush(ECGColors[COLGOOD]); // Colour brush for Good wave
	TPen Bpen(ECGColors[COLBAD], 1); // Colour pen for Bad wave
	TBrush Bbrush(ECGColors[COLBAD]); // Colour brush for Bad wave
	TPen SEpen(ECGColors[COLSTARTEND], 1); // Colour pen for start/end wave
	TBrush SEbrush(ECGColors[COLSTARTEND]); // Colour brush for start/end wave


	// Iterates through the points in the ECG
	int j = 0;
	int step = 1;
	BOOL first = TRUE;
	TWave lasttype = UNDEF;
	if (get_one_in_n_display())
		step = get_one_in_n();

	if (get_good_bad_checked())
	{
		int counter = 0;
		while (good_bad_sects[counter].status != TSECT_END_OF_ARRAY)
		{
			switch (good_bad_sects[counter].status)
			{
				case SECT_GOOD:
				{
					dc.SelectObject(Gpen);
					dc.FillRect(good_bad_sects[counter].start/get_x_scale()+10,
								0,
								good_bad_sects[counter].finish/get_x_scale()+10,
								(get_y_max() - get_y_min())/get_y_scale() + 20,
								Gbrush);
					break;
				}
				case SECT_BAD:
				{
					dc.SelectObject(Bpen);
					dc.FillRect(good_bad_sects[counter].start/get_x_scale()+10,
								0,
								good_bad_sects[counter].finish/get_x_scale()+10,
								(get_y_max() - get_y_min())/get_y_scale() + 20,
								Bbrush);
					break;
				}
				case SECT_STARTEND:
				{
					dc.SelectObject(SEpen);
					dc.FillRect(good_bad_sects[counter].start/get_x_scale()+10,
								0,
								good_bad_sects[counter].finish/get_x_scale()+10,
								(get_y_max() - get_y_min())/get_y_scale() + 20,
								SEbrush);
					break;
				}
				default:
							break;
			}
			counter++;
		}
	}

	// draw zero volts line
	dc.SelectObject(Zpen);
	dc.MoveTo(0,10 + (get_y_max() - get_zero_level())/get_y_scale());
	dc.LineTo(length()/get_x_scale() + 21,10 + (get_y_max() - get_zero_level())/get_y_scale());

	// draw mean average line
	dc.SelectObject(Mpen);
	dc.MoveTo(0,10 + (get_y_max() - (int)get_y_mean())/get_y_scale());
	dc.LineTo(length()/get_x_scale() + 21,10 + (get_y_max() - (int)get_y_mean())/get_y_scale());


	dc.SelectObject(RTpen);
	if (get_r_thresh() != -1)		// draw the R-Value threshhold line
	{
		dc.MoveTo(0,10 + (get_y_max() - (int)get_r_thresh())/get_y_scale());
		dc.LineTo(length()/get_x_scale() + 21,10 + (get_y_max() - (int)get_r_thresh())/get_y_scale());
	}

	if ((*this)[0].GetRunMean() != -1)
	{
		dc.SelectObject(RMpen);
		dc.MoveTo(10,10 + (get_y_max() -(int)((*this)[0].GetRunMean()))/get_y_scale());
		for (int i=step ; i < length() ; i += step)
		{
			dc.LineTo((*this)[i].GetPoints().x/get_x_scale()+10,10 + (get_y_max() -(int)((*this)[i].GetRunMean()))/get_y_scale());
		}
	}

	dc.SelectObject(Upen);			// Default (if not analysed)

	if (IsAnalysed())
	{
		while (j < length()) {
			ECGPoint p = (*this)[j];
			j += step;
			if (p.GetStat() != lasttype)
			{
				switch (p.GetStat())
				{
					case PWAV:
						dc.SelectObject(Ppen);
						break;
					case QWAV:
						dc.SelectObject(Qpen);
						break;
					case RWAV:
						dc.SelectObject(Rpen);
						break;
					case SWAV:
						dc.SelectObject(Spen);
						break;
					case TWAV:
						dc.SelectObject(Tpen);
						break;
					default:
						dc.SelectObject(Upen);
						break;
				}
				lasttype = p.GetStat();
			}
			if (!first)
			{
				dc.LineTo(p.GetPoints().x/get_x_scale()+10,10 + (get_y_max() -p.GetPoints().y)/get_y_scale());
			}else
			{
				dc.MoveTo(p.GetPoints().x/get_x_scale()+10,10 + (get_y_max() -p.GetPoints().y)/get_y_scale());
				first = FALSE;

			}
		}
	}
	else
	{
		dc.SelectObject(Upen);			// Default (if not analysed)
		while (j < length()) {
			ECGPoint p = (*this)[j];
			j += step;
			if (!first){
				dc.LineTo(p.GetPoints().x/get_x_scale()+10,10 + (get_y_max() -p.GetPoints().y)/get_y_scale());
			}else {
				dc.MoveTo(p.GetPoints().x/get_x_scale()+10,10 + (get_y_max() -p.GetPoints().y)/get_y_scale());
				first = FALSE;

			}
		}
	}
	dc.RestorePen();
	return TRUE;
}

void Tecg::set_good_bad_sects(TSectGoodBad sects)
{
	// class by value arguments cannot be expanded inline.
	good_bad_sects = sects;
}

// This is used for save file (Probably dont realy want it)
ostream& operator <<(ostream& os, const Tecg& ecg)
{
  // Write the number of points in the line // *******na
//  os << ecg.GetItemsInContainer();

  // Get and write pen attributes.  //************na
//  os << ' ' << line.Color << ' ' << line.PenSize;

  // Get an iterator for the array of points
  int j=0;

  // While the iterator is valid (i.e. we haven't run out of points)
  while(j <= ecg.length())
	 // Write the point from the iterator and increment the array.
//	 os << j++;
  os << '\n';
  j++;

  // return the stream object
  return os;
}

// This is used for load file can use this to convert the file into the
// co-ordinates
istream& operator >>(istream& is, Tecg& ecg)
{
//	OutputDebugString("Streaming from file\n");

	int xcord = 1;
	int ycord = 100;
	int y_min, y_max;
	long y_total;
	string temps;
	// Read in the first four lines of the data file that contain ordinary
	// Text and store it in the ECG class.
	ecg.ECG_Data_String1.read_line(is);
	ecg.ECG_Data_String2.read_line(is);
	ecg.ECG_Data_String3.read_line(is);
	is >> ycord;
	ECGPoint ecgpnt(xcord,ycord);
	ecg[xcord-1]=ecgpnt;
	xcord++;
	y_min = ycord;
	y_max = ycord;
	y_total = ycord;
	while (is.peek()!=EOF){
		is >> ycord;
		ECGPoint ecgpnt(xcord,ycord);
		ecg[xcord-1]=ecgpnt;
		if (ycord > y_max)
			y_max = ycord;
		else if (ycord < y_min)
			y_min = ycord;
		xcord++;
		y_total += ycord;
	}
	ecg.set_y_min(y_min);
	ecg.set_y_max(y_max);
	ecg.set_y_mean((y_total/(xcord-1))); 
  // return the stream object
	return is;
}




