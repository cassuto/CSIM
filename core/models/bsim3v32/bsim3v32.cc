/**
 * @file BSIM3V3.2 (Berkeley Short Channel IGFET Model Version-3)
 */

/**** BSIM3v3.2.4, Released by Xuemei Xi 12/21/2001 ****/

/**********
 * Copyright 2001 Regents of the University of California. All rights reserved.
 * File: b3set.c of BSIM3v3.2.4
 * Author: 1995 Min-Chie Jeng and Mansun Chan.
 * Author: 1997-1999 Weidong Liu.
 * Author: 2001  Xuemei Xi
 **********/
/*
 *  FastCSIM Copyright (C) 2021 cassuto                                    
 *  This project is free edition{} you can redistribute it and/or          
 *  modify it under the terms of the GNU Lesser General Public             
 *  License(LGPL) as published by the Free Software Foundation{} either     
 *  version 2.1 of the License, or (at your option) any later version.     
 *                                                                         
 *  This project is distributed in the hope that it will be useful,        
 *  but WITHOUT ANY WARRANTY{} without even the implied warranty of        
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 *  Lesser General Public License for more details.                        
 */

#include "bsim3v32.h"

namespace csimModel
{

    const ModelBase::StatusDescriptor BSIM3V32::m_statusDesc[] = {
        {"gmbs", Variant::VariantDouble, "Gmb", BSIM3v32_STAT_GMBS},
        {"gm", Variant::VariantDouble, "Gm", BSIM3v32_STAT_GM},
        {"gds", Variant::VariantDouble, "Gds", BSIM3v32_STAT_GDS},
        {"vdsat", Variant::VariantDouble, "Vdsat", BSIM3v32_STAT_VDSAT},
        {"vth", Variant::VariantDouble, "Vth", BSIM3v32_STAT_VTH},
        {"id", Variant::VariantDouble, "Ids", BSIM3v32_STAT_ID},
        {"vbd", Variant::VariantDouble, "Vbs", BSIM3v32_STAT_VBD},
        {"vbs", Variant::VariantDouble, "Vbs", BSIM3v32_STAT_VBS},
        {"vgs", Variant::VariantDouble, "Vgs", BSIM3v32_STAT_VGS},
        {"vds", Variant::VariantDouble, "Vds", BSIM3v32_STAT_VDS},
        {"ibd", Variant::VariantDouble, "Ibd", BSIM3v32_STAT_IBD},
        {"ibs", Variant::VariantDouble, "Ibs", BSIM3v32_STAT_IBS},
        {"gbd", Variant::VariantDouble, "gbd", BSIM3v32_STAT_GBD},
        {"gbs", Variant::VariantDouble, "gbs", BSIM3v32_STAT_GBS},
        {"qb", Variant::VariantDouble, "Qbulk", BSIM3v32_STAT_QB},
        {"cqb", Variant::VariantDouble, "CQbulk", BSIM3v32_STAT_CQB},
        {"qg", Variant::VariantDouble, "Qgate", BSIM3v32_STAT_QG},
        {"cqg", Variant::VariantDouble, "CQgate", BSIM3v32_STAT_CQG},
        {"qd", Variant::VariantDouble, "Qdrain", BSIM3v32_STAT_QD},
        {"cqd", Variant::VariantDouble, "CQdrain", BSIM3v32_STAT_CQD},
        {"cgg", Variant::VariantDouble, "Cggb", BSIM3v32_STAT_CGG},
        {"cgd", Variant::VariantDouble, "Cgdb", BSIM3v32_STAT_CGD},
        {"cgs", Variant::VariantDouble, "Cgsb", BSIM3v32_STAT_CGS},
        {"cdg", Variant::VariantDouble, "Cdgb", BSIM3v32_STAT_CDG},
        {"cdd", Variant::VariantDouble, "Cddb", BSIM3v32_STAT_CDD},
        {"cds", Variant::VariantDouble, "Cdsb", BSIM3v32_STAT_CDS},
        {"cbg", Variant::VariantDouble, "Cbgb", BSIM3v32_STAT_CBG},
        {"cbd", Variant::VariantDouble, "Cbdb", BSIM3v32_STAT_CBD},
        {"cbs", Variant::VariantDouble, "Cbsb", BSIM3v32_STAT_CBS},
        {"capbd", Variant::VariantDouble, "Capbd", BSIM3v32_STAT_CAPBD},
        {"capbs", Variant::VariantDouble, "Capbs", BSIM3v32_STAT_CAPBS},
    };

