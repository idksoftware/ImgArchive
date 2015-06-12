/*
imageContainer * MetadataObject.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: wzw7yn
 */
#include "DBDefines.h"
#include "MetadataObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

MetadataObject::MetadataObject() : MTRow(new MetadataSchema){

}

MetadataObject::~MetadataObject() {
	//printf("+++ MetadataObject Deleting +++\n");
}

} /* namespace simplearchive */
