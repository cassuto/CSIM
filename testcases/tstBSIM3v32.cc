#include "gtest/gtest.h"
#include "constants.h"
#include <fstream>
#include "csim/utils/errors.h"
#include "csim/utils/constants.h"
#include "csim/internal/Analyzers.h"
#include "csim/internal/Circuit.h"
#include "csim/internal/Dataset.h"
#include "csim/internal/Netlist.h"
#include "csim/internal/ModelLoader.h"
#include "csim/internal/parser/algebraic-defs.h"
#include "csim/internal/parser/hspice-defs.h"
#include <algorithm>
#include <cstring>

#define VERBOSE

namespace csim
{
    static void tstMakeNMOSModel(const char *filename)
    {
        const char *content =
            "* NMOS Model\n"
            ".param\n"
            "+lr=0.55u\n"
            "+wr=2.4u\n"
            "+nf=1\n"
            "\n"
            "* TT\n"
            ".PARAM\n"
            "*1.8V core NMOS \n"
            "+DTOX_N18_RF      = 0            DXL_N18_RF       = 0            DXW_N18_RF       = 0 \n"
            "+DVTH_N18_RF      = 0            DCJ_N18_RF       = 0            DCJSW_N18_RF     = 0\n"
            "+DPVTH0_N18_RF    = 0            DCGDO_N18_RF     = 0            DCGSO_N18_RF     = 0\n"
            "\n"
            "*1.8V core PMOS\n"
            "+DTOX_P18_RF      = 0            DXL_P18_RF       = 0            DXW_P18_RF       = 0 \n"
            "+DVTH_P18_RF      = 0	         DCJ_P18_RF       = 0            DCJSW_P18_RF     = 0\n"
            "+DPVTH0_P18_RF    = 0	         DCGDO_P18_RF     = 0            DCGSO_P18_RF     = 0\n"
            "\n"
            "*3.3V IO NMOS \n"
            "+DTOX_N33_RF      = 0            DXL_N33_RF       = 0            DXW_N33_RF       = 0 \n"
            "+DVTH_N33_RF      = 0            DCJ_N33_RF       = 0            DCJSW_N33_RF     = 0\n"
            "+DPVTH0_N33_RF    = 0            DCGDO_N33_RF     = 0            DCGSO_N33_RF     = 0\n"
            "\n"
            "*3.3V IO PMOS\n"
            "+DTOX_P33_RF      = 0            DXL_P33_RF       = 0            DXW_P33_RF       = 0 \n"
            "+DVTH_P33_RF      = 0	         DCJ_P33_RF       = 0            DCJSW_P33_RF     = 0\n"
            "+DPVTH0_P33_RF    = 0	         DCGDO_P33_RF     = 0            DCGSO_P33_RF     = 0\n"
            "\n"
            ".param\n"
            "+Rg_rf        = 'max(((724.84*pwr(lr*1e6,2)-356.18*lr*1e6+50.477)+(-72.591*pwr(lr*1e6,2)+34.823*lr*1e6-4.3737)*wr*1e6+(-709.9*pwr(lr*1e6,2)+340.42*lr*1e6-49.734)/(wr*1e6))+((-5161.5*pwr(lr*1e6,2)+1999.2*lr*1e6-33.119)+(531.35*pwr(lr*1e6,2)-224.13*lr*1e6+12.828)*wr*1e6+(3902.6*pwr(lr*1e6,2)-164.08*lr*1e6+386.39)/(wr*1e6))/nf,1e-3)'\n"
            "+Cgd_rf       = 'max((((0.6677*pwr(lr*1e6,2)-0.4222*lr*1e6+0.0529)*pwr(wr*1e6,2)+(-5.7464*pwr(lr*1e6,2)+3.8016*lr*1e6-0.1298)*wr*1e6+(3.275*pwr(lr*1e6,2)-2.272*lr*1e6+0.6075))*nf+((-3.5823*pwr(lr*1e6,2)+2.4353*lr*1e6-0.3546)*pwr(wr*1e6,2)+(30.068*pwr(lr*1e6,2)-20.101*lr*1e6+2.8423)*wr*1e6+(-23.613*pwr(lr*1e6,2)+15.794*lr*1e6-1.9974)))*1e-15, 1e-18)'\n"
            "+Cgs_rf       = 'max((((0.0651*pwr(lr*1e6,2)-0.0346*lr*1e6+0.004)*pwr(wr*1e6,2)+(-0.701*pwr(lr*1e6,2)+0.3523*lr*1e6-0.0371)*wr*1e6+(1.0807*pwr(lr*1e6,2)-0.5396*lr*1e6+0.061))*pwr(nf,2)+((-0.1448*pwr(lr*1e6,2)-0.1172*lr*1e6+0.0525)*pwr(wr*1e6,2)+(1.1016*pwr(lr*1e6,2)+2.1538*lr*1e6-0.8227)*wr*1e6+(-11.232*pwr(lr*1e6,2)+1.8929*lr*1e6+0.7917))*nf+((-2.1156*pwr(lr*1e6,2)+2.368*lr*1e6-0.5178)*pwr(wr*1e6,2)+(33.552*pwr(lr*1e6,2)-34.449*lr*1e6+6.9433)*wr*1e6+(-54.224*pwr(lr*1e6,2)+52.226*lr*1e6-7.2535)))*1e-15, 1e-18)'\n"
            "+Cds_rf       = 'max((((1.2656*pwr(lr*1e6,2)-0.8625*lr*1e6+0.1269)*pwr(wr*1e6,2)+(-14.055*pwr(lr*1e6,2)+8.8032*lr*1e6-0.7362)*wr*1e6+(16.189*pwr(lr*1e6,2)-11.156*lr*1e6+1.6503))*nf+((-6.6026*pwr(lr*1e6,2)+3.9526*lr*1e6-0.3798)*pwr(wr*1e6,2)+(76.57*pwr(lr*1e6,2)-47.009*lr*1e6+4.7061)*wr*1e6+(-74.026*pwr(lr*1e6,2)+47.818*lr*1e6-4.9076)))*1e-15, 1e-18)'\n"
            "+Rsub1_rf     = 'max(((-173.25*lr*1e6+30.089)*pwr(wr*1e6,2)+(1110*lr*1e6-81.61)*wr*1e6+(-7105.6*lr*1e6+1996.8))*pwr(nf,1.7573*lr*1e6-0.8936), 1e-3)'\n"
            "+Rsub2_rf     = 'max(((-2.52*lr*1e6+0.4097)*pwr(wr*1e6,2)+(16.422*lr*1e6-2.5923)*wr*1e6+(-21.611*lr*1e6+2.9913))*nf+((43.175*lr*1e6-5.3693)*pwr(wr*1e6,2)+(-232.18*lr*1e6+24.958)*wr*1e6+(399*lr*1e6+31.75)), 1e-3)'\n"
            "+Rsub3_rf     = 'max(((-318.02*lr*1e6+94.625)*pwr(wr*1e6,2)+(4878.4*lr*1e6-1449.3)*wr*1e6+(-18771*lr*1e6+5701.4))+((1288.3*lr*1e6-389.18)*pwr(wr*1e6,2)+(-18714*lr*1e6+5649.5)*wr*1e6+(59353*lr*1e6-18005))/NF, 1e-3)'\n"
            "+Djdb_AREA_rf = 'nf/2*wr*(0.8-0.14)*1e-6'\n"
            "+Djdb_PJ_rf   = '(1+1.2547e-7/wr)*nf*wr'\n"
            "+Djsb_AREA_rf = 'wr*(0.8-0.07)*2*1e-6+(nf/2-1)*wr*(0.8-0.14)*1e-6'\n"
            "+Djsb_PJ_rf   = '(1+1.2547e-7/wr)*nf*wr'\n"
            "+Rdc_n18      = 'max((0.1008/(wr*1e6)+0.102*nf), 1e-3)'\n"
            "+Rsc_n18      = 'max((0.1008/(wr*1e6)+0.102*nf), 1e-3)'   \n"
            "+Cgdo_n18     = 'max((0+dcgdo_n18_rf), 0)'\n"
            "+Cgso_n18     = 'max((0+dcgso_n18_rf), 0)'\n"
            "+Cj_n18       = 'max((0+dcj_n18_rf), 0)'\n"
            "+Cjsw_n18     = 'max((0+dcjsw_n18_rf), 0)'\n"
            "+Pvag_n18     = '-((0.0733*lr*1000000-0.0117)*pwr(wr*1000000,2)+(-1.02*lr*1000000+0.1431)*wr*1000000+(3.3992*lr*1000000-0.2917))*pwr(nf,(0.0508*lr*1000000-0.0097)*pwr(wr*1000000,2)+(-0.3633*lr*1000000+0.1088)*wr*1000000+(-0.24*lr*1000000+0.0165))'\n"
            "*****************************************\n"
            "* MOS Model\n"
            ".model  n18_rf  nmos\n"
            "+level = 49\n"
            "**************************************************************\n"
            "*               MODEL FLAG PARAMETERS \n"
            "**************************************************************\n"
            "+lmin    = 1.6e-007        lmax    = 1.2e-006        wmin    = 4.8e-007      \n"
            "+wmax    = 1.002e-005      version = 3.24             mobmod  = 1             \n"
            "+capmod  = 3               nqsmod  = 0               binunit = 2             \n"
            "**************************************************************\n"
            "*               GENERAL MODEL PARAMETERS \n"
            "**************************************************************\n"
            "+tref    = 25              xl      = '1.8e-008+DXL_N18_RF'        xw      = '0+DXW_N18_RF'        \n"
            "+tox     = '3.87e-009+DTOX_N18_RF'        toxm    = 3.87e-009       wint    = -1.4450482E-09   \n"
            "+lint    = 1.5757085E-08     dlc     = 8.5e-009        dwc     = 4.5e-008      \n"
            "+hdif    = 2e-007          ldif    = 7e-008          ll      = 2.6352781e-016\n"
            "+wl      = -2.3664573e-016  lln     = 1.1205959       wln     = 1.0599999     \n"
            "+lw      = -2.2625584e-016  ww      = -3.640969e-014  lwn     = 0.92          \n"
            "+wwn     = 0.8768474       lwl     = -2.0576711e-022  wwl     = -4e-021       \n"
            "+cgso    = 'Cgso_n18'               cgdo    = 'Cgdo_n18'               xpart   = 1   \n"
            "+rdc ='Rdc_n18' rsc='Rsc_n18'          \n"
            "**************************************************************\n"
            "*               EXPERT PARAMETERS \n"
            "**************************************************************\n"
            "+vth0    = '0.39+DVTH_N18_RF'        wvth0   = -2.9709472e-008  pvth0   = '5e-016+DPVTH0_N18_RF'                \n"
            "+k1      = 0.6801043       wk1     = -2.489684e-008  pk1     = 1.3e-015      \n"
            "+k2      = -0.049978       k3      = 10              k3b     = -3            \n"
            "+nlx     = 7.545103e-008   dvt0    = 1.3             dvt1    = 0.5771635     \n"
            "+dvt2    = -0.1717554      dvt0w   = 0               dvt1w   = 0             \n"
            "+dvt2w   = 0               nch     = 3.8694e+017     voff    = -0.103        \n"
            "+lvoff   = -3.3e-009       nfactor = 1.25            lnfactor= 4.5e-008      \n"
            "+cdsc    = 0               cdscb   = 0               cdscd   = 0.0001        \n"
            "+cit     = 0               u0      = 0.032953        lu0     = 2.3057663e-011\n"
            "+wu0     = -3.1009695e-009  ua      = -1.03e-009      lua     = 7.734979e-019 \n"
            "+pua     = -1e-024         ub      = 2.3667e-018     uc      = 1.2e-010      \n"
            "+puc     = 1.5e-024        xj      = 1.6e-007        w0      = 5.582015e-007 \n"
            "+prwg    = 0.4             prwb    = -0.24           wr      = 1             \n"
            "+rdsw    = 55.54972        a0      = 0.83            ags     = 0.32          \n"
            "+a1      = 0               a2      = 0.99            b0      = 6e-008        \n"
            "+b1      = 0               vsat    = 82500           pvsat   = -8.3e-010     \n"
            "+keta    = -0.003          lketa   = -1.7e-009       dwg     = -5.96e-009    \n"
            "+dwb     = 4.5e-009        alpha0  = 1.7753978e-008  beta0   = 11.168394     \n"
            "+pclm    = 1.2             ppclm   = 2.9999999e-015  pdiblc1 = 0.025         \n"
            "+pdiblc2 = 0.0038          ppdiblc2= 2.7000001e-016  pdiblcb = 0             \n"
            "+drout   = 0.56            pvag    = 'Pvag_n18'               pscbe1  = 3.45e+008     \n"
            "+pscbe2  = 1e-006          delta   = 0.01            eta0    = 0.028000001   \n"
            "+etab    = -0.027000001    dsub    = 0.4             elm     = 5             \n"
            "+alpha1  = 0.1764          lalpha1 = 7.625e-009    \n"
            "**************************************************************\n"
            "*               CAPACITANCE PARAMETERS \n"
            "**************************************************************\n"
            "+cf      = 0               acde    = 0.64            moin    = 24            \n"
            "+noff    = 1.2025        CJ       = 'Cj_n18'\n"
            "+CJSW     = 'Cjsw_n18'   CGBO=0\n"
            "\n"
            "**************************************************************\n"
            "*               TEMPERATURE PARAMETERS \n"
            "**************************************************************\n"
            "+kt1     = -0.2572866      kt1l    = -1e-009         kt2     = -0.04         \n"
            "+ute     = -1.55           ua1     = 1.76e-009       lua1    = 6e-018        \n"
            "+wua1    = -1.1e-016       pua1    = -5e-025         ub1     = -2.4e-018     \n"
            "+uc1     = -1e-010         luc1    = 1.6999999e-017  puc1    = -3e-024       \n"
            "+prt     = -55             at      = 37000           pat     = -7.5e-010     \n"
            "**************************************************************\n"
            "*               NOISE PARAMETERS \n"
            "**************************************************************\n"
            "+NOIMOD   = 2                   NOIA     = 8.2282E+19            NOIB     = 1.3327E+04    \n"
            "+NOIC     = -2.4937E-14         EM       = 1.7767E+07            EF       = 8.1800E-01 \n"
            "**************************************************************\n"
            ".end";
        FILE *fp = fopen(filename, "w");
        ASSERT_NE(fp, nullptr);
        fwrite(content, 1, strlen(content), fp);
        fclose(fp);
    }

