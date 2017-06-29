#ifndef TMATS_TYPES_H
#define TMATS_TYPES_H

/* Turbine block parameters structure */
struct TurbStruct {
    double s_T_Nc;              /* = *mxGetPr(s_T_Nc_p(S)); */
    double s_T_PR;              /* = *mxGetPr(s_T_PR_p(S)); */
    double s_T_Wc;              /* = *mxGetPr(s_T_Wc_p(S)); */
    double s_T_Eff;             /* = *mxGetPr(s_T_Eff_p(S)); */
    double NcDes;               /* = *mxGetPr(NcDes_p(S)); */
    double PRmapDes;            /* = *mxGetPr(PRmapDes_p(S)); */
    double EffDes;              /* = *mxGetPr(EffDes_p(S)); */
    double NDes;                /* = *mxGetPr(NDes_p(S)); */
    double IDes;                /* = *mxGetPr(IDesign_p(S)); */
    int    BldPosLeng;          /* = *mxGetPr(BldPosLeng_p(S)); */
    int    CoolFlwEn;           /* = *mxGetPr(CoolFlwEn_p(S)); */
    int    ConfigNPSS;          /* = *mxGetPr(ConfigNPSS_p(S)); */
    
    /* vector & array data */
    double *Y_T_NcVec;            /* = mxGetPr(Y_T_NcVec_p(S)); */
    double *X_T_PRVec;            /* = mxGetPr(X_T_PRVec_p(S)); */
    double *T_T_Map_WcArray;      /* = mxGetPr(T_T_Map_WcArray_p(S)); */
    double *T_T_Map_EffArray;     /* = mxGetPr(T_T_Map_EffArray_p(S)); */
    double *T_BldPos;             /* = mxGetPr(T_BldPos_p(S)); */
    
    int WcMapCol;   /* = *mxGetPr(WcMapCol_p(S)); */
    int EffMapCol;  /* = *mxGetPr(EffMapCol_p(S)); */
    int WcMapRw;    /* = *mxGetPr(WcMapRw_p(S));    */
    int EffMapRw;   /* = *mxGetPr(EffMapRw_p(S)); */
    
    /*------get dimensions of parameter arrays-------*/
    int A;   /* = mxGetNumberOfElements(Y_T_NcVec_p(S)); */
    int B;   /* = mxGetNumberOfElements(X_T_PRVec_p(S)); */
    
    char *BlkNm;
    int *IWork;
};
typedef struct TurbStruct TurbStruct;

/* create enumeration for Iwork */
typedef enum {Er1=0, Er2 , Er3 , Er4 , Er5 , NUM_IWORK}IWorkIdx;

/* Burner block parameters structure */
struct BurnStruct {
    double LHV;
    double dPnormBurner;
    double Eff;
    double LHVEn;
    double hFuel;
};
typedef struct BurnStruct BurnStruct;

/* Nozzle block parameters structure */
struct NozzleStruct {
	double SwitchType; /* Nozzle type */
	double flowLoss;
	double IDes;
	double WDes;
	double CfgEn;

	/* vector & array data */
	double *Y_N_FARVec;
	double *T_N_RtArray;
	double *X_N_TtVec;
	double *T_N_MAP_gammaArray;
	double *X_N_PEQPaVec;
	double *T_N_CdThArray;
	double *T_N_CvArray;
	double *T_N_CfgArray;
	double *T_N_TGArray;
	double *X_N_TtVecTG;

	/*------get dimensions of parameter arrays-------*/
	int A;
	int B;
	int B1;
	int C;
};
typedef struct NozzleStruct NozzleStruct;

/* Compressor block parameters structure */
struct CompStruct {
    double NcDes;
    double PRDes;
    double EffDes;
    double RlineDes;
    double IDes;
    double CustBldEn;
    double FBldEn;
    double CustBldNm;
    double FracBldNm;
    
    /* vector & array data */
    double *Y_C_Map_NcVec;
    double *X_C_RlineVec;
    double *Z_C_AlphaVec;
    double *T_C_Map_WcArray;
    double *T_C_Map_PRArray;
    double *T_C_Map_EffArray;
    
    double *FracCusBldht;
    double *FracCusBldPt;
    double *FracBldht;
    double *FracBldPt;
    
    double *X_C_Map_WcSurgeVec;
    double *T_C_Map_PRSurgeVec;
    
    /*------get dimensions of parameter arrays-------*/
    int A;
    int B;
    int C;
    int D;
    
    int WcMapCol;
    int PRMapCol;
    int EffMapCol;
    int WcMapRw;
    int PRMapRw;
    int EffMapRw;
    int WcMapLay;
    int PRMapLay;
    int EffMapLay;
};
typedef struct CompStruct CompStruct;

#endif /* TMATS_TYPES_H */
