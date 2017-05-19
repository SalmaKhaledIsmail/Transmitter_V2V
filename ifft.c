#ifndef IFFT_C_
#define IFFT_C_



#include "ifft.h"
#include <math.h>
#include <ti/dsplib/dsplib.h>
#include <stdlib.h>

extern unsigned char brev[64] = {
    0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
    0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
    0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
    0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
    0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
    0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
    0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
    0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
};

/* since complex here is written in the terms of a float array, with real in even indices and imaginary in odd indices, this funcion separates real and imaginary numbers in different arrays*/
void seperateRealImg(float* input, float* real, float*img, int N) {
    int i, j;

    for (i = 0, j = 0; j < N; i+=2, j++) {
        real[j] = input[i];
        img[j] = input[i + 1];
    }
}

/* this ready function generates the twiddle factors that will be used in IFFT function*/
void gen_twiddle_fft_sp (float *w, int n)
{
    int i, j, k;
    double x_t, y_t, theta1, theta2, theta3;
    const double PI = 3.141592654;

    for (j = 1, k = 0; j <= n >> 2; j = j << 2)
    {
        for (i = 0; i < n >> 2; i += j)
        {
            theta1 = 2 * PI * i / n;
            x_t = cos (theta1);
            y_t = sin (theta1);
            w[k] = (float) x_t;
            w[k + 1] = (float) y_t;

            theta2 = 4 * PI * i / n;
            x_t = cos (theta2);
            y_t = sin (theta2);
            w[k + 2] = (float) x_t;
            w[k + 3] = (float) y_t;

            theta3 = 6 * PI * i / n;
            x_t = cos (theta3);
            y_t = sin (theta3);
            w[k + 4] = (float) x_t;
            w[k + 5] = (float) y_t;
            k += 6;
        }
    }
}


/* this function shifts the input so that it swaps the two halves of the input (gnuradio uses shift so we did it here as well) */
void shiftF(float* before, float* after, int N)
{
		int n;
		for(n = 0; n < (2*N)/2 ; n++)
		{
			after[n] = before[(2*N)/2 + n];
		}
		n = (2*N)/2;
		for(n = (2*N)/2; n < (2*N) ; n++)
		{
			after[n] = before[n - (2*N)/2];
		}
}

void ifft(float* input, float* output, int N, float WindowScale, int shift, float* window)
{
	 gen_twiddle_fft_sp(window, N);
	 /* this small part multiples the input by N to reverse the normalization and divides it by the the value of WindowScale to scale the input */
	    int k = 0;
	    for (k = 0; k<2*N; k++)
	    {
	    	input[k] = input[k] * ((float) N / (sqrt((float)WindowScale)));
	    }


	    if(shift  == true)
	    {
	    	float* temp = calloc(2*N, sizeof(float));
	    	shiftF(input, temp, N);
	    	input = temp;
	    	free(temp);
	    }

	    if(N%4 == 0)
	    {
	    	DSPF_sp_ifftSPxSP(N, input, window, output, brev, 4, 0, N);
	    }
	    else
	    {
	    	DSPF_sp_ifftSPxSP(N, input, window, output, brev, 2, 0, N);
	    }


	    /* *
	     * void DSPF_sp_ifftSPxSP (int N, float *ptr_x, float *ptr_w, float *ptr_y,
    unsigned char *brev, int n_min, int offset, int n_max);
	     *      @param N         length of FFT in complex samples
 *         @param ptr_x     pointer to complex data input
 *         @param ptr_w     pointer to complex twiddle factor
 *         @param ptr_y     pointer to complex output data
 *         @param brev      pointer to bit reverse table containing 64 entries
 *         @param n_min     should be 4 if N can be represented as Power of 4 else, n_min should be 2
 *         @param offset    index in complex samples of sub-fft from start of main fft
 *         @param n_max     size of main fft in complex samples
 *
 **/
}


#endif
