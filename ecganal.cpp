#include "ecganal.h"

/////////////////////////////////////////////////////////
// Analyser base class functions
/////////////////////////////////////////////////////////
int TECGAnalyse::step()
{
	if (ecg_ptr->get_one_in_n_analysis())   // analysing
		return ecg_ptr->get_one_in_n();
	else
		return 1;
}

int TECGAnalyse::get_z_level_value(int point) const
{
	int return_val;
	if (ecg_ptr->get_zvanal_on())
	{
		switch (ecg_ptr->get_which_level())
		{
			case LEVELIS_ZERO:	return_val = ecg_ptr->get_zero_level();
				break;
			case LEVELIS_MEAN:	return_val =  (int)ecg_ptr->get_y_mean();
				break;
			case LEVELIS_RUNMEAN: return_val = (int)((*ecg_ptr)[point].GetRunMean());
				break;
			default:					return_val =  0; // should never happen
				break;
		}
	}
	else
	{
		return_val = 0;
	}

	return return_val;
}

////////////////////////////////////////////////////////////
//
// TECGAnalyse::return_range()
//
// when given a starting point for an ecg will return
// start and finish values representing the maximums of
// rwaves, if it reaches the end of the ecg before it finds
// two r-waves, then it will return false
////////////////////////////////////////////////////////////
BOOL TECGAnalyse::return_range(int from, int *start, int *finish)
{
	while ((from < length()) && ((*get_ecg())[from].GetStat() != RWAV))
	{
		from += step();
	}
	// should now be at start of first rwav or at the end of the ecg
	if (from < length())
	{
		// definately found start of rwave
		int max = 0;
		while ((from < length()) && ((*get_ecg())[from].GetStat() == RWAV))
		{
			if ((*get_ecg())[from].GetPoints().y > max)
			{
				*start = from;
				max = (*get_ecg())[from].GetPoints().y;
			}
			from += step();
		}
	}
	else
	{
		*start = -1;
		*finish = -1;
		return FALSE;
	}
	// now look for second rwave
	while ((from < length()) && ((*get_ecg())[from].GetStat() != RWAV))
	{
		from += step();
	}
	// should now be at start of second rwav or at the end of the ecg
	if (from < length())
	{
		// definately found start of rwave
		int max = 0;
		while ((from < length()) && ((*get_ecg())[from].GetStat() == RWAV))
		{
			if ((*get_ecg())[from].GetPoints().y > max)
			{
				*finish = from;
				max = (*get_ecg())[from].GetPoints().y;
			}
			from += step();
		}
	}
	else
	{
		*finish = -1;  // only set finish this time
		return FALSE;
	}
	return TRUE; // If we are here then both points where found
}

////////////////////////////////////////
// following functions ask ecg class to
// set data elements (cannot be inline)
////////////////////////////////////////
void TECGAnalyse::set_analysed(BOOL opt)
{
	const_cast<Tecg*>(ecg_ptr)->set_analysed(opt);
}

void TECGAnalyse::set_good_bad_checked(BOOL opt)
{
	const_cast<Tecg*>(ecg_ptr)->set_good_bad_checked(opt);
}

void TECGAnalyse::set_y_min(int val)
{
	const_cast<Tecg*>(ecg_ptr)->set_y_min(val);
}

void TECGAnalyse::set_y_max(int val)
{
	const_cast<Tecg*>(ecg_ptr)->set_y_max(val);
}

void TECGAnalyse::set_y_mean(long val)
{
	const_cast<Tecg*>(ecg_ptr)->set_y_mean(val);
}

void TECGAnalyse::set_r_thresh_real(long r)
{
	const_cast<Tecg*>(ecg_ptr)->set_r_thresh_real(r);
}

void TECGAnalyse::set_gb_sects(TSectGoodBad sects)
{
	const_cast<Tecg*>(ecg_ptr)->set_good_bad_sects(sects);
}

/////////////////////////////////////////////////////////
// Clear class functions
/////////////////////////////////////////////////////////

void TAnalyse_Clear_Ann::Execute()
{
	int j=0;
	while (j < length())
		(*get_ecg())[j++].ClearAnalysis(); // dont use step, as we want it clear
	set_analysed(FALSE); //?
	set_good_bad_checked(FALSE);
}

void TAnalyse_Clear_Val::Execute()
{
	int j=0;
	while (j < length())
		(*get_ecg())[j++].ClearValues(); // dont use step, as we want it clear
//	set_analysed(FALSE); //?  may need another boolean and new command enablers
//	set_good_bad_checked(FALSE);
}

void TAnalyse_Clear_All::Execute()
{
	int j=0;
	while (j < length())
		(*get_ecg())[j++].ClearAll(); // dont use step, as we want it clear
	set_analysed(FALSE); //?
	set_good_bad_checked(FALSE);
}


