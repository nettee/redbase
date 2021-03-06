//
// rm.h
//
//   Record Manager component interface
//
// This file does not include the interface for the RID class.  This is
// found in rm_rid.h
//

#ifndef RM_H
#define RM_H

// Please DO NOT include any files other than redbase.h and pf.h in this
// file.  When you submit your code, the test program will be compiled
// with your rm.h and your redbase.h, along with the standard pf.h that
// was given to you.  Your rm.h, your redbase.h, and the standard pf.h
// should therefore be self-contained (i.e., should not depend upon
// declarations in any other file).

#include <vector>
// Do not change the following includes
#include "redbase.h"
#include "rm_rid.h"
#include "pf.h"

//
// RM_Record: RM Record interface
//
class RM_Record {
public:
    RM_Record ();
    ~RM_Record();

    // Return the data corresponding to the record.  Sets *pData to the
    // record contents.
    RC GetData(char *&pData) const;

    // Return the RID associated with the record
    RC GetRid (RID &rid) const;
};

/*
 * RM_FileHdr: Header structure for files
 */
struct RM_FileHdr {
	int recordSize; // size of each record
};

//
// RM_FileHandle: RM File interface
//
class RM_FileHandle {
	friend class RM_Manager;
public:
    RM_FileHandle ();
    ~RM_FileHandle();

    // Given a RID, return the record
    RC GetRec     (const RID &rid, RM_Record &rec) const;

    RC InsertRec  (const char *pData, RID &rid);       // Insert a new record

    RC DeleteRec  (const RID &rid);                    // Delete a record
    RC UpdateRec  (const RM_Record &rec);              // Update a record

    // Forces a page (along with any contents stored in this class)
    // from the buffer pool to disk.  Default value forces all pages.
    RC ForcePages (PageNum pageNum = ALL_PAGES);


private:

    PF_FileHandle pf_fileHdl; // file handle of the paged file
    RM_FileHdr hdr; // file header
    std::vector<int> pinnedPages; // pageNums of pages pinned in buffer
    int bFileHandleOpen; // File open flag
};

//
// RM_FileScan: condition-based scan of records in the file
//
class RM_FileScan {
public:
    RM_FileScan  ();
    ~RM_FileScan ();

    RC OpenScan  (const RM_FileHandle &fileHandle,
                  AttrType   attrType,
                  int        attrLength,
                  int        attrOffset,
                  CompOp     compOp,
                  void       *value,
                  ClientHint pinHint = NO_HINT); // Initialize a file scan
    RC GetNextRec(RM_Record &rec);               // Get next matching record
    RC CloseScan ();                             // Close the scan
};

//
// RM_Manager: provides RM file management
//
class RM_Manager {
public:
    RM_Manager    (PF_Manager &pfm);
    ~RM_Manager   ();

    RC CreateFile (const char *fileName, int recordSize);
    RC DestroyFile(const char *fileName);
    RC OpenFile   (const char *fileName, RM_FileHandle &fileHandle);

    RC CloseFile  (RM_FileHandle &fileHandle);

private:
    PF_Manager *pPfMgr;
};

//
// Print-error function
//
void RM_PrintError(RC rc);

#define RM_FILE_HANDLE_OPEN (START_RM_WARN + 0) // file handle is open
#define RM_FILE_HANDLE_CLOSED (START_RM_WARN + 1) // file handle is closed
#define RM_EOF (START_RM_WARN + 2)

#define RM_RECORD_SIZE_TOO_LARGE (START_RM_ERR - 0) // record size larger than page size
#define RM_HEADER_WRITE (START_RM_ERR - 1) // incomplete write to header
#define RM_UNIX (START_RM_ERR - 2) // Unix error
#define RM_DATA_NULL (START_RM_ERR - 3) // data (type char *) is NULL

#endif
