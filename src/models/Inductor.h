#ifndef INDUCTOR_H_
#define INDUCTOR_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class Inductor : public ModelBase
    {
    public:
        Inductor(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~Inductor();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR();

    private:
        double m_kZimag;
    };

}

#endif // INDUCTOR_H_