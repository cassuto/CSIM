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
        virtual void configure();
        virtual void prepareDC();
        virtual void prepareAC();
        virtual void prepareTR();
        virtual void iterateStep();

    private:
        double m_G;
    };

}

#endif // RESISTOR_H_