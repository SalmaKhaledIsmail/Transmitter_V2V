/*
 * signal_field_impl.c
 *
 *  Created on: Feb 21, 2017
 *      Author: DotCom
 */

#include "signal_field_impl.h"

int get_bit(int b, int i) {
	return (b & (1 << i) ? 1 : 0);
}

unsigned char * generate_signal_field(frame_param* signal_param, ofdm_param* signal_ofdm,
		frame_param* data_frame, ofdm_param* data_ofdm) {

	unsigned char * out = (unsigned char *) malloc(sizeof(char) * 48); //output frame of 48 bits (24*2) 0-47
	//data bits of the signal header
	char *signal_header = (char *) malloc(sizeof(char) * 24);

	//signal header after...
	//convolutional encoding
	char *encoded_signal_header = (char *) malloc(sizeof(char) * 48);
	//interleaving
	char *interleaved_signal_header = (char *) malloc(sizeof(char) * 48);

	int length = data_frame->psdu_size;


	// first 4 bits represent the modulation and coding scheme
	signal_header[0] = get_bit(data_ofdm->rate_field, 3);
	signal_header[1] = get_bit(data_ofdm->rate_field, 2);
	signal_header[2] = get_bit(data_ofdm->rate_field, 1);
	signal_header[3] = get_bit(data_ofdm->rate_field, 0);
	// 5th bit is reserved and must be set to 0
	signal_header[4] = 0;
	// then 12 bits represent the length
	signal_header[5] = get_bit(length, 0);
	signal_header[6] = get_bit(length, 1);
	signal_header[7] = get_bit(length, 2);
	signal_header[8] = get_bit(length, 3);
	signal_header[9] = get_bit(length, 4);
	signal_header[10] = get_bit(length, 5);
	signal_header[11] = get_bit(length, 6);
	signal_header[12] = get_bit(length, 7);
	signal_header[13] = get_bit(length, 8);
	signal_header[14] = get_bit(length, 9);
	signal_header[15] = get_bit(length, 10);
	signal_header[16] = get_bit(length, 11);
	//18-th bit is the parity bit for the first 17 bits
	int sum = 0;
	int i;
	for (i = 0; i < 17; i++) {
		if (signal_header[i]) {
			sum++;
		}
	}
	signal_header[17] = sum % 2;

	// last 6 bits must be set to 0
	for (i = 0; i < 6; i++) {
		signal_header[18 + i] = 0;
	}

	ofdm_param_intialization(BPSK_1_2, signal_ofdm);
	frame_param_intialization(signal_ofdm, signal_param, 0);

	// convolutional encoding (scrambling is not needed)
	convolutional_encoding(signal_header, encoded_signal_header, signal_param);
	// interleaving
	interleave(encoded_signal_header, out, signal_param, signal_ofdm);

	//int encodedSize = sizeof(encoded_signal_header);

	free(signal_header);
	free(encoded_signal_header);
	free(interleaved_signal_header);

	return out;
}
