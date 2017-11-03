//
// File:        rm_manager.cc
// Description: RM_Manager class implementation
// Authors:     nettee (nettee.liu@gmail.com
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include "rm.h"

RM_Manager::RM_Manager(PF_Manager &pfm)
{
	pPfMgr = &pfm;
}

RM_Manager::~RM_Manager()
{

}

/*
 * CreateFile
 *
 * Create a new RM file named fileName
 * In:  fileName - name of file to create
 * Ret: RM return code
 */
RC RM_Manager::CreateFile(const char *fileName, int recordSize)
{
	RC rc;

	if (recordSize > PF_PAGE_SIZE) {
		return RM_RECORD_SIZE_TOO_LARGE;
	}

	// Create a paged file
	if ((rc = pPfMgr->CreateFile(fileName)) != 0) {
		return rc;
	}

//	PF_FileHandle pf_fileHandle;
//	if ((rc = pfMgr.OpenFile(fileName, pf_fileHandle)) != 0) {
//		return rc;
//	}
//
//	// Create a new page as header page
//	PF_PageHandle pf_pageHandle;
//	if ((rc = pf_fileHandle.AllocatePage(pf_pageHandle)) != 0) {
//		return rc;
//	}
//	char *pData;
//	if ((rc = pf_pageHandle.GetData(pData)) != 0) {
//		return rc;
//	}
//	int pageNum;
//	if ((rc = pf_pageHandle.GetPageNum(pageNum)) != 0) {
//		return rc;
//	}
//
//	// Initialize the file header
//	char hdrBuf[sizeof(RM_FileHdr)];
//	memset(hdrBuf, 0, sizeof(RM_FileHdr));
//
//	RM_FileHdr *hdr = (RM_FileHdr *)hdrBuf;
//	hdr->recordSize = recordSize;
//	memcpy(pData, hdrBuf, sizeof(RM_FileHdr));
//
//	// Write the file header to the first page
//	if ((rc = pf_fileHandle.MarkDirty(pageNum)) != 0) {
//		return rc;
//	}
//	if ((rc = pf_fileHandle.ForcePages(pageNum)) != 0) {
//		return rc;
//	}
//
//	if ((rc = pfMgr.CloseFile(pf_fileHandle)) != 0) {
//		return rc;
//	}

	// Return ok
	return 0;
}

/*
 * DestroyFile
 *
 * Delete an RM file named fileName (fileName must exist and not be open)
 * In:  fileName - name of file to delete
 * Ret: RM return code
 */
RC RM_Manager::DestroyFile(const char *fileName)
{
	return pPfMgr->DestroyFile(fileName);
}

/*
 * OpenFile
 *
 * Open the RM file named fileName. It is possible to open an RM file more than
 * once, as in the PF component. However, each call to the OpenFile method
 * creates a new instance of the open file. If a file has more than one opened
 * instance, then each instance of the open file should read but not write.
 * If a file is modified while opened more than once, the integrity of the file
 * or the RM component is not guaranteed.
 *
 * In:  fileName - name of file to open
 * Out: fileHandle -
 * Ret: RM return code
 */
RC RM_Manager::OpenFile(const char *fileName, RM_FileHandle& fileHandle)
{
	RC rc;

	// Ensure the file handle is not already open
	if (fileHandle.bFileHandleOpen) {
		return RM_FILE_HANDLE_OPEN;
	}

	// Open a paged file
	if ((rc = pPfMgr->OpenFile(fileName, fileHandle.pf_fileHandle)) != 0) {
		return rc;
	}

	// TODO read the header page

	fileHandle.bFileHandleOpen = TRUE;

	// Return ok
	return 0;
}

/*
 * CloseFile
 *
 * Close the open file instance referred to by fileHandle.
 * The file should have been opened with OpenFile().
 *
 * In:  fileHandle - handle of file to close
 * Out: fileHandle - no longer refers to an open file
 * 					 this function modifies local var's in fileHandle
 * Ret: RM return code
 */
RC RM_Manager::CloseFile(RM_FileHandle &fileHandle)
{
	RC rc;

	// Ensure the file handle is open
	if (!fileHandle.bFileHandleOpen) {
		return RM_FILE_HANDLE_CLOSED;
	}

	// TODO write all pages

	// Closed the paged file
	if ((rc = pPfMgr->CloseFile(fileHandle.pf_fileHandle)) != 0) {
		return rc;
	}

	fileHandle.bFileHandleOpen = FALSE;

	// Return ok
	return 0;
}

