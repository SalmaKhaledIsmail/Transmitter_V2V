/*
 * chunks_to_symbols_impl.c
 *
 *  Created on: Feb 23, 2017
 *      Author: fc
 */

#include "chunks_to_symbols_impl.h"
#include "constellations_impl.h"


//--------------------------------------------FUNCTIONS IMPLEMENTATIONS---------------------------------//
void chunks_to_symbols_impl(unsigned char *input_items,float *output_items,int data_size,Encoding encoding) //encoding & data_size will be known from frame parameter
	{

	float *d_mapping;

	switch (encoding) {
	case BPSK_1_2:
	case BPSK_3_4:
		d_mapping = constellation_bpsk_impl();
		break;

	case QPSK_1_2:
	case QPSK_3_4:
		d_mapping = constellation_qpsk_impl();
		break;

	case QAM16_1_2:
	case QAM16_3_4:
		d_mapping = constellation_16qam_impl();
		break;

	case QAM64_2_3:
	case QAM64_3_4:
		d_mapping =  constellation_64qam_impl();
		break;

	default:
		printf("wrong encoding");
		assert(false);
		break;
	}
	int i;
	int index=0;
	for (i=0 ; i < data_size ; i++ )
	{
	 index=(int)input_items[i];
	 output_items[i*2]=d_mapping[index*2];
	 output_items[(i*2)+1]=d_mapping[(index*2)+1];
	}
	free(d_mapping);
}





