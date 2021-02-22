#ifndef MODEL_VDC_H_
#define MODEL_VDC_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class VDC : public ModelBase
    {
    public:
        VDC(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~VDC();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC();
        virtual int iterateTR();
    };

}

#endif // MODEL_VDC_H_