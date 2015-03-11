/**
 * ECSE 426 - Microprocessor Systems
 * Singzon, Ryan				260397455
 * Tichelman, Jeffrey		260446802
 */

#include <stdio.h>
#include <stdlib.h>
#include "kstate.h"

int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length)
{
	int i;
	
	/*initialize p and k*/
	kstate->p = 0.0;
	kstate->k =0.0;
	
	
	return 0;
}

float kalman_update(struct kalman_state* kstate, float measurement){
		/* self.p = self.p+self.q */
		kstate->p =kstate->p + kstate->q;
		
		/*self.k = self.p / (self.p + self.r)*/
		kstate->k = kstate->p / (kstate->p+kstate->r);
		
		/*self.x = self.x + self.k * (measurement - self.x)*/
		kstate->x = kstate->x + kstate->k * (measurement - kstate->x);
		
		/*self.p = (1 - self.k) * self.p*/
		kstate->p = (1-kstate->k) * kstate->p;
		
		/*store x in OutputArray*/
	
	return kstate->x;
}