/*
 * testing.c
 *
 *  Created on: Feb 24, 2017
 *      Author: elnag
 */
#ifndef ofdm
#define ofdm

int ofdm_carr_alloc(int *occupied_carriers, int *pilot_carriers,
		float *pilot_sympols, float *sync_words, int fft_len,
		int output_is_shifted, float *input, float *output,
		int sizeof_input_sym) {

	int i = 0;
	int j = 0;
	int sizeof_occ_carr = 48;
	int sizeof_pilot_carr = 4;
	//int sizeof_pilot_symp = 1016;
	int sizeof_sync_words = 512;


	//this part changes the zero values in the occupied_carriers to positive to indicate real positions in array
	for (i = 0; i < sizeof_occ_carr; i++) {

		if (occupied_carriers[i] < 0) {
			occupied_carriers[i] += fft_len;
		}
		if (occupied_carriers[i] > fft_len || occupied_carriers[i] < 0) {
			break;
		}
		if (output_is_shifted) {
			occupied_carriers[i] = (occupied_carriers[i] + fft_len / 2)
					% fft_len;
		}

	}
//this part changes the zero values in the pilot_carriers to positive to indicate real positions in array
	for (i = 0; i < sizeof_pilot_carr; i++) {

		if (pilot_carriers[i] < 0) {
			pilot_carriers[i] += fft_len;
		}
		if (pilot_carriers[i] > fft_len || pilot_carriers[i] < 0) {
			break;
		}
		if (output_is_shifted) {
			pilot_carriers[i] = (pilot_carriers[i] + fft_len / 2) % fft_len;
		}

	}

	// Copy Sync word
	for (i = 0; i < sizeof_sync_words; i++) {

		output[i] = sync_words[i];

	}
	// Copy data symbols
	float *out_data;
	out_data = 512 + output;

	long n_ofdm_symbols = 0; // Number of output items
	//int curr_set = 0;
	int symbols_to_allocate = 48;
	int symbols_allocated = 0;
	int k;
	for (i = 0; i < sizeof_input_sym; i = i + 2) {
		if (symbols_allocated == 0) {
			n_ofdm_symbols++;
		}
		k = occupied_carriers[symbols_allocated];
		k = k * 2;
		out_data[k] = input[i];
		out_data[k + 1] = input[i + 1];
		symbols_allocated++;
		if (symbols_allocated == symbols_to_allocate) {
			//curr_set = (curr_set + 1) % sizeof_occ_carr;
			symbols_to_allocate = 48;
			symbols_allocated = 0;
			out_data = out_data + ((fft_len) * 2);
		}
	}

	// Copy pilot symbols
	float *out_pilot;
	out_pilot = 512 + output;
	for (i = 0; i < n_ofdm_symbols; i++) {
		for (j = 0; j < 8; j = j + 2) {
			k = pilot_carriers[j / 2];
			k = k * 2;
			out_pilot[k] = pilot_sympols[j + (i * 8)];
			out_pilot[k + 1] = pilot_sympols[j + 1 + (i * 8)];
		}
		out_pilot = out_pilot + ((fft_len) * 2);
	}

	return 0;
}

#endif
