/*
 * rpm.h
 *
 *  Created on: Jul 15, 2019
 *      Author: sbaveja
 */

#ifndef INCLUDE_RPM_H_
#define INCLUDE_RPM_H_

#define TIMEFACTOR 1.9520527E-3  //64 (number of cycles per count) * 1/32786 (period for the clock)

void initRPMTimer();

#endif /* INCLUDE_RPM_H_ */