    BSIM3V32::BSIM3V32(MODELBASE_CONSTRUCTOR_DEF)
        : ModelBase(MODELBASE_CONSTRUCTOR_VAR),
          tranFirstStep(true),
          BSIM3v32dNodePrimeInserted(false),
          BSIM3v32sNodePrimeInserted(false),
          BSIM3v32qNodeInserted(false),
          BSIM3v32dNodePrime(0),
          BSIM3v32sNodePrime(0),
          BSIM3v32qNode(0),
          BSIM3v32ueff(0),
          BSIM3v32thetavth(0),
          BSIM3v32von(0),
          BSIM3v32vdsat(0),
          BSIM3v32cgdo(0),
          BSIM3v32cgso(0),
          BSIM3v32vjsm(0),
          BSIM3v32IsEvjsm(0),
          BSIM3v32vjdm(0),
          BSIM3v32IsEvjdm(0),

          BSIM3v32l(0),
          BSIM3v32w(0),
          BSIM3v32m(0),
          BSIM3v32drainArea(0),
          BSIM3v32sourceArea(0),
          BSIM3v32drainSquares(0),
          BSIM3v32sourceSquares(0),
          BSIM3v32drainPerimeter(0),
          BSIM3v32sourcePerimeter(0),
          BSIM3v32sourceConductance(0),
          BSIM3v32drainConductance(0),
          BSIM3v32delvto(0),
          BSIM3v32mulu0(0),
          BSIM3v32vth0(0),
          BSIM3v32vfb(0),
          BSIM3v32vfbzb(0),
          BSIM3v32u0temp(0),
          BSIM3v32tconst(0),

          BSIM3v32icVBS(0),
          BSIM3v32icVDS(0),
          BSIM3v32icVGS(0),
          BSIM3v32off(0),
          BSIM3v32mode(0),
          BSIM3v32nqsMod(0),
          BSIM3v32geo(0),

          BSIM3v32qinv(0),
          BSIM3v32cd(0),
          BSIM3v32cbs(0),
          BSIM3v32cbd(0),
          BSIM3v32csub(0),
          BSIM3v32gm(0),
          BSIM3v32gds(0),
          BSIM3v32gmbs(0),
          BSIM3v32gbd(0),
          BSIM3v32gbs(0),

          BSIM3v32gbbs(0),
          BSIM3v32gbgs(0),
          BSIM3v32gbds(0),

          BSIM3v32cggb(0),
          BSIM3v32cgdb(0),
          BSIM3v32cgsb(0),
          BSIM3v32cbgb(0),
          BSIM3v32cbdb(0),
          BSIM3v32cbsb(0),
          BSIM3v32cdgb(0),
          BSIM3v32cddb(0),
          BSIM3v32cdsb(0),
          BSIM3v32capbd(0),
          BSIM3v32capbs(0),

          BSIM3v32cqgb(0),
          BSIM3v32cqdb(0),
          BSIM3v32cqsb(0),
          BSIM3v32cqbb(0),

          BSIM3v32qgate(0),
          BSIM3v32qbulk(0),
          BSIM3v32qdrn(0),

          BSIM3v32gtau(0),
          BSIM3v32gtg(0),
          BSIM3v32gtd(0),
          BSIM3v32gts(0),
          BSIM3v32gtb(0),
          BSIM3v32rds(0),
          BSIM3v32Vgsteff(0),
          BSIM3v32Vdseff(0),
          BSIM3v32Abulk(0),
          BSIM3v32AbovVgst2Vtm(0),
          pParam(nullptr),
          BSIM3v32lGiven(0),
          BSIM3v32wGiven(0),
          BSIM3v32mGiven(0),
          BSIM3v32drainAreaGiven(0),
          BSIM3v32sourceAreaGiven(0),
          BSIM3v32drainSquaresGiven(0),
          BSIM3v32sourceSquaresGiven(0),
          BSIM3v32drainPerimeterGiven(0),
          BSIM3v32sourcePerimeterGiven(0),
          BSIM3v32delvtoGiven(0),
          BSIM3v32mulu0Given(0),
          BSIM3v32dNodePrimeSet(0),
          BSIM3v32sNodePrimeSet(0),
          BSIM3v32icVBSGiven(0),
          BSIM3v32icVDSGiven(0),
          BSIM3v32icVGSGiven(0),
          BSIM3v32nqsModGiven(0),
          BSIM3v32geoGiven(0),

