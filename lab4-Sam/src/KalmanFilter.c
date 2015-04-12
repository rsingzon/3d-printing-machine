#include "KalmanFilter.h"
#include <stdio.h>



kalman_state Kalmanfilter_C(kalman_state *k_state, int input){
		
	//static kalman_state kstate;
	
	//kstate.q = 0.0015;
	//kstate.r = 1405.405;
	//kstate.k = 0;
	//kstate.p = 0;
	//kstate.x = input; 
	//peform first time initialization


	//perform kalman filtering

	
		k_state->p = k_state->p + k_state->q;
		k_state->k = k_state->p / (k_state->p + k_state->r);
		k_state->x = k_state->x + ((k_state->k) * (input - k_state->x));
		k_state->p = (1.0 - k_state->k) * (k_state->p);
		
		//printf("%s \n", "Im in kalman state");
		return *k_state;
}