#ifndef MODEL_CCCS_H_
#define MODEL_CCCS_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class CCCS : public ModelBase
    {
    public:
        CCCS(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~CCCS();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR(double tTime);

        /* Nodes of current output Ist */
        const unsigned int S_NODE = 0;
        const unsigned int T_NODE = 1;
        /* Nodes of control input Ipq */
        const unsigned int P_NODE = 2;
        const unsigned int Q_NODE = 3;

    private:
        double m_alpha;
    };

}

#endif // MODEL_CCCS_H_