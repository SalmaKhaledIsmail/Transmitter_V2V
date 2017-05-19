/*
 * Mapper.h
 *
 *  Created on: Feb 7, 2017
 *      Author: salma
 */

#ifndef MAPPER_H_
#define MAPPER_H_


/*
 * Mapper_Imp.h
 *
 *  Created on: Feb 6, 2017
 *      Author: salma
 */

#ifndef MAPPER_IMP_H_
#define MAPPER_IMP_H_



#include "utils.h"
/* The Number of data carriers */
#define Data_Carriers 48

/* Variables that will be used in the general work function of the Mapper */

/** This enumeration is used to define the encoding type used */




unsigned char * mapper_general_work_function(const unsigned char* psdu,int psdu_length,ofdm_param* d_ofdm,frame_param * frame);
bool msg_get();


#endif /* MAPPER_IMP_H_ */

#endif /* MAPPER_H_ */
