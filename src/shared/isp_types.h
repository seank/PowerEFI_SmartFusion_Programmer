/*
 * isp_types.h
 */

#ifndef SRC_SHARED_ISP_TYPES_H_
#define SRC_SHARED_ISP_TYPES_H_

/* Matches defs in mss_sys_service header. */
typedef enum ISPCodes {
  MSS_SYS_PROG_AUTHENTICATE =  0,
  MSS_SYS_PROG_PROGRAM      =  1,
  MSS_SYS_PROG_VERIFY       =  2
} ISPCodes;

#endif /* SRC_SHARED_ISP_TYPES_H_ */
