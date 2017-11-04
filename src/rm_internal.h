//
// File:        pf_internal.h
// Description: Declarations internal to the paged file component
// Authors:     Hugo Rivero (rivero@cs.stanford.edu)
//              Dallan Quass (quass@cs.stanford.edu)
//              Jason McHugh (mchughj@cs.stanford.edu)
//

#ifndef RM_INTERNAL_H_
#define RM_INTERNAL_H_

#include "rm.h"

/*
 * RM_FileHdr: Header structure for files
 */
struct RM_FileHdr {
	int recordSize; // size of each record
};

#endif /* RM_INTERNAL_H_ */