/////////////////////////////////////////////////////////
// R-Thresh analysis class functions
/////////////////////////////////////////////////////////

void TAnalyse_R_Thresh::Execute()
{
	int j=0;
	ECGPoint p = (*get_ecg())[j];
	if (p.GetPoints().y > get_r_thresh())
		p.SetStat(RWAV);
	j += step();
	while (j < length())
	{
		ECGPoint p = (*get_ecg())[j];
		if ((p.GetPoints().y > get_r_thresh()) ||
				(((*get_ecg())[j-step()].GetStat()==RWAV)
				&& (p.GetPoints().y < (*get_ecg())[j-step()].GetPoints().y)
				&& (p.GetPoints().y > get_z_level_value(j)) ) )
		{
			p.SetStat(RWAV);
			// If previous point was set as UNDEF the we have just gone over
			// The thresh-hold, so itterate backwards.
			if ((*get_ecg())[j-step()].GetStat()==UNDEF)
			{
				int ib=j;
				while ((ib > 0) &&
						  ((*get_ecg())[ib].GetPoints().y > (*get_ecg())[ib-step()].GetPoints().y )
						  && (*get_ecg())[ib-step()].GetPoints().y > get_z_level_value(ib-step()) )
				{
					ib -= step();
					ECGPoint *bp = new ECGPoint((*get_ecg())[ib]);
					bp->SetStat(RWAV);
					(*get_ecg())[ib] = *bp;
				}
			}
		}
		else
			p.SetStat(UNDEF);
		(*get_ecg())[j] = p;
		j += step();
	}
	set_analysed(TRUE);
}

/////////////////////////////////////////////////////////
// R-Change analysis class functions
/////////////////////////////////////////////////////////

void TAnalyse_R_Change::Execute()
{
	int j = step();
	while ( j < length() )
	{
		if ((((*get_ecg())[j].GetPoints().y - (*get_ecg())[j-step()].GetPoints().y)
					>= get_r_change()) && ((*get_ecg())[j].GetPoints().y > get_z_level_value(j)))
		{ // Found a large enough change
			// first iterate backwards
			int i=j;
			(*get_ecg())[i].SetStat(RWAV);
			// first iterate backwards
			while ((i>=0) &&
					( (*get_ecg())[i].GetPoints().y <= (*get_ecg())[i+step()].GetPoints().y )
						 && ((*get_ecg())[i].GetPoints().y > get_z_level_value(i)) )
			{
				(*get_ecg())[i].SetStat(RWAV);
				i -= step();
			}
			// then iterate forwards (going up)
			while ((j<length()) &&
					( (*get_ecg())[j].GetPoints().y >= (*get_ecg())[j-step()].GetPoints().y ))
			{
				(*get_ecg())[j].SetStat(RWAV);
				j += step();
			}
			// Then Back down again
			while ((j<length()) &&
					( (*get_ecg())[j].GetPoints().y <= (*get_ecg())[j-step()].GetPoints().y )
						 && ((*get_ecg())[j].GetPoints().y > get_z_level_value(j)))
			{
				(*get_ecg())[j].SetStat(RWAV);
				j += step();
			}
		} // Finished this RWave
		j += step();
	}// Ready to search for the next one
	set_analysed(TRUE);
}

/////////////////////////////////////////////////////////
// S Wave Calc analysis class functions (1)
/////////////////////////////////////////////////////////

void TAnalyse_S_Calc_1::Execute()
{
	// Find all points for S-wave following from end of each R-wave
	int j = step();
	while ( j < length() )
	{
		if ( (*get_ecg())[j].GetStat() == RWAV )
		{
			while ( ((*get_ecg())[j].GetStat() == RWAV) && (j < length()) )
			{
				j += step();
			}
			// found end of rwave, so start setting S-Wave
			(*get_ecg())[j].SetStat(SWAV);
			while ((j<length()) &&
					( (*get_ecg())[j].GetPoints().y <= (*get_ecg())[j-step()].GetPoints().y ))
			{
				(*get_ecg())[j].SetStat(SWAV);
				j +=step();
			}
			while ((j<length()) &&
					( (*get_ecg())[j].GetPoints().y >= (*get_ecg())[j-step()].GetPoints().y )
					&& ( !get_zvanal_on()
						  || ((*get_ecg())[j].GetPoints().y <= get_z_level_value(j))))
			{
				(*get_ecg())[j].SetStat(SWAV);
				j +=step();
			}
		}
	j += step();
	}
}

/////////////////////////////////////////////////////////
// S Wave Calc analysis class functions (2)
/////////////////////////////////////////////////////////

