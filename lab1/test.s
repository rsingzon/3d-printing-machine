	AREA test, CODE, READONLY
	EXPORT Kalmanfilter_asm

Kalmanfilter_asm
	; Register descriptions
	; R0: Pointer to test data
	; R1: Pointer to filtered data array
	; R2: Array length
	; R3: Pointer to Kalman struct
	
	
	; R7: Pointer to the end of the test data array
	
	; S0: 
	; S1
	
	; Struct value addresses
	;		q = R2
	;		r = R2 + 4
	;		x = R2 + 8
	; 		p = R2 + 12
	; 		k = R2 + 16

structAddress EQU 0x20000000			; Start address of Kalman struct.  Address corresponds to the beginning of SRAM
arrayLength	EQU 5*4							
increment EQU 4							; Amount to increment pointers by
      
	LDR R1, =test_vector 				; Pointer to measurements
	MOV R2, #structAddress				; Load the address of the Kalman struct
	ADD R3, R2, #arrayLength			; Load the address of the filtered array
	ADD R7, R1, #arrayLength			; Add test vector length to test vector pointer
	
	SUB R13, R13, #4					; Decrement stack pointer
	STR	R7, [R13]						; Store the array length into the stack
	
	; Initialize 'q' and 'r' to something to prevent getting divide by zero
	VMOV.F32 S0, #10.0
	VMOV.F32 S1, #1.0
	VDIV.F32 S0, S1, S0
	VSTR.F32 S0, [R2]  					; Store 0.1 into q
	VSTR.F32 S0, [R2, #4]				; Store 0.1 into r
	
kalman_update
	
	LDR R7, [R13]						; Load the array length from the stack
	
	; self.p = self.p + self.q
	VLDR.F32 S0, [R2, #12]				; Load 'p' from memory
	VLDR.F32 S1, [R2]					; Load 'q' from memory
	VADD.F32 S1, S0, S1					; S1 = self.p + self.q
	VSTR.F32 S1, [R2, #12]				; Store 'p' into memory
	VMOV.F32 S0, S1						; Move new value of p into S0
	
	; self.k = self.p / (self.p + self.r)
	VLDR.F32 S1, [R2, #4]				; Load value 'r' from memory
	VADD.F32 S1, S0, S1					; (self.p + self.r)
	VDIV.F32 S1, S0, S1					; self.p / (self.p + self.r)
	VSTR.F32 S1, [R2, #16]				; Store 'k' into memory
	
	; self.x = self.x + self.k * (measurement - self.x)
	VLDR.F32 S2, [R2, #8]				; Load 'x' from memory
	VLDR.F32 S3, [R1]					; Load measurement from memory
	VSUB.F32 S4, S3, S2					; (measurement - self.x)
	VMUL.F32 S4, S1, S4					; self.k * (measurement - self.x)
	VADD.F32 S2, S2, S4					; self.x + self.k * (measurement - self.x)
	VSTR.F32 S2, [R2, #8]				; Store 'x' into memory
	
	VSTR.F32 S2, [R3]					; Store x into the filtered array
	
	; self.p = (1 - self.k) * self.p
	VLDR.F32 S0, [R2, #12]				; Load 'p' from memory
	VMOV.F32 S2, #1						; Load 1 into S2
	VSUB.F32 S2, S2, S1					; (1 - self.k)
	VMUL.F32 S0, S2, S0					; (1 - self.k) * self.p
	VSTR.F32 S0, [R2, #12]				; Store 'p' into memory
	
	ADD R1, R1, #increment				; Increment the data pointer
	ADD R3, R3, #increment				; Increment the filtered array pointer
	
	CMP R7, R1							; Check if there are more data points left
	BGE kalman_update					; Loop if there are more measurements

	LTORG
	ADD R10, R10, #1					; Dummy instruction for finding runtime
	BX LR								; Return to startup 	
	
	align 4
test_vector
	DCFS 1.5, 3.2, 2.5, 3.4, 5.3
	END

	