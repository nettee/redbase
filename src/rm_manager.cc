//
// File:        rm_manager.cc
// Description: RM_Manager class implementation
// Authors:     nettee (nettee.liu@gmail.com
//

#include "rm.h"

RM_Manager::RM_Manager(PF_Manager &pfm)
{
	pfMgr = pfm;
}

RM_Manager::~RM_Manager()
{

}

RC RM_Manager::CreateFile(const char *fileName, int recordSize)
{

}

RC RM_Manager::DestroyFile(const char *fileName)
{

}

RC RM_Manager::OpenFile(const char *fileName, RM_FileHandle& fileHandle)
{

}

RC RM_Manager::CloseFile(RM_FileHandle &fileHandle)
{

}

