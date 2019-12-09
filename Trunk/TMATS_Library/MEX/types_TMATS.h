#ifndef TMATS_TYPES_H
#define TMATS_TYPES_H

/* Error code indices for IWork vector */
typedef enum {Er1, Er2, Er3, Er4, Er5, Er6, Er7, Er8, Er9,
              Er10, Er11, Er12, Er13, Er14, Er15, Er16, Er17} IWorkIdx;

/* Turbine block parameters structure */
struct TurbineStruct {
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
    int A;
    int B;
    int WcMapCol;
    int EffMapCol;
    int WcMapRw;
    int EffMapRw;
};
typedef struct TurbineStruct TurbineStruct;

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
    int A;
    int B;
    int B1;
    int C;
};
typedef struct NozzleStruct NozzleStruct;

/* Compressor block parameters structure */
struct CompressorStruct {
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
typedef struct CompressorStruct CompressorStruct;

/* Mixer block parameters structure */
struct MixerStruct {
    double Aphy1In;
    double Aphy2In;
    double s_M_Imp1;
    double s_M_Imp2;
    double s_M_V1;
    double s_M_V2;
    double BPRdes;
    double IDes;
    double SWPS;
    double MNp;
    
    /* Vector & array data */
    double *Y_M_FARVec;
    double *X_M_TVec;
    double *T_M_RtArray;
    double *T_M_gammaArray;
    char   *BlkNm;
    int    *IWork;

    /* Dimensions of parameter arrays */
    int A;
    int B;
};
typedef struct MixerStruct MixerStruct;

struct ValveStruct {
    double VlvfullyOpen;
    double VlvdeadZone;
    double Valve_Ae;

    /* Vector & array data */
    double *X_V_PRVec;
    double *T_V_WcVec;
    char   *BlkNm;
    int    *IWork;

    /* Dimensions of parameter arrays */
    int A;
};
typedef struct ValveStruct ValveStruct;

struct AmbientStruct{
    double AFARc;
    
    /* Vector & array data */
    double *X_A_AltVec;
    double *T_A_TsVec;
    double *T_A_PsVec;
    double *X_A_FARVec;
    double *T_A_RtArray;
    double *Y_A_TVec;
    double *T_A_gammaArray;
    char *BlkNm;
    int *IWork;
    
    /* Dimensions of parameter arrays */
    int A;
    int B;
    int C;
    
};
typedef struct AmbientStruct AmbientStruct;

struct BleedStruct{
    int WfracLen;
    
    /* Vector & array data */
    double *Wfrac;
    char *BlkNm;
    int *IWork;
};
typedef struct BleedStruct BleedStruct;

struct StaticCalcStruct{
    double AthroatIn;
    double MNIn;
    int SolveType;
    
    /* Vector & array data */
    double *X_FARVec;
    double *T_RtArray;
    double *Y_TtVec;
    double *T_gammaArray;
    char *BlkNm;
    int *IWork;
    
    /* Dimensions of parameter arrays */
    int A;
    int B;
};
typedef struct StaticCalcStruct StaticCalcStruct;

struct Turbine_PSIStruct{
    double NcDes;
    double PRmapDes;
    double EffDes;
    double NDes;
    double IDes;
    double s_T_hi;
    double gamma_T;
    double Rt_T;
    int BldPosLeng;
    int CoolFlwEn;
    
    /* Vector & array data */
    double *Y_T_NcpsiVec;
    double *X_T_PRpsiVec;
    double *Y_T_NcwowVec;
    double *X_T_PRwowVec;
    double *T_T_Map_WoWArray;
    double *T_T_Map_psiArray;
    double *T_BldPos;
    char *BlkNm;
    int *IWork;
    
    /* Dimensions of parameter arrays */
    int A;
    int B;
    int C;
    int D;
};
typedef struct Turbine_PSIStruct Turbine_PSIStruct;

struct ValvePHYStruct{
    double s_V_Ae_vlv;
    double s_V_Ae_byp;
    double s_V_Ae_th;
    
    /* Vector & array data */
    double *X_V_FAR_vec;
    double *T_V_Rt_vec;
    double *Y_V_Tt_vec;
    double *T_V_gamma_array;
    char *BlkNm;
    int *IWork;
    
    /* Dimensions of parameter arrays */
    int A1;
    int B1;

};
typedef struct ValvePHYStruct ValvePHYStruct;

#endif /* TMATS_TYPES_H */
