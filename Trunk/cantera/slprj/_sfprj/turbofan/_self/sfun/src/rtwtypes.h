
#ifndef __RTWTYPES_H__  
  #define __RTWTYPES_H__  
  #include "tmwtypes.h"  
  
/* This ID is used to detect inclusion of an incompatible rtwtypes.h */
#define RTWTYPES_ID_C08S16I32L32N32F1
  
  #include "simstruc_types.h"  
    #ifndef POINTER_T  
    # define POINTER_T  
      typedef void * pointer_T;  
    #endif  
    #ifndef TRUE  
    # define TRUE (1U)  
    #endif  
    #ifndef FALSE  
    # define FALSE (0U)  
    #endif  

/*
 * MultiWord supporting definitions
 */
typedef long int long_T;

/*
 * MultiWord types
 */


typedef struct {
  uint32_T chunks[2];
} int64m_T;

typedef struct {
  int64m_T re;
  int64m_T im;
} cint64m_T;



typedef struct {
  uint32_T chunks[2];
} uint64m_T;

typedef struct {
  uint64m_T re;
  uint64m_T im;
} cuint64m_T;



typedef struct {
  uint32_T chunks[3];
} int96m_T;

typedef struct {
  int96m_T re;
  int96m_T im;
} cint96m_T;



typedef struct {
  uint32_T chunks[3];
} uint96m_T;

typedef struct {
  uint96m_T re;
  uint96m_T im;
} cuint96m_T;



typedef struct {
  uint32_T chunks[4];
} int128m_T;

typedef struct {
  int128m_T re;
  int128m_T im;
} cint128m_T;



typedef struct {
  uint32_T chunks[4];
} uint128m_T;

typedef struct {
  uint128m_T re;
  uint128m_T im;
} cuint128m_T;



typedef struct {
  uint32_T chunks[5];
} int160m_T;

typedef struct {
  int160m_T re;
  int160m_T im;
} cint160m_T;



typedef struct {
  uint32_T chunks[5];
} uint160m_T;

typedef struct {
  uint160m_T re;
  uint160m_T im;
} cuint160m_T;



typedef struct {
  uint32_T chunks[6];
} int192m_T;

typedef struct {
  int192m_T re;
  int192m_T im;
} cint192m_T;



typedef struct {
  uint32_T chunks[6];
} uint192m_T;

typedef struct {
  uint192m_T re;
  uint192m_T im;
} cuint192m_T;



typedef struct {
  uint32_T chunks[7];
} int224m_T;

typedef struct {
  int224m_T re;
  int224m_T im;
} cint224m_T;



typedef struct {
  uint32_T chunks[7];
} uint224m_T;

typedef struct {
  uint224m_T re;
  uint224m_T im;
} cuint224m_T;



typedef struct {
  uint32_T chunks[8];
} int256m_T;

typedef struct {
  int256m_T re;
  int256m_T im;
} cint256m_T;



typedef struct {
  uint32_T chunks[8];
} uint256m_T;

typedef struct {
  uint256m_T re;
  uint256m_T im;
} cuint256m_T;



typedef struct {
  uint32_T chunks[9];
} int288m_T;

typedef struct {
  int288m_T re;
  int288m_T im;
} cint288m_T;



typedef struct {
  uint32_T chunks[9];
} uint288m_T;

typedef struct {
  uint288m_T re;
  uint288m_T im;
} cuint288m_T;



typedef struct {
  uint32_T chunks[10];
} int320m_T;

typedef struct {
  int320m_T re;
  int320m_T im;
} cint320m_T;



typedef struct {
  uint32_T chunks[10];
} uint320m_T;

typedef struct {
  uint320m_T re;
  uint320m_T im;
} cuint320m_T;



typedef struct {
  uint32_T chunks[11];
} int352m_T;

typedef struct {
  int352m_T re;
  int352m_T im;
} cint352m_T;



typedef struct {
  uint32_T chunks[11];
} uint352m_T;

typedef struct {
  uint352m_T re;
  uint352m_T im;
} cuint352m_T;



typedef struct {
  uint32_T chunks[12];
} int384m_T;

typedef struct {
  int384m_T re;
  int384m_T im;
} cint384m_T;



typedef struct {
  uint32_T chunks[12];
} uint384m_T;

typedef struct {
  uint384m_T re;
  uint384m_T im;
} cuint384m_T;



typedef struct {
  uint32_T chunks[13];
} int416m_T;

typedef struct {
  int416m_T re;
  int416m_T im;
} cint416m_T;



typedef struct {
  uint32_T chunks[13];
} uint416m_T;

typedef struct {
  uint416m_T re;
  uint416m_T im;
} cuint416m_T;



typedef struct {
  uint32_T chunks[14];
} int448m_T;

typedef struct {
  int448m_T re;
  int448m_T im;
} cint448m_T;



typedef struct {
  uint32_T chunks[14];
} uint448m_T;

typedef struct {
  uint448m_T re;
  uint448m_T im;
} cuint448m_T;



typedef struct {
  uint32_T chunks[15];
} int480m_T;

typedef struct {
  int480m_T re;
  int480m_T im;
} cint480m_T;



typedef struct {
  uint32_T chunks[15];
} uint480m_T;

typedef struct {
  uint480m_T re;
  uint480m_T im;
} cuint480m_T;



typedef struct {
  uint32_T chunks[16];
} int512m_T;

