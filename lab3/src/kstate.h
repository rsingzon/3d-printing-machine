/**
  * @file    kstate.h
  * @author  Jeffrey Tichelman, Ryan Singzon
  * @date    March 11, 2015
  * @brief   Defines the struct for the kalman filter state
	*/

#ifndef __KSTATE_H
#define __KSTATE_H

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