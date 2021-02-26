#ifndef RESISTOR_H_
#define RESISTOR_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class Resistor : public ModelBase
    {
    public:
        Resistor(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~Resistor();

    public:
        virtual int configure();
        virtual int prepareDC();
        virtual int prepareAC();
        virtual int prepareTR();
        virtual int iterateDC();
        virtual int iterateAC(double omega);
        virtual int iterateTR(double tTime);

    private:
        double m_G;
        bool m_cutThrough;
    };

}

#endif // RESISTOR_H_