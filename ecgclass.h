#ifndef ECG_CLASS_HEADER
#define ECG_CLASS_HEADER

#include <owl\point.h>
#include "array.h"
#include "ecgprint.rc"
#include <owl\dc.h>
#include <owl\docview.h>
#include <limits.h>

enum TWave { UNDEF, PWAV, QWAV, RWAV, SWAV, TWAV };
enum TAnalyse { CLEAR, R_THRESH, R_CHANGE, S_CALC_1, S_CALC_2, Q_CALC_1,
						Q_CALC_2, MIN_CALC_1, MAX_CALC_1, GOOD_OR_BAD_1,
						CALC_RUN_MEAN_1, INVERT };
enum TCol { TCOL_MIN, COLPWAV, COLQWAV, COLRWAV, COLSWAV, COLTWAV, COLUNDEF,
				COLZERO, COLMEAN, COLRUNMEAN, COLRTHRESH, COLGOOD, COLBAD,
				COLSTARTEND, TCOL_MAX };
enum TMinIs { MINIS_QWAV, MINIS_SWAV, MINIS_AUTO, MINIS_BOTH };
enum TMaxIs { MAXIS_PWAV, MAXIS_TWAV, MAXIS_AUTO, MAXIS_BOTH };
enum TSect { SECT_GOOD, SECT_BAD, SECT_STARTEND, TSECT_END_OF_ARRAY };
enum TLevelIs { LEVELIS_ZERO, LEVELIS_MEAN, LEVELIS_RUNMEAN };
enum TRunMeanCalc { TRMC_FULL, TRMC_RTR, TRMC_RTRWIN };
//typedef struct{
//	TPoint ecgcoord;
//	int stat;
//} ECGPoint;

typedef struct {
	TSect	status;
	int start;
	int finish;
} TGoodBad;

typedef struct {
	int Mode;
	int Opt_Val;
} TFData;


class ECGPoint {
public:
	ECGPoint(int x=0, int y=0, TWave s=UNDEF, long rm=-1)
						{x_cord=x; x_orig=x ;y_cord=y; y_orig=y; stat=s;
							running_mean=rm; }
	TPoint GetPoints() const {return TPoint(x_cord,y_cord); }
	TPoint GetOrigPoints() const {return TPoint(x_orig,y_orig); }
	TWave GetStat() const {return stat; }
	long GetRunMean() const {return running_mean; }
	void SetStat(TWave s) {stat=s; }
	void SetRunMean(long rm) {running_mean=rm; }
	void Set_y(int val) { y_cord = val; }
	void ClearAnalysis() { stat = UNDEF ; running_mean = -1; }
	void ClearValues() { x_cord=x_orig ; y_cord=y_orig; }
	void ClearAll() { ClearAnalysis(); ClearValues(); }

	BOOL operator ==(const ECGPoint& other) const { return &other == this; }

private:
	int x_orig, x_cord; // Orig stores perminent copy of what was loaded
	int y_orig; // cord is the altered version (for analysis)
	int y_cord;
	TWave stat;
	long running_mean;
};

typedef Array<ECGPoint> TPoints;
typedef Array<TColor> TColors;
typedef Array<int> TPenSizes;
typedef Array<TGoodBad> TSectGoodBad;
typedef Array<TWave> TWaves;

class Tecg : public TPoints {
	public:
		// Constructor no arguments needed (yet)
		Tecg();

		// ECGTrace Draws itself. Returns TRUE if everything fine.
		virtual BOOL Paint(TDC &) const;

