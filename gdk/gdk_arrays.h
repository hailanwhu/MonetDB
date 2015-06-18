#ifndef _GDK_ARRAYS_H
#define _GDK_ARRAYS_H

#include "gdk.h"
#include <math.h>

typedef enum errors {
	general_error,
    new_bat,
    dimension_type,
    value_type
} errors;

gdk_return gdk_error_msg(errors errorCode, const char* funcName, const char *msg);

#define dimensionCharacteristics(TPE, dimensionBAT, min, max, step, elementRepeats, groupRepeats) \
do {\
    TPE *vls; \
    BUN i; \
    vls = (TPE*)Tloc(dimensionBAT, BUNfirst(dimensionBAT)); \
    *min = vls[0]; \
    *step = vls[BATcount(dimensionBAT)-1]; \
    *max = vls[BATcount(dimensionBAT)-2]; \
\
    *elementRepeats = *groupRepeats = 0; \
    vls = (TPE*)Tloc(dimensionBAT, BUNfirst(dimensionBAT)); \
\
    for(i=0; i<BATcount(dimensionBAT)-2; i++) { /*last element is the step and at least one element is max*/\
        if(vls[i] != *min) \
            break; \
        (*elementRepeats)++; \
    } \
    *groupRepeats = BATcount(dimensionBAT)-i-1; \
\
	/*a zero step shows a single element in the dimension but leads to infinite loops and we do not want that*/ \
	if(!(*step)) \
		*step = 1; \
} while(0)

#define dimensionElement(min, max, step, elementRepeats, oid) \
    ({ \
        long elementsNum = floor((max-min)/step) + 1; \
        long elementsPerGroup = elementsNum*elementRepeats; \
        long elementInGroup = floor((oid%elementsPerGroup)/elementRepeats); \
/*fprintf(stderr, "dimension element at %d is %d\n", (int)oid, (int)(min+elementInGroup*step)); */\
        min+elementInGroup*step; \
    })

#define dimensionElementsNum(min, max, step)\
    ({\
        long num = 1; \
        if(!step) { \
            if(min!=max) { \
                GDKerror("dimensionElementsNum: step is 0 but min and max are not equal\n"); \
                num = 0; \
            } \
        } else \
            num = floor((max-min)/step) + 1; \
        num; \
    })

/*find the position in the dimension indices (no repetitions) of the given value*/
#define dimensionFndValuePos(value, min, step) fmod((value-min), step)? BUN_NONE : (BUN)(value-min)/step
/*find the position in the dimension indices (no repetitions) of the  given value
 * or the position of the index that is closest to the given value and greater than it*/
#define dimensionFndGreaterValuePos(value, min, step, eq) \
	({\
		BUN pos = (BUN)(value-min)/step; \
		fmod((value-min), step) ? ++pos : (pos +(1-eq)); \
	})

/*find the position in the dimension indices (no repetitions) of the  given value
 * or the position of the index that is closest to the given value and smaller than it*/
#define dimensionFndLowerValuePos(value, min, step, eq) \
	({\
		BUN pos = (BUN)(value-min)/step; \
		if(value < min) \
			pos = -2; \
		fmod((value-min), step) ? pos : (pos - (1-eq)); \
	})

BUN dimension_void_replace_bat(BAT *resBAT, BAT *oidsBAT, BAT *dimensionBAT, bit force);


BAT* materialiseDimensionBAT(BAT* dimensionBAT);
BUN dimensionBATsize(BAT* dimensionBAT);
BUN dimensionBATelementsNum(BAT* dimensionBAT);

gdk_return dimensionBATsubselect(BAT** outBAT, BAT *dimensionBAT, BAT *candsBAT, const void *low, const void *high, int includeLow, int includeHigh, int anti);
gdk_return BATmbrsubselect(BAT **outBAT, BAT *dimensionBAT, BAT *oidsBAT, BAT *candsBAT);
gdk_return dimensionBATproject(BAT** outBAT, BAT* oidsBAT, BAT* dimensionBAT);
BAT* dimensionBATproject_wrap(BAT* oidsBAT, BAT* dimensionBAT);
gdk_return BATmbrproject(BAT **outBAT, BAT *b, BAT *oidsToProjectBAT, BAT *subselectBAT);
gdk_return dimensionBATgroup(BAT **groups, BAT **extents, BAT **histo, BAT *dimensionBAT, BAT *g, BAT *e, BAT *h);
gdk_return dimensionBATgroupavg(BAT **bnp, BAT **cntsp, BAT *b, BAT *g, BAT *e, BAT *s, int tp, int skip_nils, int abort_on_error);
gdk_return dimensionBATsubjoin(BAT **outBATl, BAT **outBATr, BAT *dimensionBATl, BAT *dimensionBATr, BAT *sl, BAT *sr, int nil_matches, BUN estimate);
#endif /* _GDK_ARRAYS_H */
