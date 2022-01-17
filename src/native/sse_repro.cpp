#include <windows.h>
#include <immintrin.h>
#include <stdio.h>
 
void __vectorcall  ShuffleSSERegisters_vector(
	__m128 xmm_write_register_0,
	__m128 xmm_write_register_1,
	__m128 xmm_write_register_2,
	__m128 xmm_write_register_3,
	__m128 xmm_write_register_4,
	__m128 xmm_write_register_5,
	__m128 xmm_write_register_6,
	__m128 xmm_write_register_7) {
	// Use the registers for printf so the compiler doesn't optimize everything away
	printf("\nPointers are %p, %p, %p, %p, %p, %p, %p, %p\n",
		(&xmm_write_register_0),
		(&xmm_write_register_1),
		(&xmm_write_register_2),
		(&xmm_write_register_3),
		(&xmm_write_register_4),
		(&xmm_write_register_5),
		(&xmm_write_register_6),
		(&xmm_write_register_7)
	);
}

extern "C" __declspec(dllexport) float* __stdcall ShuffleSSERegisters() {
	// __m128 holds an SSE compatible value in a struct
	__m128 xmm_write_register_0;
	__m128 xmm_write_register_1;
	__m128 xmm_write_register_2;
	__m128 xmm_write_register_3;
	__m128 xmm_write_register_4;
	__m128 xmm_write_register_5;
	__m128 xmm_write_register_6;
	__m128 xmm_write_register_7;

	// Fill an buffer with looping bytes 00..FF
	byte* writebuffer = (byte*)calloc(4 * 8, 32);
	for (int i = 0; i < 4 * 8 * 32; i++)
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
	xmm_write_register_6 = _mm_loadu_ps(fake + (4 * 6));
	xmm_write_register_7 = _mm_loadu_ps(fake + (4 * 7));

	// Pass the placeholders to our __vectorcall function
	// VC is going to put them in registers as per __vectorcall
	// specification.
	ShuffleSSERegisters_vector(
		xmm_write_register_0,
		xmm_write_register_1,
		xmm_write_register_2,
		xmm_write_register_3,
		xmm_write_register_4,
		xmm_write_register_5,
		xmm_write_register_6,
		xmm_write_register_7
		);

	// Return our test data to Java for comparison
	return fake;
}

extern "C" __declspec(dllexport) float* __stdcall ReadSSERegisters() {
	// Structs we read the data into
	__m128 xmm_read_register_0;
	__m128 xmm_read_register_1;
	__m128 xmm_read_register_2;
	__m128 xmm_read_register_3;
	__m128 xmm_read_register_4;
	__m128 xmm_read_register_5;
	__m128 xmm_read_register_6;
	__m128 xmm_read_register_7;

	// Move the current register values to the placeholder variables
	__asm {
		MOVDQU xmm_read_register_0, xmm0
		MOVDQU xmm_read_register_1, xmm1
		MOVDQU xmm_read_register_2, xmm2
		MOVDQU xmm_read_register_3, xmm3
		MOVDQU xmm_read_register_4, xmm4
		MOVDQU xmm_read_register_5, xmm5
		MOVDQU xmm_read_register_6, xmm6
		MOVDQU xmm_read_register_7, xmm7
	}

	// Read the placeholders into a bunch of floats
	float* readbuffer = new float[4 * 8];
	_mm_storeu_ps(readbuffer + (4 * 0), xmm_read_register_0);
	_mm_storeu_ps(readbuffer + (4 * 1), xmm_read_register_1);
	_mm_storeu_ps(readbuffer + (4 * 2), xmm_read_register_2);
	_mm_storeu_ps(readbuffer + (4 * 3), xmm_read_register_3);
	_mm_storeu_ps(readbuffer + (4 * 4), xmm_read_register_4);
	_mm_storeu_ps(readbuffer + (4 * 5), xmm_read_register_5);
	_mm_storeu_ps(readbuffer + (4 * 6), xmm_read_register_6);
	_mm_storeu_ps(readbuffer + (4 * 7), xmm_read_register_7);

	return readbuffer;
}