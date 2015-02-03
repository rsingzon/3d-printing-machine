#define KALMAN_FILTER_STATE

struct kalman_state
{
	float q;
	float r;
	float x;
	float p;
	float k;
};