    TEST(tstBSIM3v32, transfer)
    {
        int ret = 0;
        ModelEntry *e_MOSFET = ModelLoader::load(BSIM3V32Library);
        ASSERT_NE(nullptr, e_MOSFET);
        ModelEntry *e_V2 = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_V2);
        ModelEntry *e_V3 = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_V3);
        ModelEntry *e_V4 = ModelLoader::load(VDCLibrary);
        ASSERT_NE(nullptr, e_V4);

        Circuit *circuit = new Circuit();

        /* Add models */
        ret = circuit->netlist()->addComponent("M1", e_MOSFET);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V2", e_V2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V3", e_V3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addComponent("V4", e_V4);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Model */
        const csim::ModelEntry::MdlEntry *nmosMdlEntry;
        ret = e_MOSFET->getMdlEntry("nmos", &nmosMdlEntry);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->addMdl("n18_rf", nmosMdlEntry);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        csimModel::PropertyMdl *nmosMdl = circuit->netlist()->getMdl("n18_rf");
        ASSERT_NE(nmosMdl, nullptr);
        const char *filename = "tst_bsim3v32_transfer.cir";
        tstMakeNMOSModel(filename);
        HSPICE_AST *ast = new HSPICE_AST();
        ret = ast->parse(filename);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ASSERT_EQ(1U, ast->blockRoot->models.size());
        ret = ast->blockRoot->models[0].configureMdl(nmosMdlEntry, nmosMdl);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        delete ast;

        /* Configure */
        ret = circuit->netlist()->configComponent("M1", "W", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(2.4e-6));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("M1", "L", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.55e-6));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("M1", "M", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(1.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("M1", "mdl", csimModel::Variant(csimModel::Variant::VariantPropertyModel).setPropertyModel(nmosMdl));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V2", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V3", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->configComponent("V4", "V", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0));
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* Set netlist */
        ret = circuit->netlist()->prepare();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->wire("V4", 0, "V3", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V4", 1, "M1", 0);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V3", 1, "V2", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V2", 0, "M1", 1);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("V2", 1, "M1", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->wire("M1", 2, "M1", 3);
        ASSERT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->ground("M1", 2);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        ret = circuit->netlist()->generateNodes();
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        /* DC analysis */
        AnalyzerBase *analyzer = Analyzers::createInstance("DC", circuit);
        ASSERT_NE(nullptr, analyzer);
        analyzer->property().setProperty("vstart", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.0));
        analyzer->property().setProperty("vstop", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(5.0));
        analyzer->property().setProperty("vincr", csimModel::Variant(csimModel::Variant::VariantDouble).setDouble(0.1));
        analyzer->property().setProperty("srcname", csimModel::Variant(csimModel::Variant::VariantString).setString("V2"));

        /* Get nodes */
        unsigned int n_gnd, n2, b4;
        ret = circuit->netlist()->getBranch("V4", 0, &b4);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("V2", 0, &n2);
        EXPECT_EQ(CERR_SUCCEEDED, ret);
        ret = circuit->netlist()->getTermlNode("V2", 1, &n_gnd);
        EXPECT_EQ(CERR_SUCCEEDED, ret);

        analyzer->addInterestBranch(b4);
        analyzer->addInterestNode(n2);
        analyzer->addInterestNode(n_gnd);

        Dataset dset;
        ret = analyzer->analyze(&dset);
        ASSERT_EQ(CERR_SUCCEEDED, ret);

        const Variable &sweep = dset.getIndependentVar("V2");
        const Variable &Vgnd = dset.getDependentVar("voltage", analyzer->makeVarName("V", n_gnd));
        const Variable &Vn1 = dset.getDependentVar("voltage", analyzer->makeVarName("V", n2));
        const Variable &Ib2 = dset.getDependentVar("current", analyzer->makeVarName("I", b4));

        std::ofstream fof("outBSIM3v32.csv");
        for (size_t i = 0; i < sweep.getNumValues(); i++)
        {
            double voltSrc = sweep.at(i).real();
            fof << voltSrc << "," << Ib2.at(i).real() << "\n";
        }

        delete circuit;
        delete e_MOSFET;
        delete e_V4;
        delete e_V3;
        delete e_V2;
    }
}
