#ifndef TMATS_TYPES_H
#define TMATS_TYPES_H

/* Error code indices for IWork vector */
typedef enum {Er1, Er2, Er3, Er4, Er5, Er6, Er7, Er8, Er9,
              Er10, Er11, Er12, Er13, Er14, Er15, Er16} IWorkIdx;

/* Turbine block parameters structure */
struct TurbStruct {
    double s_T_Nc;
    double s_T_PR;
    double s_T_Wc;
    double s_T_Eff;
    double NcDes;
    double PRmapDes;
    double EffDes;
    double NDes;
    double IDes;
    int    BldPosLeng;
    int    CoolFlwEn;
    int    ConfigNPSS;

    /* Vector & array data */
    double *Y_T_NcVec;
    double *X_T_PRVec;
    double *T_T_Map_WcArray;
    double *T_T_Map_EffArray;
    double *T_BldPos;
    char   *BlkNm;
    int    *IWork;

    /* Dimensions of parameter arrays */
    int Y_T_NcVecLen;
    int X_T_PRVecLen;
    int WcMapCol;
    int EffMapCol;
    int WcMapRw;
    int EffMapRw;
};
typedef struct TurbStruct TurbStruct;

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
	double SwitchType;
	double flowLoss;
	double IDes;
	double WDes;
	double CfgEn;

	/* Vector & array data */
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
    char   *BlkNm;
    int    *IWork;

	/* Dimensions of parameter arrays */
	int Y_N_FARVecLen;
	int X_N_TtVecLen;
	int X_N_PEQPaVecLen;
	int X_N_TtVecTGLen;
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
    
    /* Vector & array data */
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
    char   *BlkNm;
    int    *IWork;
    
    /* Dimensions of parameter arrays */
    int Y_C_Map_NcVecLen;
    int X_C_RlineVecLen;
    int Z_C_AlphaVecLen;
    int X_C_Map_WcSurgeVecLen;
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
