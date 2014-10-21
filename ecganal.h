#ifndef __ECGANAL_H
#define __ECGANAL_H
//////////////////////////////////////////////////////////////
//
// ECG Analysis Classes
//
//////////////////////////////////////////////////////////////
#include "ecgclass.h"
#include "ecgprint.rc"
#include <math.h>



// Base Class
class TECGAnalyse {
public:
	TECGAnalyse(const Tecg *ecg){ecg_ptr = ecg; }
	virtual void Execute() = 0;

protected:
	// functions executed in this class
	Tecg *get_ecg() {return (Tecg*)ecg_ptr; }
	int length() {return ecg_ptr->length(); }
	int step();
	int get_z_level_value(int point) const;
	BOOL return_range(int from, int *star, int *finish);

	// functions that ask the ecg class to set data elements
	void set_analysed(BOOL opt);
	void set_good_bad_checked(BOOL opt);
	void set_y_min(int val);
	void set_y_max(int val);
	void set_y_mean(long val);
	void set_r_thresh_real(long r);
	void set_gb_sects(TSectGoodBad sects);

	// functions that ask the ecg class for data
	BOOL IsAnalysed() const {return ecg_ptr->IsAnalysed(); }
	BOOL get_zvanal_on() {return ecg_ptr->get_zvanal_on(); }
	TMinIs get_min_for() const {return ecg_ptr->get_min_for(); }
	TMaxIs get_max_for() const {return ecg_ptr->get_max_for(); }
	long get_r_thresh() {return ecg_ptr->get_r_thresh(); }
	int get_r_change() {return ecg_ptr->get_r_change(); }
	TSectGoodBad get_gb_sects() {return ecg_ptr->get_good_bad_sects(); }
	TRunMeanCalc get_run_mean_calc() const {return ecg_ptr->get_run_mean_calc(); }
	int get_zero_level() const {return ecg_ptr->get_zero_level(); }
	int get_y_max() const {return ecg_ptr->get_y_max(); }
	int get_y_min() const {return ecg_ptr->get_y_min(); }
	long get_y_mean() const {return ecg_ptr->get_y_mean(); }

private:
	const Tecg *ecg_ptr;
};

/////////////////////////////////////////////////////////////
// Clear (base) class
/////////////////////////////////////////////////////////////
class TAnalyse_Clear : public TECGAnalyse {
public:
	TAnalyse_Clear(const Tecg *ecg) : TECGAnalyse(ecg){};
};

/////////////////////////////////////////////////////////////
// Clear analysis  class
/////////////////////////////////////////////////////////////
class TAnalyse_Clear_Ann : public TAnalyse_Clear {
public:
	TAnalyse_Clear_Ann(const Tecg *ecg) : TAnalyse_Clear(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Clear values  class
/////////////////////////////////////////////////////////////
class TAnalyse_Clear_Val : public TAnalyse_Clear {
public:
	TAnalyse_Clear_Val(const Tecg *ecg) : TAnalyse_Clear(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Clear all  class
/////////////////////////////////////////////////////////////
class TAnalyse_Clear_All : public TAnalyse_Clear {
public:
	TAnalyse_Clear_All(const Tecg *ecg) : TAnalyse_Clear(ecg){};

	void Execute();
};


/////////////////////////////////////////////////////////////
// R_Thresh analysis class
/////////////////////////////////////////////////////////////
class TAnalyse_R_Thresh : public TECGAnalyse {
public:
	TAnalyse_R_Thresh(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// R_Change analysis class
/////////////////////////////////////////////////////////////
class TAnalyse_R_Change : public TECGAnalyse {
public:
	TAnalyse_R_Change(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// S Wave calculation analysis class (1)
/////////////////////////////////////////////////////////////
class TAnalyse_S_Calc_1 : public TECGAnalyse {
public:
	TAnalyse_S_Calc_1(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// S Wave calculation analysis class (2)
/////////////////////////////////////////////////////////////
class TAnalyse_S_Calc_2 : public TECGAnalyse {
public:
	TAnalyse_S_Calc_2(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Q Wave calculation analysis class (1)
/////////////////////////////////////////////////////////////
class TAnalyse_Q_Calc_1 : public TECGAnalyse {
public:
	TAnalyse_Q_Calc_1(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Q Wave calculation analysis class (2)
/////////////////////////////////////////////////////////////
class TAnalyse_Q_Calc_2 : public TECGAnalyse {
public:
	TAnalyse_Q_Calc_2(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Q &| S Wave Minimum calculation analysis class (1)
/////////////////////////////////////////////////////////////
class TAnalyse_Min_Calc_1 : public TECGAnalyse {
public:
	TAnalyse_Min_Calc_1(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// P &| T Wave Maximum calculation analysis class (1)
/////////////////////////////////////////////////////////////
class TAnalyse_Max_Calc_1 : public TECGAnalyse {
public:
	TAnalyse_Max_Calc_1(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Good or Bad Wave calculation analysis class
/////////////////////////////////////////////////////////////
class TAnalyse_Good_Or_Bad : public TECGAnalyse {
public:
	TAnalyse_Good_Or_Bad(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Running Mean calculation analysis class
/////////////////////////////////////////////////////////////
class TAnalyse_Run_Mean : public TECGAnalyse {
public:
	TAnalyse_Run_Mean(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Running Mean calculation analysis class
/////////////////////////////////////////////////////////////
class TAnalyse_Invert : public TECGAnalyse {
public:
	TAnalyse_Invert(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Execute();
};

/////////////////////////////////////////////////////////////
// Filter calculation analysis class
/////////////////////////////////////////////////////////////
class TAnalyse_Filter : public TECGAnalyse {
public:
	TAnalyse_Filter(const Tecg *ecg) : TECGAnalyse(ecg){};

	void Set_Filter_Mode(int mode, int o_val=0) {filt_mode = mode; opt_val = o_val; }
	void Execute();
private:
	void Run_Hanning_Filter();
	void Run_Least_Square_Filter();
	void Run_IIR_Low_Pass_Filter();
	void Run_Adapt_Filter();
	void Run_Two_Pole_Filter();
	void Run_Integer_filter();
	void Run_QRSDet_Low_Pass_Filter();
	void Run_QRSDet_High_Pass_Filter();
	void Run_QRSDet_Derivative_Filter();
	void Run_QRSDet_Square_Filter();
	void Run_QRSDet_Moveing_Integer_Filter();
	int filt_mode;
	int opt_val;
};


#endif // ECGANAL_H