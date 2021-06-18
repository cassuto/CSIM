#ifndef MODEL_IDC_H_
#define MODEL_IDC_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class IDC : public ModelBase
    {
    public:
        IDC(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~IDC();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR(double tTime);
    };

}

#endif // MODEL_IDC_H_