void TAnalyse_S_Calc_2::Execute()
{
	// Find all points for S-wave following from end of each R-wave
	int j = step();
	while ( j < length() )
	{
		if ( (*get_ecg())[j].GetStat() == RWAV )
		{
			while ( ((*get_ecg())[j].GetStat() == RWAV) && (j < length()) )
			{
				j += step();
			}
			// found end of rwave, so start setting S-Wave
			(*get_ecg())[j].SetStat(SWAV);
			while ((j<length()) &&
					( (*get_ecg())[j].GetPoints().y <= (*get_ecg())[j-step()].GetPoints().y ))
			{
				(*get_ecg())[j].SetStat(SWAV);
				j +=step();
			}
			while ((j<length()) &&
					(( (*get_ecg())[j].GetPoints().y >= (*get_ecg())[j-step()].GetPoints().y )
					|| ( get_zvanal_on()
						  && ((*get_ecg())[j].GetPoints().y <= get_z_level_value(j)))))
			{
				(*get_ecg())[j].SetStat(SWAV);
				j +=step();
			}
		}
	j += step();
	}
}

/////////////////////////////////////////////////////////
// Q Wave Calc analysis class functions (1)
/////////////////////////////////////////////////////////

void TAnalyse_Q_Calc_1::Execute()
{
	// Find all points for Q-wave preceeding each R-wave
	int j = step();
	while ( j < length() )
	{
		if ( (*get_ecg())[j].GetStat() == RWAV )
		{
			// found end of rwave, so start setting Q-Wave
			int i = j - step(); // set secondry pointer and drop back one
			(*get_ecg())[i].SetStat(QWAV);
			while ((i > 0) &&
					( (*get_ecg())[i].GetPoints().y <= (*get_ecg())[i+step()].GetPoints().y ))
			{
				(*get_ecg())[i].SetStat(QWAV);
				i -=step();
			}
			while ((i > 0) &&
					( (*get_ecg())[i].GetPoints().y >= (*get_ecg())[i+step()].GetPoints().y )
					&& ( !get_zvanal_on()
						  || ((*get_ecg())[i].GetPoints().y <= get_z_level_value(i))))
			{
				(*get_ecg())[i].SetStat(QWAV);
				i -=step();
			}
			// finaly zip to end of R-Wave
			while ( ((*get_ecg())[j].GetStat() == RWAV) && (j < length()) )
			{
				j += step();
			}
		}
	j += step();
	}
}

/////////////////////////////////////////////////////////
// Q Wave Calc analysis class functions (2)
/////////////////////////////////////////////////////////

void TAnalyse_Q_Calc_2::Execute()
{
	// Not yet Implemented
}

/////////////////////////////////////////////////////////
// Q &| S Wave Minimum Calc analysis class functions (1)
/////////////////////////////////////////////////////////

void TAnalyse_Min_Calc_1::Execute()
{
	// Find all points for Q or S wave preceeding each R-wave
	int j = step();
	while ( j < length() )
	{
		if ( (*get_ecg())[j].GetStat() == RWAV )
		{
			while ( ((*get_ecg())[j].GetStat() == RWAV) && (j < length()) )
			{
				j += step();
			}
			// at end of rwave, so find minimum between this and the next rwave
			int min_point = j;
			int end_of_rwav = j;
			while ( (j < length()) && ((*get_ecg())[j].GetStat() != RWAV) )
			{
				if ( (*get_ecg())[j].GetPoints().y < (*get_ecg())[min_point].GetPoints().y)
				{
					min_point = j;
				}
				j += step();
			}
			if ( j < length() ) // just incase we overran array
			{
				// set forward and back from min_point to be q/r Wave
				switch (get_min_for())
				{
					case MINIS_QWAV: (*get_ecg())[min_point].SetStat(QWAV);
						break;
					case MINIS_SWAV: (*get_ecg())[min_point].SetStat(SWAV);
						break;
					case MINIS_AUTO:
						{
							if ( ((j-end_of_rwav)/2) > (min_point-end_of_rwav) )
							{
								(*get_ecg())[min_point].SetStat(SWAV);
							}
							else
							{
								(*get_ecg())[min_point].SetStat(QWAV);
							}
						}
						break;
					case MINIS_BOTH:
						{
							if ( ((j-end_of_rwav)/2) > (min_point-end_of_rwav) )
							{
								(*get_ecg())[min_point].SetStat(SWAV);
								// now find minimum in other half
								min_point = ((j-end_of_rwav)/2+end_of_rwav);
								for (int second_half=((j-end_of_rwav)/2)+end_of_rwav;
										second_half < j; second_half += step())
								{
									if ((*get_ecg())[second_half].GetPoints().y <
											(*get_ecg())[min_point].GetPoints().y)
									{
										min_point = second_half;
									}
								}
								(*get_ecg())[min_point].SetStat(QWAV);
							}
							else
							{
								(*get_ecg())[min_point].SetStat(QWAV);
								// now find minimum in other half
								min_point = end_of_rwav;
								for (int second_half = end_of_rwav;
										second_half < ((j-end_of_rwav)/2)+end_of_rwav;
															second_half += step())
								{
									if ((*get_ecg())[second_half].GetPoints().y <
											(*get_ecg())[min_point].GetPoints().y)
									{
										min_point = second_half;
									}
								}
								(*get_ecg())[min_point].SetStat(SWAV);
							}
						}
						break;
					default:				// should not happen
						break;
				}
			}
		}
	j += step();
	}
}

