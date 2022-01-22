#include <windows.h>
#include <xmmintrin.h>
#include <stdio.h>
#include <cstdint>
 
void ShuffleSSERegisters_vector(
	__m128 xmm_write_register_0,
	__m128 xmm_write_register_1,
	__m128 xmm_write_register_2,
	__m128 xmm_write_register_3,
	__m128 xmm_write_register_4,
	__m128 xmm_write_register_5) {
	// Do something in the function so the compiler keeps it
	printf("fake");
}

extern "C" __declspec(dllexport) int8_t* __stdcall ShuffleSSERegisters() {
	// __m128 holds an SSE compatible value in a struct
	__m128 xmm_write_register_0;
	__m128 xmm_write_register_1;
	__m128 xmm_write_register_2;
	__m128 xmm_write_register_3;
	__m128 xmm_write_register_4;
	__m128 xmm_write_register_5;

	// Fill an buffer with looping bytes 00..FF
	int8_t* writebuffer = (int8_t*)calloc(6, sizeof(__m128));
	for (int i = 0; i < 6 * sizeof(__m128); i++)
		writebuffer[i] = i;

	// Use those bytes as floats for test data
	float* fake = (float*)writebuffer;

	// Load the generated random data into __m128 placeholders
	xmm_write_register_0 = _mm_loadu_ps(fake + (4 * 0));
	xmm_write_register_1 = _mm_loadu_ps(fake + (4 * 1));
	xmm_write_register_2 = _mm_loadu_ps(fake + (4 * 2));
	xmm_write_register_3 = _mm_loadu_ps(fake + (4 * 3));
	xmm_write_register_4 = _mm_loadu_ps(fake + (4 * 4));
	xmm_write_register_5 = _mm_loadu_ps(fake + (4 * 5));

	// Pass the placeholders to our __vectorcall function
	// VC is going to put them in registers as per __vectorcall
	// specification.
	ShuffleSSERegisters_vector(
		xmm_write_register_0,
		xmm_write_register_1,
		xmm_write_register_2,
		xmm_write_register_3,
		xmm_write_register_4,
		xmm_write_register_5
		);

	// Return our test data to Java for comparison
	return writebuffer;
}

extern "C" __declspec(dllexport) int8_t* __stdcall ReadSSERegisters() {
	// Structs we read the data into
	__m128 xmm_read_register_0;
	__m128 xmm_read_register_1;
	__m128 xmm_read_register_2;
	__m128 xmm_read_register_3;
	__m128 xmm_read_register_4;
	__m128 xmm_read_register_5;

	// Move the current register values to the placeholder variables
	__asm {
		MOVDQU xmm_read_register_0, xmm0
		MOVDQU xmm_read_register_1, xmm1
		MOVDQU xmm_read_register_2, xmm2
		MOVDQU xmm_read_register_3, xmm3
		MOVDQU xmm_read_register_4, xmm4
		MOVDQU xmm_read_register_5, xmm5
	}

	// Read the placeholders into a bunch of floats
	float* readbuffer = (float*)calloc(4 * 6, sizeof(float));
	_mm_storeu_ps(readbuffer + (4 * 0), xmm_read_register_0);
	_mm_storeu_ps(readbuffer + (4 * 1), xmm_read_register_1);
	_mm_storeu_ps(readbuffer + (4 * 2), xmm_read_register_2);
	_mm_storeu_ps(readbuffer + (4 * 3), xmm_read_register_3);
	_mm_storeu_ps(readbuffer + (4 * 4), xmm_read_register_4);
	_mm_storeu_ps(readbuffer + (4 * 5), xmm_read_register_5);

	return (int8_t*)readbuffer;
}