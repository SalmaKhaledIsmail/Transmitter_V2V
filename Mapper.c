/*
 * Mapper.c
 *
 *  Created on: Feb 9, 2017
 *      Author: salma
 */
#include "Mapper.h"
#include <inttypes.h>
unsigned char* mapper_general_work_function(const unsigned char* psdu,
		int psdu_length, ofdm_param* d_ofdm, frame_param * frame) {

	char * d_symbols;

	int d_symbols_offset = 0;

	int noutput = 32768; /** I found this value in the gnu radio but I don't understand where it came from **/

	int d_symbols_len = 0;

	/** Constructing and initializing an OFDM struct **/

	/** Printing the initial value of d_symbols_offset &&  the value of d_symbols_length **/

	printf("MAPPER called offset: %i\n", d_symbols_offset);
	printf("length: %i\n", d_symbols_len);


	while (!d_symbols_offset) {

		/** This msg_get function is used to check if there is actually a message received by the physical layer returns true-> if yes returns false if NO */
		if (!msg_get()) {
			return 0;
		}

		printf("MAPPER: received new message \n");

		if (frame->n_sym > MAX_SYM) {

			printf("packet too large, maximum number of symbols is %i\n ",
					MAX_SYM);
			return 0;
		}

		//allocate memory for modulation steps

		char *data_bits = (char*) calloc(frame->n_data_bits, sizeof(char));
		if (data_bits == NULL) {
			printf("Not enough memory for data_bits \n");
		}
		char *scrambled_data = (char*) calloc(frame->n_data_bits, sizeof(char));

		if (scrambled_data == NULL) {
			printf("Not enough memory for scrambled_data\n");
		}
		char *encoded_data = (char*) calloc(frame->n_data_bits * 2,
				sizeof(char));
		if (encoded_data == NULL) {
			printf("Not enough memory for encoded_data \n");
		}
		char *punctured_data = (char*) calloc(frame->n_encoded_bits,
				sizeof(char));
		if (punctured_data == NULL) {
			printf("Not enough memory for punctured_data \n");
		}
		char *interleaved_data = (char*) calloc(frame->n_encoded_bits,
			sizeof(char));
		if (interleaved_data == NULL) {
			printf("Not enough memory for interleaved_data \n");
		}
		char *symbols = (char*) calloc((frame->n_encoded_bits / d_ofdm->n_bpsc),
				sizeof(char));
		if (symbols == NULL) {
			printf("Not enough memory for symbols \n");
		}

		//generate the WIFI data field, adding service field and pad bits

		generate_bits(psdu, data_bits, frame);

		// scrambling

		// Initial state of the scrambler is set  to : 93
		static uint8_t scrambler = 93;

		scramble(data_bits, scrambled_data, frame, scrambler);

		if (scrambler > 127) {
			scrambler = 1;
		}

		// reset tail bits

		reset_tail_bits(scrambled_data, frame);
		// encoding

		convolutional_encoding(scrambled_data, encoded_data, frame);
		// puncturing

		puncturing(encoded_data, punctured_data, frame, d_ofdm);
		// interleaving

		interleave(punctured_data, interleaved_data, frame, d_ofdm);
		// one byte per symbol

		split_symbols(interleaved_data, symbols, frame, d_ofdm);

		// calculate the length of the processed data

		d_symbols_len = frame->n_sym * 48;

		// This is the array containing  the final output from the mapper without offset

		d_symbols = (char*) calloc(d_symbols_len, 1);

		memcpy(d_symbols, symbols, d_symbols_len);

		// free the allocated memory

		free(data_bits);
		free(scrambled_data);
		free(encoded_data);
		free(punctured_data);
		free(interleaved_data);
		free(symbols);

		break;

	}

	int i = min(noutput, d_symbols_len - d_symbols_offset);

	// Final output array

	unsigned char * out = (unsigned char*) calloc(i, sizeof(char));

	// if there was an offset copy it to the out data

	memcpy(out, d_symbols + d_symbols_offset, i);

	d_symbols_offset += i;

	if (d_symbols_offset == d_symbols_len) {
		d_symbols_offset = 0;
		free(d_symbols);
		d_symbols = 0;
	}

	return out;

}

//2-msg_get()
/** This function is used to check if the message is valid ,but yet NOT COMPLETED */

bool msg_get(unsigned char * psdu, int psdu_length) {

	// This function is not implemented yet
	return true;

}

/* Function to print the output data from the mapper can be used after the split symbols */
void print_Output_bits(char* output, frame_param* frame, ofdm_param* d_ofdm) {
	int i;
	int k;
	int symbols_length = frame->n_sym * 48;
	for (i = 0; i < symbols_length; i++) {
		for (k = 0; k < d_ofdm->n_bpsc; k++) {
			printf("%i", output[i]);
		}
	}
}
