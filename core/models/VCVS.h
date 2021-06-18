#ifndef MODEL_VCVS_H_
#define MODEL_VCVS_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class VCVS : public ModelBase
    {
    public:
        VCVS(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~VCVS();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR(double tTime);

        /* Nodes of voltage output Ust */
        const unsigned int S_NODE = 0;
        const unsigned int T_NODE = 1;
        /* Nodes of control input Upq */
        const unsigned int P_NODE = 2;
        const unsigned int Q_NODE = 3;

    private:
        double m_mu;
    };

}

#endif // MODEL_VCVS_H_