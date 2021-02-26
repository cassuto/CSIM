#ifndef MODEL_IAC_H_
#define MODEL_IAC_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class IAC : public ModelBase
    {
    public:
        IAC(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~IAC();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR(double tTime);

    private:
        MComplex m_I;
        double m_Ip;
        double m_omega;
        double m_phase;
    };

}

#endif // MODEL_IAC_H_