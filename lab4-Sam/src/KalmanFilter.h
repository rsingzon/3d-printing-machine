#ifndef	 KALMANFILTER_H
#define KALMANFILTER_H

typedef struct
{
	float	q,r,x,p,k; 
	
}kalman_state;

kalman_state Kalmanfilter_C(kalman_state *k_state, int input);


#endif