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

#endif