typedef struct {
  int512m_T re;
  int512m_T im;
} cint512m_T;



typedef struct {
  uint32_T chunks[16];
} uint512m_T;

typedef struct {
  uint512m_T re;
  uint512m_T im;
} cuint512m_T;



typedef struct {
  uint32_T chunks[17];
} int544m_T;

typedef struct {
  int544m_T re;
  int544m_T im;
} cint544m_T;



typedef struct {
  uint32_T chunks[17];
} uint544m_T;

typedef struct {
  uint544m_T re;
  uint544m_T im;
} cuint544m_T;



typedef struct {
  uint32_T chunks[18];
} int576m_T;

typedef struct {
  int576m_T re;
  int576m_T im;
} cint576m_T;



typedef struct {
  uint32_T chunks[18];
} uint576m_T;

typedef struct {
  uint576m_T re;
  uint576m_T im;
} cuint576m_T;



typedef struct {
  uint32_T chunks[19];
} int608m_T;

typedef struct {
  int608m_T re;
  int608m_T im;
} cint608m_T;



typedef struct {
  uint32_T chunks[19];
} uint608m_T;

typedef struct {
  uint608m_T re;
  uint608m_T im;
} cuint608m_T;



typedef struct {
  uint32_T chunks[20];
} int640m_T;

typedef struct {
  int640m_T re;
  int640m_T im;
} cint640m_T;



typedef struct {
  uint32_T chunks[20];
} uint640m_T;

typedef struct {
  uint640m_T re;
  uint640m_T im;
} cuint640m_T;



typedef struct {
  uint32_T chunks[21];
} int672m_T;

typedef struct {
  int672m_T re;
  int672m_T im;
} cint672m_T;



typedef struct {
  uint32_T chunks[21];
} uint672m_T;

typedef struct {
  uint672m_T re;
  uint672m_T im;
} cuint672m_T;



typedef struct {
  uint32_T chunks[22];
} int704m_T;

typedef struct {
  int704m_T re;
  int704m_T im;
} cint704m_T;



typedef struct {
  uint32_T chunks[22];
} uint704m_T;

typedef struct {
  uint704m_T re;
  uint704m_T im;
} cuint704m_T;



typedef struct {
  uint32_T chunks[23];
} int736m_T;

typedef struct {
  int736m_T re;
  int736m_T im;
} cint736m_T;



typedef struct {
  uint32_T chunks[23];
} uint736m_T;

typedef struct {
  uint736m_T re;
  uint736m_T im;
} cuint736m_T;



typedef struct {
  uint32_T chunks[24];
} int768m_T;

typedef struct {
  int768m_T re;
  int768m_T im;
} cint768m_T;



typedef struct {
  uint32_T chunks[24];
} uint768m_T;

typedef struct {
  uint768m_T re;
  uint768m_T im;
} cuint768m_T;



typedef struct {
  uint32_T chunks[25];
} int800m_T;

typedef struct {
  int800m_T re;
  int800m_T im;
} cint800m_T;



typedef struct {
  uint32_T chunks[25];
} uint800m_T;

typedef struct {
  uint800m_T re;
  uint800m_T im;
} cuint800m_T;



typedef struct {
  uint32_T chunks[26];
} int832m_T;

typedef struct {
  int832m_T re;
  int832m_T im;
} cint832m_T;



typedef struct {
  uint32_T chunks[26];
} uint832m_T;

typedef struct {
  uint832m_T re;
  uint832m_T im;
} cuint832m_T;



typedef struct {
  uint32_T chunks[27];
} int864m_T;

typedef struct {
  int864m_T re;
  int864m_T im;
} cint864m_T;



typedef struct {
  uint32_T chunks[27];
} uint864m_T;

typedef struct {
  uint864m_T re;
  uint864m_T im;
} cuint864m_T;



typedef struct {
  uint32_T chunks[28];
} int896m_T;

typedef struct {
  int896m_T re;
  int896m_T im;
} cint896m_T;



typedef struct {
  uint32_T chunks[28];
} uint896m_T;

typedef struct {
  uint896m_T re;
  uint896m_T im;
} cuint896m_T;



typedef struct {
  uint32_T chunks[29];
} int928m_T;

typedef struct {
  int928m_T re;
  int928m_T im;
} cint928m_T;



typedef struct {
  uint32_T chunks[29];
} uint928m_T;

typedef struct {
  uint928m_T re;
  uint928m_T im;
} cuint928m_T;



typedef struct {
  uint32_T chunks[30];
} int960m_T;

typedef struct {
  int960m_T re;
  int960m_T im;
} cint960m_T;



typedef struct {
  uint32_T chunks[30];
} uint960m_T;

typedef struct {
  uint960m_T re;
  uint960m_T im;
} cuint960m_T;



typedef struct {
  uint32_T chunks[31];
} int992m_T;

typedef struct {
  int992m_T re;
  int992m_T im;
} cint992m_T;



typedef struct {
  uint32_T chunks[31];
} uint992m_T;

typedef struct {
  uint992m_T re;
  uint992m_T im;
} cuint992m_T;



typedef struct {
  uint32_T chunks[32];
} int1024m_T;

typedef struct {
  int1024m_T re;
  int1024m_T im;
} cint1024m_T;



typedef struct {
  uint32_T chunks[32];
} uint1024m_T;

typedef struct {
  uint1024m_T re;
  uint1024m_T im;
} cuint1024m_T;

#endif /* __RTWTYPES_H__ */  
