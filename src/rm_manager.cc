//
// File:        rm_manager.cc
// Description: RM_Manager class implementation
// Authors:     nettee (nettee.liu@gmail.com
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include "rm_internal.h"

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

	// Create and open a paged file
	PF_FileHandle pf_fileHdl;
	if ((rc = pPfMgr->CreateFile(fileName)) ||
			(rc = pPfMgr->OpenFile(fileName, pf_fileHdl))) {
		return rc;
	}

	// Create a new page as header page
	PF_PageHandle pf_pageHandle;
	char *pData;
	PageNum pageNum;
	if ((rc = pf_fileHdl.AllocatePage(pf_pageHandle)) ||
			(rc = pf_pageHandle.GetData(pData)) ||
			(rc = pf_pageHandle.GetPageNum(pageNum))) {
		return rc;
	}

	// Initialize the file header
	char hdrBuf[sizeof(RM_FileHdr)];
	memset(hdrBuf, 0, sizeof(RM_FileHdr));

	RM_FileHdr *hdr = (RM_FileHdr *)hdrBuf;
	hdr->recordSize = recordSize;
	memcpy(pData, hdrBuf, sizeof(RM_FileHdr));

	// Write the file header to the first page
	if ((rc = pf_fileHdl.MarkDirty(pageNum)) ||
			(rc = pf_fileHdl.UnpinPage(pageNum))) {
		return rc;
	}

	if ((rc = pPfMgr->CloseFile(pf_fileHdl)) != 0) {
		return rc;
	}

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

	// Open the paged file
	if ((rc = pPfMgr->OpenFile(fileName, fileHandle.pf_fileHdl)) != 0) {
		return rc;
	}

	// Read the file header
	PF_PageHandle headerPageHdl;
	PageNum headerPageNum;
	char *pData;
	if ((rc = fileHandle.pf_fileHdl.GetFirstPage(headerPageHdl)) ||
			(rc = headerPageHdl.GetPageNum(headerPageNum)) ||
			(rc = headerPageHdl.GetData(pData))) {
		return rc;
	}

	memcpy((char *)&fileHandle.hdr, pData, sizeof(RM_FileHdr));

	if ((rc = fileHandle.pf_fileHdl.MarkDirty(headerPageNum))) {
		return rc;
	}
	fileHandle.pinnedPages.push_back(headerPageNum);

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

	// Unpin all pages
	for (std::vector<int>::iterator it = fileHandle.pinnedPages.begin();
			it != fileHandle.pinnedPages.end();
			++it) {
		fileHandle.pf_fileHdl.UnpinPage(*it);
	}

	// Closed the paged file
	if ((rc = pPfMgr->CloseFile(fileHandle.pf_fileHdl)) != 0) {
		return rc;
	}

	fileHandle.bFileHandleOpen = FALSE;

	// Return ok
	return 0;
}