/////////////////////////////////////////////////////////
// P &| T Wave Maximum Calc analysis class functions (1)
/////////////////////////////////////////////////////////

void TAnalyse_Max_Calc_1::Execute()
{
	// Find all points for P or T wave preceeding each R-wave
	int j = step();
	while ( j < length() )
	{
		if ( (*get_ecg())[j].GetStat() == RWAV )
		{
			while ( ((*get_ecg())[j].GetStat() == RWAV) && (j < length()) )
			{
				j += step();
			}
			// at end of rwave, so find minimum between this and the next rwave
			int max_point = j;
			int end_of_rwav = j;
			while ( (j < length()) && ((*get_ecg())[j].GetStat() != RWAV) )
			{
				if ( (*get_ecg())[j].GetPoints().y > (*get_ecg())[max_point].GetPoints().y)
				{
					max_point = j;
				}
				j += step();
			}
			if ( j < length() ) // just incase we overran array
			{
				// set forward and back from min_point to be q/r Wave
				switch (get_max_for())
				{
					case MAXIS_PWAV: (*get_ecg())[max_point].SetStat(PWAV);
						break;
					case MAXIS_TWAV: (*get_ecg())[max_point].SetStat(TWAV);
						break;
					case MAXIS_AUTO:
						{
							if ( ((j-end_of_rwav)/2) > (max_point-end_of_rwav) )
							{
								(*get_ecg())[max_point].SetStat(TWAV);
							}
							else
							{
								(*get_ecg())[max_point].SetStat(PWAV);
							}
						}
						break;
					case MAXIS_BOTH:
						{
							if ( ((j-end_of_rwav)/2) > (max_point-end_of_rwav) )
							{
								(*get_ecg())[max_point].SetStat(TWAV);
								// now find minimum in other half
								max_point = ((j-end_of_rwav)/2+end_of_rwav);
								for (int second_half=((j-end_of_rwav)/2)+end_of_rwav;
										second_half < j; second_half += step())
								{
									if ((*get_ecg())[second_half].GetPoints().y >
											(*get_ecg())[max_point].GetPoints().y)
									{
										max_point = second_half;
									}
								}
								(*get_ecg())[max_point].SetStat(PWAV);
							}
							else
							{
								(*get_ecg())[max_point].SetStat(PWAV);
								// now find minimum in other half
								max_point = end_of_rwav;
								for (int second_half = end_of_rwav;
										second_half < ((j-end_of_rwav)/2)+end_of_rwav;
															second_half += step())
								{
									if ((*get_ecg())[second_half].GetPoints().y >
											(*get_ecg())[max_point].GetPoints().y)
									{
										max_point = second_half;
									}
								}
								(*get_ecg())[max_point].SetStat(TWAV);
							}
						}
						break;
					default:				// should not happen
						break;
				}
			}
		}
	j += step();
	}
}

/////////////////////////////////////////////////////////
// Good Or Bad Wave Maximum Calc analysis class functions
/////////////////////////////////////////////////////////

