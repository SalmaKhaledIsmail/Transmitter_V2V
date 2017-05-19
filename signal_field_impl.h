/*
 * signal_field_impl.h
 *
 *  Created on: Feb 21, 2017
 *      Author: DotCom
 */

#ifndef SIGNAL_FIELD_IMPL_H_
#define SIGNAL_FIELD_IMPL_H_

#include "utils.h"




unsigned char * generate_signal_field( frame_param* signal_param, ofdm_param* signal_ofdm, frame_param* data_frame, ofdm_param* data_ofdm);

int get_bit(int b, int i);




#endif /* SIGNAL_FIELD_IMPL_H_ */
