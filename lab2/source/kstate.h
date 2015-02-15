#ifndef KALMAN_FILTER_STATE
#define KALMAN_FILTER_STATE

typedef struct kalman_state
{
	float q;
	float r;
	float x;
	float p;
	float k;
} kalman_state;


float kalman_update(kalman_state* kstate, float measurement);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);

#endif