void TAnalyse_Good_Or_Bad::Execute()
{
	TWaves order_of_waves;
	TSectGoodBad good_bad_sects = get_gb_sects();
	int order_of_counter=0;
	int sect_counter=0;
	int start_point=0;
	int present_point=0;

	// go to start of first RWAVE
	while (present_point < length() && (*get_ecg())[present_point].GetStat() != RWAV)
	{
		present_point += step();
	}

	good_bad_sects[sect_counter].start = start_point;
	good_bad_sects[sect_counter].finish = present_point;
	good_bad_sects[sect_counter].status = SECT_STARTEND;
	sect_counter++;
	order_of_waves[order_of_counter++]=RWAV;
	start_point = present_point;
	while (present_point < length())
	{
		while ( (present_point < length()) &&
					(((*get_ecg())[present_point].GetStat() == order_of_waves[order_of_counter-1])
					|| ((*get_ecg())[present_point].GetStat() == UNDEF)) )
		{
			present_point += step();
		}
		// at this point we will either be at the end of the ecg or the start
		// of a new wave
		if (present_point < length())
		{
			// we have not reached the end of the ecg so add this new wave
			// to the order of waves
			order_of_waves[order_of_counter++]=(*get_ecg())[present_point].GetStat();
			if ((*get_ecg())[present_point].GetStat()==RWAV)
			{
				// we have found the next r-wave, so we now want to analyse the
				// order
				int state = 0; // state for FSM, 0 = start, 1-4 = running states
									// 6 = end state (Good), & 7 = end state (Bad)
				for (int an_count=0 ; an_count < order_of_counter; an_count++)
				{
					switch (state)
					{
						case 0:
						{
							switch (order_of_waves[an_count])
							{
								case RWAV:	state = 1; // good, so move on a state
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 1:
						{
							switch (order_of_waves[an_count])
							{
								case RWAV:  // allowed, but do nothing (stay here)
									break;
								case SWAV:	state = 2; // good, so move on a state
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 2:
						{
							switch (order_of_waves[an_count])
							{
								case SWAV:  // allowed, but do nothing (stay here)
									break;
								case TWAV:	state = 3; // good, so move on a state
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 3:
						{
							switch (order_of_waves[an_count])
							{
								case TWAV:  // allowed, but do nothing (stay here)
									break;
								case PWAV:	state = 4; // good, so move on a state
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 4:
						{
							switch (order_of_waves[an_count])
							{
								case PWAV:  // allowed, but do nothing (stay here)
									break;
								case QWAV:	state = 5; // good, so move on a state
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 5:
						{
							switch (order_of_waves[an_count])
							{
								case QWAV:  // allowed, but do nothing (stay here)
									break;
								case RWAV:	state = 6; // good, so move on a state
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 6:
						{
							switch (order_of_waves[an_count])
							{
								case RWAV:  // allowed, but do nothing (stay here)
									break;
								default:		state = 7; // bad, so go to end state
							}
							break;
						}
						case 7:
						{
							// This is the final ending state for bad wave forms,
							// so we stay here.
							break;
						}
						default:
						{
							// should not be here, set state to state 6
							state = 7;
							break;
						}
					} // end case statement
				}

				if (state == 6)
				{
					// good state reached, so set section as good
					good_bad_sects[sect_counter].status = SECT_GOOD;
				}else
				{	// bad state must have been reached (either 7 or stuck at 1)
					good_bad_sects[sect_counter].status = SECT_BAD;
				}
				// fill in rest of data for section
				good_bad_sects[sect_counter].start = start_point;
				good_bad_sects[sect_counter].finish = present_point;
				sect_counter++;
				// reset order_of counter for next section
				order_of_counter = 0;
				// move start point on ready for next section
				start_point = present_point;
				// set this up so that the start of the loop will clear past the
				// first rwave
				order_of_waves[order_of_counter++]=RWAV;
			}
		} else
		{
			// we have reached the end of the ecg so mark as startend
			good_bad_sects[sect_counter].start = start_point;
			good_bad_sects[sect_counter].finish = present_point;
			good_bad_sects[sect_counter].status = SECT_STARTEND;
			sect_counter++;
		}
		present_point+=step(); // move on
	}
	good_bad_sects[sect_counter].status = TSECT_END_OF_ARRAY;
	set_gb_sects(good_bad_sects); // pass data back to ecg class
	set_good_bad_checked(TRUE);
}

/////////////////////////////////////////////////////////
// Running Mean Calc analysis class functions
/////////////////////////////////////////////////////////

void TAnalyse_Run_Mean::Execute()
{
	switch (get_run_mean_calc())
	{
		case TRMC_FULL:
		{
			long total = 0;
			for (int i=0; i < length(); i+= step())
			{
				total += (*get_ecg())[i].GetPoints().y;
				(*get_ecg())[i].SetRunMean(total/((i/step())+1));
			}
			break;
		}
		case TRMC_RTR:
		{
			if (IsAnalysed()) // function can be called in both cases
			{
				int i=0;
				int start=0;
				int finish=0;
				int main_start=0;
				// get first one (to store main start)
				if (return_range(i, &start, &finish))
				{
					long total=0;
					long average;
					main_start = start;
					for (int j=start; j <= finish; j+=step())
					{
						total += (*get_ecg())[j].GetPoints().y;
					}
					average = total/(((finish-start)/step())+1);
					for (int k=start; k <= finish; k++)
					{
						(*get_ecg())[k].SetRunMean(average);
					}
					i = finish;
				}
				else
				{
					// if we are here, then two rwaves were not found
					// so set whole lot to -1
					for (i = 0 ; i < length(); i++)
					{
						(*get_ecg())[i].SetRunMean(-1);
					}
				}
				while (i < length() && return_range(i, &start, &finish))
				{
					long total=0;
					long average;
					for (int j=start; j <= finish; j+=step())
					{
						total += (*get_ecg())[j].GetPoints().y;
					}
					average = total/(((finish-start)/step())+1);
					for (int k=start; k <= finish; k++)
					{
						(*get_ecg())[k].SetRunMean(average);
					}
					i = finish;
				}
				if (main_start != 0 )
				{  // clean up start and end of ecg
					i = start;
					while (i < length())
					{
						(*get_ecg())[i].SetRunMean((*get_ecg())[i-1].GetRunMean());
						i++;
					}
					for (i = main_start; i >= 0 ; i--)
					{
						(*get_ecg())[i].SetRunMean((*get_ecg())[i+1].GetRunMean());
					}
				}
			}
			break;
		}
		case TRMC_RTRWIN:
		{  // Not yet implemented
			break;
		}
		default:
		{  // Should not Happen
			break;
		}
	}
}

/////////////////////////////////////////////////////////
// Invert Calc analysis class functions
/////////////////////////////////////////////////////////

void TAnalyse_Invert::Execute()
{
	// invert entire ecg data
	int zero_level = get_zero_level();
	for (int i = 0 ; i < length() ; i ++)
	{
		(*get_ecg())[i].Set_y((2*zero_level)-(*get_ecg())[i].GetPoints().y);
		(*get_ecg())[i].SetRunMean((2*zero_level) - (*get_ecg())[i].GetRunMean());
	}
	int y_min = (2*zero_level)-get_y_max();
	set_y_max((2*zero_level)-get_y_min());
	set_y_min(y_min);
	set_y_mean((2*zero_level) - get_y_mean());
	set_r_thresh_real((2*zero_level) - get_r_thresh());
}

/////////////////////////////////////////////////////////
// Filter analysis class functions
/////////////////////////////////////////////////////////

void TAnalyse_Filter::Execute()
{
	switch (filt_mode)
	{
		case IDS_FILT_HANNING :
		{
			Run_Hanning_Filter(); // Execute Hanning Filter
			break;
		}
		case IDS_FILT_LEASTSQUARE :
		{
			Run_Least_Square_Filter();// Execute least square algorythm
			break;
		}
		case IDS_FILT_IIRLOWPASS :
		{
			Run_IIR_Low_Pass_Filter(); // Execute IRR Low Pass filter
			break;
		}
		case IDS_FILT_ADAPT :
		{
			Run_Adapt_Filter(); // Execute adaptive filter filter
			break;
		}
		case IDS_FILT_TWO_POLE_REC:
		{
			Run_Two_Pole_Filter(); // Execute Two pole recursive filter
			break;
		}
		case IDS_FILT_INTEGER:
		{
			Run_Integer_filter(); // Execute Integer filter
			break;
		}
		case IDS_FILT_QRSDET_LOWPASS:
		{
			Run_QRSDet_Low_Pass_Filter(); // Execute QRS Low pass filter
			break;
		}
		case IDS_FILT_QRSDET_HIGHPASS:
		{
			Run_QRSDet_High_Pass_Filter(); // Execute QRS High pass filter
			break;
		}
		case IDS_FILT_QRSDET_DERIV:
		{
			Run_QRSDet_Derivative_Filter(); // Execute QRS Derivative filter
			break;
		}
		case IDS_FILT_QRSDET_SQUARE:
		{
			Run_QRSDet_Square_Filter(); // Execute QRS Square all function
			break;
		}
		case IDS_FILT_QRSDET_MOVEINT:
		{
			Run_QRSDet_Moveing_Integer_Filter(); //Execute Moving integer(win size)
			break;
		}
		case IDS_FILT_QRSDET_RUN_ALL:
		{
			// Run all the QRS Detection functions in order
			Run_QRSDet_Low_Pass_Filter();
			Run_QRSDet_High_Pass_Filter();
			Run_QRSDet_Derivative_Filter();
			Run_QRSDet_Square_Filter();
			Run_QRSDet_Moveing_Integer_Filter();
			break;
		}
		default:
		{	// should not be here
			break;
		}
	}
}

void TAnalyse_Filter::Run_Hanning_Filter()
{
	/* Hanning filter    (From Biometrical digital signal processing P107)
	Difference equation:
		y(nT) = (x(nT) + 2*x(nT - T) + x(nT - 2T))/4
	C language implementation equation:
		ynt = (xnt + 2*xm1 + xm2)/4
	*/
	int i, xnt, xm1, xm2;

	xm2 = (*get_ecg())[0].GetPoints().y;
	xm1 = (*get_ecg())[1].GetPoints().y;

	for(i = 2; i < length(); i++) // may want i+=step()
	{
		xnt = (*get_ecg())[i].GetPoints().y;

		(*get_ecg())[i-1].Set_y( (xnt + 2*xm1 + xm2)/4 ); // should be into a temporary y-value
		xm2 = xm1;
		xm1 = xnt;
	}
}

void TAnalyse_Filter::Run_Least_Square_Filter()
{
	// Implementation of a Least Squares polynomial filter
	// uses the opt_val intiger to determine how many taps to use
	//  opt_val			tap weights
	//		2			1/35(-3, 12, 17, 12, -3)
	//		3			1/21(-2, 3, 6, 7, 6, 3, -2)
	//		4			1/231(-21, 14, 39, 54, 59, 54, 39, 14, -21)
	//		5			1/429(-36, 9, 44, 69, 84, 89, 84, 69, 44, 9, -36)

	long x[11];
	int count;
	long y;
	for (count=10; count>=0; count--)
	{
		x[count]=0; // initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=(opt_val*2); count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		switch (opt_val)
		{
		case 2: y = (-3*x[0] + 12*x[1] + 17*x[2] + 12*x[3] - 3*x[4])/35;
			break;
		case 3: y = (-2*x[0] + 3*x[1] + 6*x[2] + 7*x[3] + 6*x[4] + 3*x[5]
							- 2*x[6])/21;
			break;
		case 4: y = (-21*x[0] + 14*x[1] + 39*x[2] + 54*x[3] + 59*x[4] + 54*x[5]
							+ 39*x[6] + 14*x[7] - 21*x[8])/231;
			break;
		case 5: y = (-36*x[0] + 9*x[1] + 44*x[2] + 69*x[3] + 84*x[4] + 89*x[5]
							+ 84*x[6] + 69*x[7] + 44*x[8] + 9*x[9] -36*x[10])/429;
			break;
		default: y=x[0]; // should not be here but return original just in case
		}

		(*get_ecg())[i].Set_y((int)y); // may want to use pos i-opt_val (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_IIR_Low_Pass_Filter()
{
	// Implementation of an IRR Low-Pass filter
	// using r and q method where r = 0.5 and q = 45 degrees
	// the difference equation is:
	// y(nT) = 0.707y(nT-T)-0.25y(nT-2T)+x(nT)+2x(nT-T)+x(nT-2T)
	// y(nT) = 0.707*y[1] - 0.25*y[2] + x[0] + 2*x[1] + x[2]

	float x[3], y[3];
	int count;
	for (count=2; count>=0; count--)
	{
		x[count]=0;
		y[count]=0; //initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=2; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y[0]=0.707*y[1] - 0.25*y[2] + x[0] + 2*x[1] + x[2];

		for (count=2; count>0; count--)
		{
			y[count]=y[count-1];	// shift y term delays
		}

		(*get_ecg())[i].Set_y(y[0]/7.366482); // may want to use pos i-1 (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_Adapt_Filter()
{
// implementation of 60Hz adaptive filter
// for array values, 0 = nT+T, 1 = nT, 2 = nT-T
	float N;
	long d=.25;
	float err[2], x[2], y, f;
	int count;

	N=cos((2*3.141592636*60)/750);//.9999615;  // cos( (2Pi*60Hz)/750 sps)
	for (count=2; count>=0; count--)
	{
		x[count]=0;
		err[count]=0; //initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=2; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
			err[count]=err[count-1];	// shift error term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point


		err[0]=2*N*err[1]-err[2];
//		dc[0]=x[0]-err[0];
//		dc[1]=x[1]-err[1];
		f=x[0]-err[0]-(x[1]-err[1]);
		if (f > 0)
		{
			err[0]=err[0]+d;
		}
		else if (f < 0)
		{
			err[0]=err[0]-d;
		}
		y=x[0]-err[0];
		(*get_ecg())[i].Set_y(y);
	}
}

void TAnalyse_Filter::Run_Two_Pole_Filter()
{
	// Implementation of an two pole recursive filter
	// the difference equation is:
	// y(nT) = 1.87635y(nT-T)-0.9219y(nT-2T)+x(nT)-x(nT-2T)
	// y(nT) = 1.87635*y[1] - 0.9219*y[2] + x[0] - x[2]

	float x[3], y[3];
	int count;
	for (count=2; count>=0; count--)
	{
		x[count]=0;
		y[count]=0; //initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=2; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y[0]=1.87635*y[1] - 0.9216*y[2] + x[0] - x[2];  // difference equation

		for (count=2; count>0; count--)
		{
			y[count]=y[count-1];	// shift y term delays
		}

		(*get_ecg())[i].Set_y(y[0]); // may want to use pos i-1 (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_Integer_filter()
{
	// Implementation of an integer filter
	// the difference equation is:
	// y(nT) = 2y(nT-T)-3y(nT-2T)+2y(nT-3T)-y(nT-4T)+x(nT)-2x(nT-12T)+x(nT-24T)
	// y(nT) = y[0]=2*y[1] - 3*y[2] + 2*y[3] - y[4]  + x[0] - 2*x[12] + x[24]

	float x[25], y[5];
	int count;
	for (count=24; count>=0; count--)
	{
		x[count]=0;
		if (count < 5)
			y[count]=0; //initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=24; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y[0]=2*y[1]-3*y[2]+2*y[3]-y[4]+x[0]-2*x[12]+x[24];  // difference equation

		for (count=4; count>0; count--)
		{
			y[count]=y[count-1];	// shift y term delays
		}

		(*get_ecg())[i].Set_y(y[0]); // may want to use pos i-1 (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_QRSDet_Low_Pass_Filter()
{
	// Implementation of low pass QRS Detecting filter
	// the difference equation is:
	// y(nT) = 2y(nT-T)-y(nT-2T)+x(nT)-2x(nT-6T)+x(nT-12T)
	// y(nT) = y[0]=2*y[1] - y[2] + x[0] - 2*x[6] + x[12]

	float x[13], y[3];
	int count;
	for (count=12; count>=0; count--)
	{
		x[count]=0;
		if (count < 3)
			y[count]=0; //initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=12; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y[0]=2*y[1]-y[2]+x[0]-2*x[6]+x[12];// difference equation

		for (count=2; count>0; count--)
		{
			y[count]=y[count-1];	// shift y term delays
		}
		if (i>=5)  // ensure dont write to array points less than 0
		(*get_ecg())[i-5].Set_y(y[0]/36); // may want to use pos i-5 (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_QRSDet_High_Pass_Filter()
{
	// Implementation of high pass QRS Detecting filter
	// the difference equation is:
	// y(nT) = x(nT-16T)- 1/32[y(nT-T)+x(nT)-x(nT-32T)
	// y(nT) = y[0] = x[16] -(y[1] +x[0] - x[32])/32

	float x[33], y[2];
	int count;
	for (count=32; count>=0; count--)
	{
		x[count]=0;
		if (count < 2)
			y[count]=0; //initialise
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=32; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y[0]=x[16] -(y[1] +x[0] - x[32])/32;// difference equation

		for (count=1; count>0; count--)
		{
			y[count]=y[count-1];	// shift y term delays
		}
		if (i>=16)  //ensure array index > 0
			(*get_ecg())[i-16].Set_y(y[0]); // may want to use pos i-16 (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_QRSDet_Derivative_Filter()
{
	// Implementation of Derivative QRS Detecting filter
	// the difference equation is:
	// y = 1/10 (2x(nT)+x(nT-T)-x(nT-3T)-2x(nT-4T)
	// y = (2*x[0] + x[1] -x[3] -2*x[4])/10

	int zero = get_zero_level();
	float x[5], y;
	int count;
	for (count=4; count>=0; count--)
	{
		x[count]=0;
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		for (count=4; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y=(2*x[0] + x[1] -x[3] -2*x[4]);// difference equation

		if (i >=2)  // ensure array index > 0
			(*get_ecg())[i-2].Set_y(y+zero); // may want to use pos i-2 (so ecg wont shift)

	}
}

void TAnalyse_Filter::Run_QRSDet_Square_Filter()
{
	// Implementation of squaring function
	// just square all values (subtract value of zero volts first then replace)
	int x;
	float y;
	int zero = get_zero_level();
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		x=(*get_ecg())[i].GetPoints().y; // load in next point

		y= (((x-zero)/10)*((x-zero)/10));   // for now

		(*get_ecg())[i].Set_y((y>INT_MAX)?INT_MAX:(int)y); // for now

	}
}

void TAnalyse_Filter::Run_QRSDet_Moveing_Integer_Filter()
{
	// Implementation of a moving integrator

	int x[32];
	long double y=0;
	int count;
	for (count=31; count>=0; count--)
	{
		x[count]=0;
	}
	for (int i=0; i<length(); i++)// may want i+=step();
	{
		y -= x[31];
		for (count=31; count>0; count--)
		{
			x[count]=x[count-1];	// shift x term delays
		}

		x[0]=(*get_ecg())[i].GetPoints().y; // load in next point

		y+=x[0];  // difference equation

		(*get_ecg())[i].Set_y((y>INT_MAX)?INT_MAX:(int)y); // for now

	}
}
