#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "slidingwindow.h"

void sliding_sum(double * output_arr, double * input_arr, long input_arr_size, long win_size){
	// Step 3: Sum the first window
	double cur_sum = 0.0;
	for (long i = 0; i != win_size; ++i){
		cur_sum += input_arr[i];
	}
	output_arr[0] = cur_sum;
	// Step 4: Do sliding window for the remaining numbers
	long next_index = win_size;
	for (long i = 0; i != input_arr_size-win_size; ++i){
		cur_sum -= input_arr[i];
		cur_sum += input_arr[next_index];
		output_arr[i+1] = cur_sum;
		++next_index;
	}
}

void sliding_ttest(double * output_arr, double * input_arr, long input_arr_size, long win_size){
	// Step 1: Figure out the first t-value
	double cur_mom1 = 0.0;
	double cur_mom2 = 0.0;
	double counter = 1.0;
	double delta = 0.0;
	double db_win_size = (double) win_size;
	double nm1 = db_win_size - 1.0;
	for (long i = 0; i != win_size; ++i){
		delta = input_arr[i] - cur_mom1;
		cur_mom1 += delta/counter;
		cur_mom2 += delta * (input_arr[i] - cur_mom1);
		counter += 1.0;
	}
	// Store the value in the first position
	output_arr[0] = fabs(cur_mom1/(sqrt(cur_mom2) / db_win_size));

	// Step 2: Calculate the rest of the t-values
	long counter2 = win_size;
	long num_iter = input_arr_size - win_size;

	double old_val_min_mu = 0.0;
	double old_val_min_mu_sqr = 0.0;
	double win_size_div_nm1 = db_win_size / nm1;

	for (long i = 0; i != num_iter; ++i){
		// Do calculation
		old_val_min_mu = input_arr[i] - cur_mom1;
		old_val_min_mu_sqr = old_val_min_mu * old_val_min_mu;
		// remove contribution of old_value
		cur_mom2 -= win_size_div_nm1 * old_val_min_mu_sqr;
		cur_mom1 = (db_win_size * cur_mom1 - input_arr[i]) / nm1;
		// add contribution of new value
		delta = input_arr[counter2] - cur_mom1;
		cur_mom1 += delta / db_win_size;
		cur_mom2 += delta * (input_arr[counter2] - cur_mom1);

		//increment counter2
		++counter2;
		//store t_value
		output_arr[i + 1] = fabs(cur_mom1/(sqrt(cur_mom2) / db_win_size));
	}
}