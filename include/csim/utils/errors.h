/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#ifndef CSIM_ERRORS_H_
#define CSIM_ERRORS_H_

/*
 * The following Macro definitions participate in automatic generation of error
 * messages data ( errors-generated.h ), which begins with gen\{\{ and ends with \}\}gen
 * comment.
 */

/* gen{{ */

/** Operation has been successfully completed */
#define CERR_SUCCEEDED (0)
/** Operation has been failed */
#define CERR_FAULT (-1)
/** Operation is time-out */
#define CERR_TIMEOUT (-2)
/** Operation has been aborted */
#define CERR_ABORTED (-3)
/** Out of memory */
#define CERR_NO_MEMORY (-4)
/** Max number of iterations exceeded */
#define CERR_MAX_ITERATION_EXCEEDED (-5)
/** Duplicated reference of component */
#define CERR_DUP_COMPONENT_REFERENCE (-6)
/** Non Existent reference of component */
#define CER_NO_SUCH_COMPONENT_REFERENCE (-7)
/** Invalid index of terminal */
#define CERR_INVALID_TERML_INDEX (-8)
/** There are some terminals floating */
#define CERR_FLOATING_TERMINALS (-9)
/** No such property */
#define CERR_NO_SUCH_PROPERTY (-10)
/** Invalid range */
#define CERR_INVALD_RANGE (-11)
/** Singular Matrix */
#define CERR_SINGULAR_MATRIX (-12)
/** No target to analyze */
#define CERR_NO_TARGET_TO_ANALYZE (-13)
/** Failed to set the integral order */
#define CERR_SET_INTEGRAL_ORDER (-14)
/** Failed to set the integral step */
#define CERR_SET_INTEGRAL_STEP (-15)
/** No mdl specified */
#define CERR_NO_MDL (-16)
/** Invalid parameter */
#define CERR_INVALID_PARAMETER (-17)
/** Non convergence */
#define CERR_NON_CONVERGENCE (-18)
/** Implemented */
#define CERR_UNIMPLEMENTED (-19)
/** Invalid source name */
#define CERR_INVALID_SRCNAME (-20)
/** Invalid space of sweep variable */
#define CERR_INVALID_SWEEP_SPACE (-21)
/** Missing required parameters */
#define CERR_MSSING_PARAMETERS (-22)
/** Failed to open file */
#define CERR_OPEN_FILE (-23)
/** Parse file */
#define CERR_PARSE_FILE (-24)
/** Parameter is undefined in scopes */
#define CERR_IDENTIFIER_UNDEFINED (-25)
/** Failed to parse algebraic expression */
#define CERR_PARSE_ALGEBRAIC_EXPRESSION (-26)
/** Failed to create mdl */
#define CERR_CREATE_MDL (-27)
/** Duplicated identifier of device model */
#define CERR_DUP_MODEL_ID (-28)
/** Duplicated identifier of property mdl */
#define CERR_DUP_MDL_ID (-29)
/** Property of property mdl is existing */
#define CERR_MDL_PROP_EXISTING (-28)
/** No such device model */
#define CERR_NO_SUCH_MODEL (-29)
/** Property model is existing */
#define CERR_MDL_EXISTING (-30)
/** No such mdl */
#define CERR_NO_SUCH_MDL (-31)
/** Invalid branch index */
#define CERR_INVALID_BRANCH_INDEX (-32)
/** Data type of value mismatch */
#define CERR_TYPE_MISMATCH (-33)

/** Unknown */
#define CERR_UNKNOWN (-65536)

/* }}gen */

#define UPDATE_RC(_rc_)    \
    do                     \
    {                      \
        int rc__ = (_rc_); \
        if (rc__)          \
            return rc__;   \
    } while (0)

#define CSIM_OK(_rc_) \
    ((_rc_) == 0 ? true : false)

#define CSIM_FAILED(_rc_) \
    ((_rc_) ? true : false)

namespace csim
{
    class ErrorInfo
    {
    public:
        /** Pointer to the short message string. */
        const char *msgShort;
        /** Pointer to the full message string. */
        const char *msgFull;
        /** Pointer to the define string. */
        const char *msgDefine;
        /** Status code number. */
        int code;

        static const ErrorInfo &get(int rc);
    };
}

#endif //!defined(CSIM_ERRORS_H_)
