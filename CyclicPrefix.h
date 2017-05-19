/*
 * CyclicPrefix.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: User Tito
 */

#ifndef CYCLICPREFIX_H_
#define CYCLICPREFIX_H_
typedef struct
{
	int d_fft_len; // initialized in gnuradio by 64 (size of IFFT block)
	int d_output_size; // output size = d_fft_len + cyclic prefix size(initialized in gnuradio by 16)
	int d_rolloff_len; // initialized in gnuradio by 2
	int symbols_to_read; // number of OFDM symbols input to this block
}StructCyclicPrefix_Init;
void CyclicPrefix(StructCyclicPrefix_Init*,float*,float*);
void CyclicPrefix_Init(StructCyclicPrefix_Init*,int);





#endif /* CYCLICPREFIX_H_ */
