#ifndef IFFT_H_
#define IFFT_H_

#include "IEEE802_11_Common_Variables.h"
extern void gen_twiddle_fft_sp (float *w, int n);
extern void shiftF(float* before, float* after, int N);
extern void ifft(float* input, float* output, int N, float WindowScale, int shift, float* window);
extern void seperateRealImg(float* input, float* real, float*img, int N);


#endif /* IFFT_H_ */
