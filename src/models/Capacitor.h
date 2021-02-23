#ifndef CAPACITOR_H_
#define CAPACITOR_H_

#include "csim/model/ModelBase.h"

namespace csimModel
{

    class Capacitor : public ModelBase
    {
    public:
        Capacitor(MODELBASE_CONSTRUCTOR_DEF);
        virtual ~Capacitor();

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

#endif // CAPACITOR_H_