		void set_x_scale(int x_val) {x_scale = x_val; }
		void set_y_scale(int y_val) {y_scale = y_val; }
		void set_r_thresh(long r) {r_thresh = (10-r)*y_scale + y_max; }
		void set_r_thresh_real(long r) {r_thresh = r; }
		void set_r_change(int c) {r_change = c; }
		void set_y_min(int val) {y_min = val; }
		void set_y_max(int val) {y_max = val; }
		void set_y_mean(long val) {y_mean = val; }
		void set_zero_level(int level) {zero_level = level; }
		void set_analysed(BOOL an) {data_analysed = an; }
		void set_color(TCol wcol, TColor color) {ECGColors[wcol]=color; }
		void set_colors(TColors& cols) {ECGColors=cols; }
		void set_cust_colors(TColors& cols) {ECGCustColors=cols; }
		void set_pen_sizes(TPenSizes& siz) {ECGPenSizes=siz; }
		void set_zvanal_on(BOOL option) {z_evaluation_on = option; }
		void set_which_level(TLevelIs option) {use_which_level = option; }
		void set_one_in_n(int val) {one_in_n = val; }
		void set_one_in_n_display(BOOL opt) {one_in_n_disp = opt; }
		void set_one_in_n_analysis(BOOL opt) {one_in_n_ann = opt; }
		void set_min_for(TMinIs option) {min_for = option; }
		void set_max_for(TMaxIs option) {max_for = option; }
		void set_good_bad_checked(BOOL val) {good_bad_checked = val; }
		void set_run_mean_calc(TRunMeanCalc opt) {rm_calc = opt; }
		void set_run_mean_winsize(int opt) {rmc_winsize = opt; }
		void set_good_bad_sects(TSectGoodBad sects); // Can't be inline
		void set_cur_filt_data(TFData fdat) {Cur_Filt_Data = fdat; }
		void set_cur_filt_data(int mode, int opt_val=0)
						{Cur_Filt_Data.Mode = mode; Cur_Filt_Data.Opt_Val = opt_val; }
		BOOL IsAnalysed() const {return data_analysed; }
		int get_x_scale() const {return x_scale; }
		int get_y_scale() const {return y_scale; }
		long get_r_thresh() const {return r_thresh; }
		int get_r_change() const {return r_change; }
		int get_y_max() const {return y_max; }
		int get_y_min() const {return y_min; }
		long get_y_mean() const {return y_mean; }
		int get_zero_level() const {return zero_level; }
		BOOL get_zvanal_on() const {return z_evaluation_on; }
		TLevelIs get_which_level() const {return use_which_level; }
		int get_one_in_n() const {return one_in_n; }
		BOOL get_one_in_n_display() const {return one_in_n_disp; }
		BOOL get_one_in_n_analysis() const {return one_in_n_ann; }
		TColor get_color(TCol wcol) const {return ECGColors[wcol]; }
		TColors get_colors() const {return ECGColors; }
		TColors get_cust_colors() const {return ECGCustColors; }
		TPenSizes get_pen_sizes() const {return ECGPenSizes; }
		TMinIs get_min_for() const {return min_for; }
		TMaxIs get_max_for() const {return max_for; }
		BOOL get_good_bad_checked() const {return good_bad_checked; }
		TRunMeanCalc get_run_mean_calc() const {return rm_calc; }
		int get_run_mean_winsize() const {return rmc_winsize; }
		TSectGoodBad get_good_bad_sects() const {return good_bad_sects; }
		TFData get_cur_filt_data() const {return Cur_Filt_Data; }

		// The == operator must be defined for the container class, even if
		// unused (has potential for trace matching though)
		BOOL operator==(const Tecg& other) const { return &other == this; }
		friend ostream& operator <<(ostream& os, const Tecg& ecg);
		friend istream& operator >>(istream& is, Tecg& ecg);
		// ********** This should be protected, but I have not yet written
		// the return string and its length functions yet.
		string ECG_Data_String1;
		string ECG_Data_String2;
		string ECG_Data_String3;

	private:

	protected:
		int x_scale;
		int y_scale;
		long r_thresh;
		int r_change;
		int y_max;
		int y_min;
		long y_mean;
		int zero_level;
		int one_in_n;
		BOOL one_in_n_disp, one_in_n_ann;
		BOOL z_evaluation_on;
		TLevelIs use_which_level;
		TMinIs min_for;
		TMaxIs max_for;
		BOOL data_analysed;
		TColors ECGColors;
		TColors ECGCustColors;
		TPenSizes ECGPenSizes;
		BOOL good_bad_checked;
		TSectGoodBad good_bad_sects;
		TRunMeanCalc  rm_calc;
		int rmc_winsize;
		TFData Cur_Filt_Data;
};



#endif ECG_CLASS_HEADER