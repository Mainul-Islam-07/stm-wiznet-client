/*
 * config.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Qbits
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_
/************ LANE SELECT INFO*********************

Lane select will be 2- then ip will be 10.150.0.191
Lane select will be 3- then ip will be 10.150.0.192
Lane select will be 4- then ip will be 10.150.0.193
Lane select will be 5- then ip will be 10.150.0.194
Lane select will be 6- then ip will be 10.150.0.195
Lane select will be 7- then ip will be 10.150.0.196
Gateway- 10.150.0.1 (For above all)
Lane_select will be 0- then ip will be 192.168.0.14 and gateway will be 192.168.0.1
Subnet mask- 255.255.255.0  (For all)


*************************************************/
#define LANE_SELECT 0


#if LANE_SELECT == 2

#define LANE_2 1

#define LANE_3 0

#define LANE_4 0

#define LANE_5 0

#define LANE_6 0

#define LANE_7 0


#elif LANE_SELECT == 3

#define LANE_2 0

#define LANE_3 1

#define LANE_4 0

#define LANE_5 0

#define LANE_6 0

#define LANE_7 0


#elif LANE_SELECT == 4

#define LANE_2 0

#define LANE_3 0

#define LANE_4 1

#define LANE_5 0

#define LANE_6 0

#define LANE_7 0


#elif LANE_SELECT == 5

#define LANE_2 0

#define LANE_3 0

#define LANE_4 0

#define LANE_5 1

#define LANE_6 0

#define LANE_7 0


#elif LANE_SELECT == 6

#define LANE_2 0

#define LANE_3 0

#define LANE_4 0

#define LANE_5 0

#define LANE_6 1

#define LANE_7 0


#elif LANE_SELECT == 7

#define LANE_2 0

#define LANE_3 0

#define LANE_4 0

#define LANE_5 0

#define LANE_6 0

#define LANE_7 1

#else

#define LANE_2 0

#define LANE_3 0

#define LANE_4 0

#define LANE_5 0

#define LANE_6 0

#define LANE_7 0

#endif // lane selecting guard

#endif /* INC_CONFIG_H_ */
