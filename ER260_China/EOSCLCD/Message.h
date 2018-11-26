#ifndef MESSAGE_H
#define MESSAGE_H

#if defined(CASE_FORHANZI)

//#include "MessageCH.h"
#include "MessageCH_C.h"
#include "MessageCH_D16.h"
#include "MessageCH_P32.h"
#else

#if defined(CASE_VIETNAM)//т╫до

#if defined(CHAR_6X8)
#include "MessageVN_C.h"
#include "MessageVN_D21.h"
#include "MessageVN_P32.h"
#else
#include "MessageVN_260.h"
#endif

#elif defined(CASE_SPANISH)

#include "MessageSP.h"

#elif defined(CHAR_6X8)

//#include "MessageEN_21N.h"
#include "MessageEN_C.h"
#include "MessageEN_D21.h"
#include "MessageEN_P32.h"

#else

#include "MessageEN_260.h"
#include "MessageEN_P32.h"

#endif

#endif

#endif
