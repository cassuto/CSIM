#ifndef RESISTOR_H_
#define RESISTOR_H_

#include "csim/ModelBase.h"

namespace csim
{

    class Resistor : public ModelBase
    {
    public:
        Resistor(Circuit *circuit);
        virtual ~Resistor();
    };

}

#endif // RESISTOR_H_