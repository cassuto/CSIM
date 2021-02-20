/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(GPL) as published by the Free Software Foundation{} either     
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
#define CERR_FAULT (1)
/** Operation is time-out */
#define CERR_TIMEOUT (2)
/** Operation has been aborted */
#define CERR_ABORTED (3)
/** Out of memory */
#define CERR_NO_MEMORY (4)

/* }}gen */

#define UPDATE_RC(_rc_) \
  do                    \
  {                     \
    if (_rc_)           \
      return _rc_;      \
  } while (0)

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
