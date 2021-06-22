/*
 * Some SPICE sources come from ngspice-34 examples
 */
#include "gtest/gtest.h"
#include "constants.h"
#include <cstdio>
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/parser/hspice-defs.h"
#include <algorithm>
#include <cstring>

#define VERBOSE

namespace csim
{
    static void tstCreateSPICE(const char *filename, const char *content)
    {
        FILE *fp = fopen(filename, "w");
        ASSERT_NE(fp, nullptr);
        fwrite(content, 1, strlen(content), fp);
        fclose(fp);
    }

#define TST_TITLE "Tthis is title line: ~!@#$%^&*()_+`-={}|:\"<>?[]\\;',./\n"

    TEST(tstHSPICEParser, title)
    {
        int ret = 0;
        const char *filename = "tst_hspice_title.cir";
        const char *content =
            TST_TITLE
            "* Comment: ~!@#$%^&*()_+`-={}|:\"<>?[]\\;',./\n"
            "\n"
            " \n"
            ".subckt Rd 1 2\n"
            " \t.subckt Rd2 3 4\n"
            ".ends\n"
            ".ends Rd\n"
            "\n"
            ".end";
        tstCreateSPICE(filename, content);

        HSPICE_AST *ast = new HSPICE_AST();
        ret = ast->parse(filename);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ASSERT_STREQ(TST_TITLE, ast->getTitle());
        ASSERT_EQ(1U, ast->blockRoot->subs.size());
        ASSERT_STREQ("Rd", ast->blockRoot->subs[0]->name.c_str());
        ASSERT_EQ(2U, ast->blockRoot->subs[0]->nodes->nodes.size());
        ASSERT_STREQ("2", ast->blockRoot->subs[0]->nodes->nodes[0].c_str());
        ASSERT_STREQ("1", ast->blockRoot->subs[0]->nodes->nodes[1].c_str());

        ASSERT_EQ(1U, ast->blockRoot->subs[0]->block->subs.size());
        ASSERT_STREQ("Rd2", ast->blockRoot->subs[0]->block->subs[0]->name.c_str());
        ASSERT_STREQ("4", ast->blockRoot->subs[0]->block->subs[0]->nodes->nodes[0].c_str());
        ASSERT_STREQ("3", ast->blockRoot->subs[0]->block->subs[0]->nodes->nodes[1].c_str());

        delete ast;
    }

    TEST(tstHSPICEParser, model)
    {
        int ret = 0;
        const char *filename = "tst_hspice_model.cir";
        const char *content =
            TST_TITLE
            "* Comment: ~!@#$%^&*()_+`-={}|:\"<>?[]\\;',./\n"
            "\n"
            " \n"
            ".subckt Rd 1 2\n"
            "    \t.subckt Sub2 1 2\n"
            ".model n3 nmos level=49 version=3.3.0 tox=3.5n nch=2.4e17 nsub=5e16 vth0=0.15\n"
            "    \t.ends Sub2\n"
            ".ends Rd\n"
            "\n"
            ".model N1 NMOS\n"
            "+Level=        8 version=3.3.0\n"
            "+Tnom=27.0\n"
            "+Nch= 2.498E+17  Tox=9E-09 Xj=1.00000E-07\n"
            "+Lint=9.36e-8 Wint=1.47e-7\n"
            "+Vth0= .6322    K1= .756  K2= -3.83e-2  K3= -2.612\n"
            "+Dvt0= 2.812  Dvt1= 0.462  Dvt2=-9.17e-2\n"
            "+Nlx= 3.52291E-08  W0= 1.163e-6\n"
            "+K3b= 2.233\n"
            "+Vsat= 86301.58  Ua= 6.47e-9  Ub= 4.23e-18  Uc=-4.706281E-11\n"
            "+Rdsw= 650  U0= 388.3203 wr=1\n"
            "+A0= .3496967 Ags=.1    B0=0.546    B1= 1\n"
            "+Dwg = -6.0E-09 Dwb = -3.56E-09 Prwb = -.213\n"
            "+Keta=-3.605872E-02  A1= 2.778747E-02  A2= .9\n"
            "+Voff=-6.735529E-02  NFactor= 1.139926  Cit= 1.622527E-04\n"
            "+Cdsc=-2.147181E-05\n"
            "+Cdscb= 0  Dvt0w =  0 Dvt1w =  0 Dvt2w =  0\n"
            "+Cdscd =  0 Prwg =  0\n"
            "+Eta0= 1.0281729E-02  Etab=-5.042203E-03\n"
            "+Dsub= .31871233\n"
            "+Pclm= 1.114846  Pdiblc1= 2.45357E-03  Pdiblc2= 6.406289E-03\n"
            "+Drout= .31871233  Pscbe1= 5000000  Pscbe2= 5E-09 Pdiblcb = -.234\n"
            "+Pvag= 0 delta=0.01\n"
            "+Wl =  0 Ww = -1.420242E-09 Wwl =  0\n"
            "+Wln =  0 Wwn =  .2613948 Ll =  1.300902E-10\n"
            "+Lw =  0 Lwl =  0 Lln =  .316394\n"
            "+Lwn =  0\n"
            "+kt1=-.3  kt2=-.051\n"
            "+At= 22400\n"
            "+Ute=-1.48\n"
            "+Ua1= 3.31E-10  Ub1= 2.61E-19 Uc1= -3.42e-10\n"
            "+Kt1l=0 Prt=764.3\n"
            "+vgs_max=4 vds_max=4 vbs_max=4\n"
            "\n"
            ".model n2 nmos level=49 version=3.3.0 tox=3.5n nch=2.4e17 nsub=5e16 vth0=0.15\n"
            "+tst_unit1=1f tst_unit2=1p tst_unit3=1n \n"
            "+tst_unit4=1m tst_unit5=1meg tst_unit6=1mil\n"
            "+tst_unit7=1mV tst_unit8=1mega tst_unit9=1megohm\n"
            ".end\n";
        tstCreateSPICE(filename, content);

        HSPICE_AST *ast = new HSPICE_AST();
        ret = ast->parse(filename);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ASSERT_STREQ(ast->getTitle(), TST_TITLE);
#ifdef VERBOSE
        ast->dump();
#endif
        delete ast;
    }

} // namespace
