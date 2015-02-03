	AREA test, CODE, READONLY
	EXPORT Kalmanfilter_asm

Kalmanfilter_asm
	; Register descriptions
	; R1: Pointer to test data
	; R2: Pointer to Kalman struct
	;		q = R2
	;		r = R2 + 4
	;		x = R2 + 8
	; 		p = R2 + 12
	; 		k = R2 + 16
	
	

startAddress EQU 0x20000000				; Start address of Kalman struct.  Address corresponds to the beginning of SRAM

;;;Temporarily hard code the number of data points in the test vector
arrayLength	EQU 5*4							
filteredArray EQU 0x21000000			; Start address for the filtered array
increment EQU 4							; Amount to increment pointers by
structHeight EQU 20				        ; Note: this value is dependent on the depth									
      
	LDR R1, =test_vector 				; Pointer to measurements
	MOV R2, #startAddress				; Load the address of the Kalman struct
	MOV R3, #filteredArray				; Load the address of the filtered array
	ADD R7, R1, #arrayLength			; Add test vector length to test vector pointer
	
	; Initialize 'q' and 'r' to something to prevent getting divide by zero
	VMOV.F32 S0, #0.5
	VSTR.F32 S0, [R2]  					; Store 0.5 into q
	VSTR.F32 S0, [R2, #4]				; Store 0.5 into r
	
kalman_update
	
	; self.p = self.p + self.q
	VLDR.F32 S0, [R2, #12]				; Load 'p' from memory
	VLDR.F32 S1, [R2]					; Load 'q' from memory
	VADD.F32 S1, S0, S1					; S1 = self.p + self.q
	VSTR.F32 S1, [R2, #12]				; Store 'p' into memory
	
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
	ADD R1, R1, #increment				; Increment the filtered array pointer
	
	CMP R7, R1							; Check if there are more data points left
	BGE kalman_update					; Loop if there are more measurements

	LTORG
	BX LR								; Return to startup 	
	
	align 4
test_vector
	DCFS 0.08592903,  0.02212529,  0.09950619, -0.0108313,  0.13294764
	END

	