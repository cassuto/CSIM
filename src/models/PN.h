#ifndef PN_H_
#define PN_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class PN : public ModelBase
    {
    public:
        PN(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~PN();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR(double tTime);
        virtual void stepChangedTR(double tTime, double nstep);

    private:
        double vlimit(double Ud);

    private:
        double m_Is, m_N, m_Isr, m_Nr, m_Temp, m_Ibv, m_Bv;
        bool m_Bv_set;
        double m_Ut;
        double m_Uth;
        double m_Ud_1;
    };

}

#endif // PN_H_