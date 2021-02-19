#ifndef RESISTOR_H_
#define RESISTOR_H_

#include "csim/ModelBase.h"

namespace csim
{

    class Resistor : public ModelBase
    {
    public:
        Resistor();
        virtual ~Resistor();

    public:
        virtual void prepareDC();
        virtual void prepareAC();
        virtual void prepareTR();
    };

}

#endif // RESISTOR_H_