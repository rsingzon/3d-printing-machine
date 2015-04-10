/**
 * ECSE 426 - Microprocessor Systems
 * Singzon, Ryan				260397455
 * Tichelman, Jeffrey		260446802
 */

#include <stdio.h>
#include <stdlib.h>
#include "kstate.h"

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