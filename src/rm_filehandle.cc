#include "rm_internal.h"

RM_FileHandle::RM_FileHandle()
{
	bFileHandleOpen = FALSE;
}

RM_FileHandle::~RM_FileHandle()
{
	// do nothing
}

RC RM_FileHandle::InsertRec(const char *pData, RID &rid)
{
	if (pData == null) {
		return RM_DATA_NULL;
	}

	return RC_OK;
}



