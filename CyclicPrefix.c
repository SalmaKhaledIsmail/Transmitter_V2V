/*
 * CyclicPrefix.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: User Tito
 */
#include "CyclicPrefix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> // as i use cosine function

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

void CyclicPrefix_Init(StructCyclicPrefix_Init *Cyclic,int num_ofdm_sym)
{
	Cyclic->d_fft_len= 64;
	Cyclic->d_output_size=80;
	Cyclic->d_rolloff_len=2;
	Cyclic->symbols_to_read=num_ofdm_sym;
}

void CyclicPrefix(StructCyclicPrefix_Init *PtrToStruct,float *data_ptr,float *out_ptr)
{
	int i;
	int d_cp_size = PtrToStruct->d_output_size - PtrToStruct->d_fft_len;
	float *d_up_flank;
	float *d_down_flank;
	float *d_delay_line;
    if (PtrToStruct->d_rolloff_len == 1)
    {
    	PtrToStruct->d_rolloff_len = 0;
    }

    if (PtrToStruct->d_rolloff_len)
    {
    	d_up_flank=(float*) malloc((PtrToStruct->d_rolloff_len-1)*sizeof(float));
		d_down_flank=(float*) malloc((PtrToStruct->d_rolloff_len-1)*sizeof(float));
		d_delay_line=(float*) malloc((PtrToStruct->d_rolloff_len-1)*sizeof(float));
		//---------- construct up flank and down flank ----------//
		for (i = 1; i < PtrToStruct->d_rolloff_len; i++)
		{
			d_up_flank[i-1]   = 0.5 * (1 + cos(M_PI *(float) i/(float)PtrToStruct->d_rolloff_len - M_PI));
			d_down_flank[i-1] = 0.5 * (1 + cos(M_PI *(float)(PtrToStruct->d_rolloff_len-i)/(float)PtrToStruct->d_rolloff_len - M_PI));
			d_delay_line[i-1]=0;
		}
    }
	//---------- cyclic prefix implementation ----------//
	float *in=data_ptr;
	float *out=out_ptr;
	int sym_idx;
	 for (sym_idx = 0; sym_idx < PtrToStruct->symbols_to_read;sym_idx++)
	 {
		memcpy((out + (d_cp_size*2)),in, PtrToStruct->d_fft_len * sizeof(float)* 2);
		memcpy(out,(in + (PtrToStruct->d_fft_len*2) - (d_cp_size*2)), d_cp_size * sizeof(float)* 2);
		if (PtrToStruct->d_rolloff_len)
		{
		  for (i = 0; i < PtrToStruct->d_rolloff_len-1; i+=2)
		  {
		    out[i] = out[i] * d_up_flank[i/2] + d_delay_line[i/2]; //real part
		    out[i+1] = out[i+1] * d_up_flank[i/2] + d_delay_line[i/2]; // imaginary part
		    d_delay_line[i/2] = in[i] * d_down_flank[i/2]; //real part
		    d_delay_line[i/2] = in[i+1] * d_down_flank[i/2]; //imaginary part
		  }
		}
		in += (PtrToStruct->d_fft_len*2);
		out += (PtrToStruct->d_output_size*2);
	  }
//---------- adding delay line for the last OFDM symbol ----------//
	 if (PtrToStruct->d_rolloff_len)
	 {
	 	  for (i = 0; i < PtrToStruct->d_rolloff_len-1; i++)
	 	  {
	 		  *out++ = d_delay_line[i/2]; //  real part
	 	      *out++ = d_delay_line[i/2]; // imaginary part
	 	  }
	 	 free(d_delay_line);
	 	 free(d_up_flank);
	 	 free(d_down_flank);
	 }
}