          BSIM3v32DdPtr(nullptr),
          BSIM3v32GgPtr(nullptr),
          BSIM3v32SsPtr(nullptr),
          BSIM3v32BbPtr(nullptr),
          BSIM3v32DPdpPtr(nullptr),
          BSIM3v32SPspPtr(nullptr),
          BSIM3v32DdpPtr(nullptr),
          BSIM3v32GbPtr(nullptr),
          BSIM3v32GdpPtr(nullptr),
          BSIM3v32GspPtr(nullptr),
          BSIM3v32SspPtr(nullptr),
          BSIM3v32BdpPtr(nullptr),
          BSIM3v32BspPtr(nullptr),
          BSIM3v32DPspPtr(nullptr),
          BSIM3v32DPdPtr(nullptr),
          BSIM3v32BgPtr(nullptr),
          BSIM3v32DPgPtr(nullptr),
          BSIM3v32SPgPtr(nullptr),
          BSIM3v32SPsPtr(nullptr),
          BSIM3v32DPbPtr(nullptr),
          BSIM3v32SPbPtr(nullptr),
          BSIM3v32SPdpPtr(nullptr),

          BSIM3v32QqPtr(nullptr),
          BSIM3v32QdpPtr(nullptr),
          BSIM3v32QgPtr(nullptr),
          BSIM3v32QspPtr(nullptr),
          BSIM3v32QbPtr(nullptr),
          BSIM3v32DPqPtr(nullptr),
          BSIM3v32GqPtr(nullptr),
          BSIM3v32SPqPtr(nullptr),
          BSIM3v32BqPtr(nullptr),
          model(nullptr)
    {
        property().addProperty("l", Variant(Variant::VariantDouble).setDouble(0), "Length", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("w", Variant(Variant::VariantDouble).setDouble(0), "Width", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("m", Variant(Variant::VariantDouble).setDouble(0), "Parallel multiplier", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("ad", Variant(Variant::VariantDouble).setDouble(0), "Drain area", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("as", Variant(Variant::VariantDouble).setDouble(0), "Source area", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("pd", Variant(Variant::VariantDouble).setDouble(0), "Drain perimeter", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("ps", Variant(Variant::VariantDouble).setDouble(0), "Source perimeter", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("nrd", Variant(Variant::VariantDouble).setDouble(0), "Number of squares in drain", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("nrs", Variant(Variant::VariantDouble).setDouble(0), "Number of squares in source", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("off", Variant(Variant::VariantBoolean).setBoolean(false), "Device is initially off", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("nqsmod", Variant(Variant::VariantInt32).setInt32(0), "Non-quasi-static model selector", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("geo", Variant(Variant::VariantInt32).setInt32(0), "ACM model drain/source connection", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("delvto", Variant(Variant::VariantDouble).setDouble(0), "Zero bias threshold voltage variation", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("mulu0", Variant(Variant::VariantDouble).setDouble(0), "Low field mobility multiplier", csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);
        property().addProperty("ic_vbs", Variant(Variant::VariantDouble).setDouble(0), "BS initial voltages", csimModel::PropertyBag::Write);
        property().addProperty("ic_vds", Variant(Variant::VariantDouble).setDouble(0), "DS initial voltages", csimModel::PropertyBag::Write);
        property().addProperty("ic_vgs", Variant(Variant::VariantDouble).setDouble(0), "GS initial voltages", csimModel::PropertyBag::Write);
        property().addProperty("mdl", Variant(Variant::VariantPropertyModel).setPropertyModel(nullptr), "Model", csimModel::PropertyBag::Required | csimModel::PropertyBag::Write | csimModel::PropertyBag::Read);

        registerStatus(m_statusDesc, sizeof(m_statusDesc) / sizeof(ModelBase::StatusDescriptor));
    }

    BSIM3V32::~BSIM3V32()
    {
    }

    int BSIM3V32::configure()
    {
        int rc;
        unsigned int numInnerNodes = 0;
        unsigned int insertedNodeIdx = 4; /* = Number of terminals */

        const char *entry;
        entry = "w";
        this->BSIM3v32w = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32wGiven = property().propertyGiven(entry);

        entry = "l";
        this->BSIM3v32l = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32lGiven = property().propertyGiven(entry);

        entry = "m";
        this->BSIM3v32m = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32mGiven = property().propertyGiven(entry);

        entry = "as";
        this->BSIM3v32sourceArea = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32sourceAreaGiven = property().propertyGiven(entry);

        entry = "ad";
        this->BSIM3v32drainArea = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32drainAreaGiven = property().propertyGiven(entry);

        entry = "ps";
        this->BSIM3v32sourcePerimeter = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32sourcePerimeterGiven = property().propertyGiven(entry);

        entry = "pd";
        this->BSIM3v32drainPerimeter = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32drainPerimeterGiven = property().propertyGiven(entry);

        entry = "nrs";
        this->BSIM3v32sourceSquares = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32sourceSquaresGiven = property().propertyGiven(entry);

        entry = "nrd";
        this->BSIM3v32drainSquares = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32drainSquaresGiven = property().propertyGiven(entry);

        entry = "off";
        this->BSIM3v32off = property().getProperty(entry).getBooleanCast(&rc);
        UPDATE_RC(rc);

        entry = "ic_vbs";
        this->BSIM3v32icVBS = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32icVBSGiven = property().propertyGiven(entry);

        entry = "ic_vds";
        this->BSIM3v32icVDS = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32icVDSGiven = property().propertyGiven(entry);

        entry = "ic_vgs";
        this->BSIM3v32icVGS = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32icVGSGiven = property().propertyGiven(entry);

        entry = "nqsmod";
        this->BSIM3v32nqsMod = property().getProperty(entry).getInt32Cast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32nqsModGiven = property().propertyGiven(entry);

        entry = "geo";
        this->BSIM3v32geo = property().getProperty(entry).getInt32Cast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32geoGiven = property().propertyGiven(entry);

        entry = "delvto";
        this->BSIM3v32delvto = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32delvtoGiven = property().propertyGiven(entry);

        entry = "mulu0";
        this->BSIM3v32mulu0 = property().getProperty(entry).getDoubleCast(&rc);
        UPDATE_RC(rc);
        this->BSIM3v32mulu0Given = property().propertyGiven(entry);

        entry = "mdl";
        this->model = dynamic_cast<BSIM3v32model *>(property().getProperty(entry).getPropertyModel());

        if (!this->model)
            return CERR_NO_MDL;

        /* perform the parameter defaulting */
        if (!this->BSIM3v32drainAreaGiven)
            this->BSIM3v32drainArea = 0.0;
        if (!this->BSIM3v32drainPerimeterGiven)
            this->BSIM3v32drainPerimeter = 0.0;
        if (!this->BSIM3v32drainSquaresGiven)
        {
            if (model->BSIM3v32acmMod == 0)
                this->BSIM3v32drainSquares = 1.0;
            else
                this->BSIM3v32drainSquares = 0.0;
        }
        if (!this->BSIM3v32delvtoGiven)
            this->BSIM3v32delvto = 0.0;
        if (!this->BSIM3v32mulu0Given)
            this->BSIM3v32mulu0 = 1.0;
        if (!this->BSIM3v32icVBSGiven)
            this->BSIM3v32icVBS = 0.0;
        if (!this->BSIM3v32icVDSGiven)
            this->BSIM3v32icVDS = 0.0;
        if (!this->BSIM3v32icVGSGiven)
            this->BSIM3v32icVGS = 0.0;
        if (!this->BSIM3v32lGiven)
            this->BSIM3v32l = 5.0e-6;
        if (!this->BSIM3v32sourceAreaGiven)
            this->BSIM3v32sourceArea = 0.0;
        if (!this->BSIM3v32sourcePerimeterGiven)
            this->BSIM3v32sourcePerimeter = 0.0;
        if (!this->BSIM3v32sourceSquaresGiven)
        {
            if (model->BSIM3v32acmMod == 0)
                this->BSIM3v32sourceSquares = 1.0;
            else
                this->BSIM3v32sourceSquares = 0.0;
        }
        if (!this->BSIM3v32wGiven)
            this->BSIM3v32w = 5.0e-6;
        if (!this->BSIM3v32nqsModGiven)
            this->BSIM3v32nqsMod = model->BSIM3v32nqsMod;
        else if ((this->BSIM3v32nqsMod != 0) && (this->BSIM3v32nqsMod != 1))
        {
            this->BSIM3v32nqsMod = model->BSIM3v32nqsMod;
            printf("Warning: nqsMod has been set to its global value %d.\n",
                   model->BSIM3v32nqsMod);
        }
        if (!this->BSIM3v32geoGiven)
            this->BSIM3v32geo = 0;

        if (!this->BSIM3v32mGiven)
            this->BSIM3v32m = 1;

        /* process source/drain series resistance */
        /* ACM model */

        double DrainResistance, SourceResistance;

        if (model->BSIM3v32acmMod == 0)
        {
            DrainResistance = model->BSIM3v32sheetResistance * this->BSIM3v32drainSquares;
            SourceResistance = model->BSIM3v32sheetResistance * this->BSIM3v32sourceSquares;
        }
        else /* ACM > 0 */
        {
            rc = ACM_SourceDrainResistances(
                model->BSIM3v32acmMod,
                model->BSIM3v32ld,
                model->BSIM3v32ldif,
                model->BSIM3v32hdif,
                model->BSIM3v32wmlt,
                this->BSIM3v32w,
                model->BSIM3v32xw,
                model->BSIM3v32sheetResistance,
                this->BSIM3v32drainSquaresGiven,
                model->BSIM3v32rd,
                model->BSIM3v32rdc,
                this->BSIM3v32drainSquares,
                this->BSIM3v32sourceSquaresGiven,
                model->BSIM3v32rs,
                model->BSIM3v32rsc,
                this->BSIM3v32sourceSquares,
                &DrainResistance,
                &SourceResistance);
            if (rc)
                return rc;
        }

        /* process drain series resistance */
        if (DrainResistance != 0)
        {
            if (!this->BSIM3v32dNodePrimeInserted)
            {
                this->BSIM3v32dNodePrime = insertedNodeIdx++;
                this->BSIM3v32dNodePrimeInserted = true;
                numInnerNodes++;
            }
        }
        else
        {
            this->BSIM3v32dNodePrime = this->BSIM3v32dNode;
        }

        /* process source series resistance */
        if (SourceResistance != 0)
        {
            if (!this->BSIM3v32sNodePrimeInserted)
            {
                this->BSIM3v32sNodePrime = insertedNodeIdx++;
                this->BSIM3v32sNodePrimeInserted = true;
                numInnerNodes++;
            }
        }
        else
        {
            this->BSIM3v32sNodePrime = this->BSIM3v32sNode;
        }

        /* internal charge node */
        if (this->BSIM3v32nqsMod)
        {
            if (!this->BSIM3v32qNodeInserted)
            {
                this->BSIM3v32qNode = insertedNodeIdx++;
                this->BSIM3v32qNodeInserted = true;
                numInnerNodes++;
            }
        }
        else
        {
            this->BSIM3v32qNode = 0;
        }

        resizeModel(4, numInnerNodes, 0);

        /* Channel length scaling with lmlt model parameter */

        if (model->BSIM3v32lmltGiven)
            this->BSIM3v32l *= model->BSIM3v32lmlt;

        return 0;
    }

    int BSIM3V32::prepareDC()
    {
        resizeIntegrator(BSIM3v32numStates);
        if (!model)
            return CERR_NO_MDL;

            /* set Sparse Matrix Pointers */

/* macro to make elements with built in test for out of memory */
#define ALLOC_VOLT(ptr, first, second)      \
    do                                      \
    {                                       \
        this->ptr = getYPtr(first, second); \
    } while (0)

#define GET_INSERTED_NODE(first) getNode(first)

        ALLOC_VOLT(BSIM3v32DdPtr, getNode(BSIM3v32dNode), getNode(BSIM3v32dNode));
        ALLOC_VOLT(BSIM3v32GgPtr, getNode(BSIM3v32gNode), getNode(BSIM3v32gNode));
        ALLOC_VOLT(BSIM3v32SsPtr, getNode(BSIM3v32sNode), getNode(BSIM3v32sNode));
        ALLOC_VOLT(BSIM3v32BbPtr, getNode(BSIM3v32bNode), getNode(BSIM3v32bNode));
        ALLOC_VOLT(BSIM3v32DPdpPtr, GET_INSERTED_NODE(BSIM3v32dNodePrime), GET_INSERTED_NODE(BSIM3v32dNodePrime));
        ALLOC_VOLT(BSIM3v32SPspPtr, GET_INSERTED_NODE(BSIM3v32sNodePrime), GET_INSERTED_NODE(BSIM3v32sNodePrime));
        ALLOC_VOLT(BSIM3v32DdpPtr, getNode(BSIM3v32dNode), GET_INSERTED_NODE(BSIM3v32dNodePrime));
        ALLOC_VOLT(BSIM3v32GbPtr, getNode(BSIM3v32gNode), getNode(BSIM3v32bNode));
        ALLOC_VOLT(BSIM3v32GdpPtr, getNode(BSIM3v32gNode), GET_INSERTED_NODE(BSIM3v32dNodePrime));
        ALLOC_VOLT(BSIM3v32GspPtr, getNode(BSIM3v32gNode), GET_INSERTED_NODE(BSIM3v32sNodePrime));
        ALLOC_VOLT(BSIM3v32SspPtr, getNode(BSIM3v32sNode), GET_INSERTED_NODE(BSIM3v32sNodePrime));
        ALLOC_VOLT(BSIM3v32BdpPtr, getNode(BSIM3v32bNode), GET_INSERTED_NODE(BSIM3v32dNodePrime));
        ALLOC_VOLT(BSIM3v32BspPtr, getNode(BSIM3v32bNode), GET_INSERTED_NODE(BSIM3v32sNodePrime));
        ALLOC_VOLT(BSIM3v32DPspPtr, GET_INSERTED_NODE(BSIM3v32dNodePrime), GET_INSERTED_NODE(BSIM3v32sNodePrime));
        ALLOC_VOLT(BSIM3v32DPdPtr, GET_INSERTED_NODE(BSIM3v32dNodePrime), getNode(BSIM3v32dNode));
        ALLOC_VOLT(BSIM3v32BgPtr, getNode(BSIM3v32bNode), getNode(BSIM3v32gNode));
        ALLOC_VOLT(BSIM3v32DPgPtr, GET_INSERTED_NODE(BSIM3v32dNodePrime), getNode(BSIM3v32gNode));
        ALLOC_VOLT(BSIM3v32SPgPtr, GET_INSERTED_NODE(BSIM3v32sNodePrime), getNode(BSIM3v32gNode));
        ALLOC_VOLT(BSIM3v32SPsPtr, GET_INSERTED_NODE(BSIM3v32sNodePrime), getNode(BSIM3v32sNode));
        ALLOC_VOLT(BSIM3v32DPbPtr, GET_INSERTED_NODE(BSIM3v32dNodePrime), getNode(BSIM3v32bNode));
        ALLOC_VOLT(BSIM3v32SPbPtr, GET_INSERTED_NODE(BSIM3v32sNodePrime), getNode(BSIM3v32bNode));
        ALLOC_VOLT(BSIM3v32SPdpPtr, GET_INSERTED_NODE(BSIM3v32sNodePrime), GET_INSERTED_NODE(BSIM3v32dNodePrime));

        if (this->BSIM3v32nqsMod)
        {
            ALLOC_VOLT(BSIM3v32QqPtr, GET_INSERTED_NODE(BSIM3v32qNode), GET_INSERTED_NODE(BSIM3v32qNode));

            ALLOC_VOLT(BSIM3v32QdpPtr, GET_INSERTED_NODE(BSIM3v32qNode), GET_INSERTED_NODE(BSIM3v32dNodePrime));
            ALLOC_VOLT(BSIM3v32QspPtr, GET_INSERTED_NODE(BSIM3v32qNode), GET_INSERTED_NODE(BSIM3v32sNodePrime));
            ALLOC_VOLT(BSIM3v32QgPtr, GET_INSERTED_NODE(BSIM3v32qNode), getNode(BSIM3v32gNode));
            ALLOC_VOLT(BSIM3v32QbPtr, GET_INSERTED_NODE(BSIM3v32qNode), getNode(BSIM3v32bNode));
            ALLOC_VOLT(BSIM3v32DPqPtr, GET_INSERTED_NODE(BSIM3v32dNodePrime), GET_INSERTED_NODE(BSIM3v32qNode));
            ALLOC_VOLT(BSIM3v32SPqPtr, GET_INSERTED_NODE(BSIM3v32sNodePrime), GET_INSERTED_NODE(BSIM3v32qNode));
            ALLOC_VOLT(BSIM3v32GqPtr, getNode(BSIM3v32gNode), GET_INSERTED_NODE(BSIM3v32qNode));
            ALLOC_VOLT(BSIM3v32BqPtr, getNode(BSIM3v32bNode), GET_INSERTED_NODE(BSIM3v32qNode));
        }
        else
        {
            this->BSIM3v32QqPtr = nullptr;
            this->BSIM3v32QdpPtr = nullptr;
            this->BSIM3v32QspPtr = nullptr;
            this->BSIM3v32QgPtr = nullptr;
            this->BSIM3v32QbPtr = nullptr;
            this->BSIM3v32DPqPtr = nullptr;
            this->BSIM3v32SPqPtr = nullptr;
            this->BSIM3v32GqPtr = nullptr;
            this->BSIM3v32BqPtr = nullptr;
        }
        return 0;
    }
    int BSIM3V32::prepareOP()
    {
        UPDATE_RC(prepareDC());
        return 0;
    }
    int BSIM3V32::prepareTrOP()
    {
        UPDATE_RC(prepareDC());
        return 0;
    }
    int BSIM3V32::prepareAC()
    {
        UPDATE_RC(prepareDC());
        return 0;
    }
    int BSIM3V32::prepareTR()
    {
        UPDATE_RC(prepareDC());
        registerAdoptStep();
        tranFirstStep = true;
        return 0;
    }

    int BSIM3V32::iterateOP()
    {
        return stampDCTran(getEnvironment());
    }

    int BSIM3V32::iterateTrOP()
    {
        return stampDCTran(getEnvironment());
    }

    int BSIM3V32::iterateDC()
    {
#ifdef VERBOSE_DEBUG
        std::cout << "mod=" << std::hex << getEnvironment()->getCKTmode() << std::endl;
#endif
        return stampDCTran(getEnvironment());
    }

    int BSIM3V32::iterateAC(double omega)
    {
        return stampAC(getEnvironment(), omega);
    }

    int BSIM3V32::iterateTR(double tTime)
    {
        if (tranFirstStep)
        {
            for (int i = 0; i < BSIM3v32numStates; i++)
            {
                getIntegratorX(i)->set(1, getIntegratorX(i)->get(0));
                getIntegratorY(i)->set(1, getIntegratorY(i)->get(0));
            }
            tranFirstStep = false;
        }
        return stampDCTran(getEnvironment());
    }

    int BSIM3V32::saveOP()
    {
        return stampDCTran(getEnvironment());
    }

    int BSIM3V32::loadTempature()
    {
        if (!model)
            return CERR_NO_MDL;
        return model->setTemp(this, getEnvironment());
    }

    void BSIM3V32::adaptStep(double *step)
    {
        spiceTruncErr(getIntegratorX(BSIM3v32qb),
                      getIntegratorY(BSIM3v32qb),
                      getIntegratorTimestep(),
                      getIntegratorTruncErrorCoeff(),
                      getEnvironment()->getAbsTol(),
                      getEnvironment()->getRelTol(),
                      getEnvironment()->getChargeTol(),
                      getEnvironment()->getTranTol(),
                      getIntegratorOrder(),
                      step);
        spiceTruncErr(getIntegratorX(BSIM3v32qg),
                      getIntegratorY(BSIM3v32qg),
                      getIntegratorTimestep(),
                      getIntegratorTruncErrorCoeff(),
                      getEnvironment()->getAbsTol(),
                      getEnvironment()->getRelTol(),
                      getEnvironment()->getChargeTol(),
                      getEnvironment()->getTranTol(),
                      getIntegratorOrder(),
                      step);
        spiceTruncErr(getIntegratorX(BSIM3v32qd),
                      getIntegratorY(BSIM3v32qd),
                      getIntegratorTimestep(),
                      getIntegratorTruncErrorCoeff(),
                      getEnvironment()->getAbsTol(),
                      getEnvironment()->getRelTol(),
                      getEnvironment()->getChargeTol(),
                      getEnvironment()->getTranTol(),
                      getIntegratorOrder(),
                      step);
    }
}

extern "C" csimModel::ModelBase *createModel(MODELBASE_CONSTRUCTOR_DEF)
{
    return new csimModel::BSIM3V32(MODELBASE_CONSTRUCTOR_VAR);
}

extern "C" void deleteModel(csimModel::ModelBase *model)
{
    delete model;
}

extern "C" const ModelDescriptor descriptor = {
    /* id */
    "BSIM3V32",
    /* description */
    "BSIM3 V3.2 (Berkeley Short Channel IGFET Model Version-3)"};
