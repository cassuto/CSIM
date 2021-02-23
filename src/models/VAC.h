#ifndef MODEL_VAC_H_
#define MODEL_VAC_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class VAC : public ModelBase
    {
    public:
        VAC(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~VAC();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR();

    private:
        MComplex m_E;
    };

}

#endif // MODEL_VAC_H_