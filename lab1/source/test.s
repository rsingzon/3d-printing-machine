
 ;ECSE 426 - Microprocessor Systems
 ;Singzon, Ryan				260397455
 ;Tichelman, Jeffrey		260446802
 
	
	AREA test, CODE, READONLY
	EXPORT Kalmanfilter_asm

Kalmanfilter_asm
	; Register descriptions
	; R0: Pointer to test data
	; R1: Pointer to filtered data array
	; R2: Pointer to Kalman struct
	; R3: Array length
	
	; R7: Pointer to the end of the test data array
		
	; Struct value addresses
	;		q = R2
	;		r = R2 + 4
	;		x = R2 + 8
	; 		p = R2 + 12
	; 		k = R2 + 16

increment EQU 4							; Amount to increment pointers by
      
	MOV R4, #4							; Load 4 into a register so it can be multiplied
	MUL R3, R3, R4						; Get array length in bytes
	ADD R7, R0, R3						; Add test vector length to test vector pointer
	SUB R7, R7, #1						; Subtract to avoid off by one error
	
	ADD R3, R2, R3						; Load the address of the filtered array
	
	; Initialize 'q' and 'r' to 0.1
	;VMOV.F32 S0, #0.005					
	;VMOV.F32 S1, #5.0
	;VDIV.F32 S0, S1, S0					; Divide 1 by 10 to get 0.1
	;VSTR.F32 S0, [R2]  					; Store 0.1 into q
	;VSTR.F32 S1, [R2, #4]				; Store 0.1 into r
	
	; self.p = self.p + self.q
	VLDM.32 R2, {S0-S4} 				;loads q, r, x, p, k into S0, S1, S2, S3, S4, respectively
	
kalman_update
	
	
	; p= p+q
	VADD.F32 S3, S3, S0					;p=p+q
	
	; k= p/(p+r)
	VADD.F32 S5, S3, S1					;S5=p+r
	VDIV.F32 S4, S3, S5					;k=p/S5
	
	; self.x = self.x + self.k * (measurement - self.x)
	VLDR.F32 S5, [R0]					;load measurement from memory
	VSUB.F32 S5, S5, S2					;S5=measurement-x
	VMUL.F32 S5, S5, S4					;S5 = k*(measurement-x)
	VADD.F32 S2, S2, S5					;x=x+S5
	
	; self.p = (1 - self.k) * self.p
	VMOV.F32 S5, #1						;Brings 1 into S5
	VSUB.F32 S5, S5, S4					;S5=1-k
	VMUL.F32 S3, S3, S5					;p=p*S5
	
	VSTR.F32 S2, [R1]					; Store x into the filtered array
	
	ADD R0, R0, #increment				; Increment the data pointer
	ADD R1, R1, #increment				; Increment the filtered array pointer
	
	CMP R7, R0							; Check if there are more data points left	
	BGE kalman_update					; Loop if there are more measurements
	
	;Store results into memory
	VSTM.32 R2, {S0-S4}

	LTORG
	ADD R10, R10, #1					; Dummy instruction for finding runtime
	BX LR								; Return to startup 	
	

	END

	