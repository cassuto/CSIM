#ifndef MODEL_VCCS_H_
#define MODEL_VCCS_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class VCCS : public ModelBase
    {
    public:
        VCCS(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~VCCS();

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
        /* Nodes of control input Upq */
        const unsigned int P_NODE = 2;
        const unsigned int Q_NODE = 3;

    private:
        double m_g;
    };

}

#endif // MODEL_VCCS_H_