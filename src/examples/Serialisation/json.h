////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "rapidjson/prettywriter.h"
#include "rapidjson/filewritestream.h"

#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/filereadstream.h"


#include "gwRTTI/gwRTTI.h"

using namespace rapidjson;
using namespace gw::RTTI;



////////////////////////////////////////////////////////////////////////////////

extern void ExampleWrite( PrettyWriter< FileWriteStream >&, const TypeInfo*, void* );
extern void* ExampleRead( void*, const Document::GenericValue& );
