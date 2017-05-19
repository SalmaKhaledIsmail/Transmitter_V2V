/*
 * utils.h
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

#ifndef UTILS_H_
#define UTILS_H_

/*
 * utils.h
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

#include "IEEE802_11_Common_Variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#define MAX_PAYLOAD_SIZE 1500
#define MAX_PSDU_SIZE (MAX_PAYLOAD_SIZE + 28) // MAC, CRC
#define MAX_SYM (((16 + 8 * MAX_PSDU_SIZE + 6) / 24) + 1)
#define MAX_ENCODED_BITS ((16 + 8 * MAX_PSDU_SIZE + 6) * 2 + 288)

//#define dout d_debug && std::cout
;
/**----------------------------------------------------------ofdm_param_implementation--------------------------------------------**/
typedef enum{
    BPSK_1_2  = 0,
    BPSK_3_4  = 1,
    QPSK_1_2  = 2,
    QPSK_3_4  = 3,
    QAM16_1_2 = 4,
    QAM16_3_4 = 5,
    QAM64_2_3 = 6,
    QAM64_3_4 = 7,
}Encoding ;

typedef struct {

	// data rate
	Encoding encoding;
	// rate field of the SIGNAL header
	char rate_field;
	// number of coded bits per sub carrier
	int n_bpsc;
	// number of coded bits per OFDM symbol
	int n_cbps;
	// number of data bits per OFDM symbol
	int n_dbps;

} ofdm_param;

/** This function is used to initialize the parameters of the ofdm */
void ofdm_param_intialization(Encoding, ofdm_param*);
/** This function print the values of the ofdm param */
void print_ofdm_param(const ofdm_param*);

/**----------------------------------------------------------frame_param_implementation-------------------------------------------**/
typedef struct {

	// PSDU size in bytes
	int psdu_size;
	// number of OFDM symbols (17-11)
	int n_sym;
	// number of padding bits in the DATA field (17-13)
	int n_pad;
	int n_encoded_bits;
	// number of data bits, including service and padding (17-12)
	int n_data_bits;
} frame_param;

/** This function is used to initialize the parameters of the ofdm */
void frame_param_intialization(ofdm_param*,frame_param*,int );
/** This function is used to print the value of the frame param */
void print_frame_param(const frame_param*);

/**-----------------------------------------------------------PSDU_Processing-----------------------------------------------------**/

void scramble(const char *input, char *out, frame_param* frame, char initial_state);

void reset_tail_bits(char *scrambled_data, frame_param* frame);

void convolutional_encoding(const char *input, char *out, frame_param* frame);

void puncturing(const char *input, char *out, frame_param* frame, ofdm_param* ofdm);

/** N.B: Their is no default arguments in C,so I removed the parameter (bool reverse) since it is always false and modified
 *  the implementation inside the function **/

void interleave(const char *input, char *out, frame_param* frame, ofdm_param* ofdm);

void split_symbols(const char *input, char *out, frame_param* frame, ofdm_param* ofdm);

void generate_bits(const unsigned char *psdu, char *data_bits, frame_param* frame);


#endif /* UTILS_H_ */

