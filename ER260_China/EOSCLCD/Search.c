#define  SEARCH_C 4
#include "king.h"
#include "exthead.h"
#include "exth.h"


/***************************************************************/

/* binary serach function  */
/* return 0 if not found else ENTRYNUMBER + 1 and set RamOffSet to */
/* Start location */

WORD BinarySearch(void *record ,WORD recordsize,BYTE length ,short Max)
{
    short result, low;
    WORD  mid;
    UnLong start ;   /* unsigned long start; */
	char  sRadom[8];				//ccr20130905

	if (length>sizeof(sRadom))		//ccr20130905
		length = sizeof(sRadom);	//ccr20130905

    if (Max)
    {
		start = RamOffSet;
		low = 0;
		Max--;
		while (low <= Max)
		{
		    mid = Max;      /* Note that Max and low are Signed Int? */
		    mid += low;
		    mid >>= 1;
		    RamOffSet =(UnLong)mid * recordsize + start;
		    ReadRam(sRadom, length);
		    result = CompareBCDValue(record, sRadom, length);
		    if (result < 0)
		    {
				Max = mid;
				Max-- ;
		    }
		    else if (result)
				{
					low = mid;
					low++;
			    }
			    else
			    {
				    RamOffSet -= length;  /* set offset to start */
					return (mid + 1);   /* to support also entry 0 */
			    }
		}
	    RamOffSet -= length;
		if (result > 0)
		  RamOffSet += recordsize;
    }
    return 0;
}

