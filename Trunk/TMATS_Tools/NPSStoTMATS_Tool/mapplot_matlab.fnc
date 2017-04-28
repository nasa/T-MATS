//
// NPSS functions used for turbomachinery component map conversion
//
// Written by Scott Jones for python
// modified by Jeff Chin for matlab
// further customized by Jeffryes Chapman for T-MATS
//
//
//-------------------------------------------------------------------------
// declare GLOBAL variables (used in both functions)
//-------------------------------------------------------------------------

// Comment these out to get MATLAB map data for original maps.
// Uncomment them to extend the maps to speed lines of 20% and/or PR = 1.
//#define EXTEND_FAN_MAP
//#define EXTEND_TURBINE_MAPS

string COMP_NAMES[];   // names of all the compressors
string TURB_NAMES[];   // names of all the turbines
string Amb_NAME;       // name of the Ambient or FlightConditions element used for MN & alt
string Perf_NAME;      // name of the EngPerf element used for PC
string mapType;        // compressor map type - Rline or Beta
//int firstcall = 1;     // flag for the first call to function saveOpPoint()
//int nosave = 0;        // flag to prevent plotting of operating points


void createMapDataFiles_matlab() {

   //-------------------------------------------------------------------------
   // declare DATA streams and variables
   //-------------------------------------------------------------------------
   OutFileStream COMPONENT_LIST { }    // list of turbomachinery components
   OutFileStream DATA_MAP { }          // component map data stream
   int n;                              // index number for turbomachinery maps


   //-------------------------------------------------------------------------
   // create a list of all compressors and turbines in this model
   //-------------------------------------------------------------------------
   COMP_NAMES = list( "Compressor", TRUE );
   COMP_NAMES.append(list( "CompressorG", TRUE));
   TURB_NAMES = list( "Turbine", TRUE );
   TURB_NAMES.append(list( "TurbineTwo", FALSE ));
   Amb_NAME = "myFreeStream";
   Perf_NAME = "Eng";
   //Amb_NAME = list( "FlightConditions", TRUE );
   //Amb_NAME = list( "FlightConditions" || "Ambient", TRUE );

   //-------------------------------------------------------------------------
   // write the start of the component list file as a Matlab variable
   //-------------------------------------------------------------------------

   COMPONENT_LIST.filename = "Setup_TMATS/mapCompList.txt";
   COMPONENT_LIST << "component_list=[ ";

   for ( n=0; n < COMP_NAMES.entries(); ++n ) {
      COMPONENT_LIST << "\"" << "mapData" + COMP_NAMES[n] + ".txt" << "\", ";
   }
   for ( n=0; n < TURB_NAMES.entries(); ++n ) {
      COMPONENT_LIST << "\"" << "mapData" + TURB_NAMES[n] + ".txt" << "\", ";
   }

   COMPONENT_LIST << "\"done\" ]";
   COMPONENT_LIST.close();


   //-------------------------------------------------------------------------
   // for each compressor, write its map data to a file in MATLAB format
   //-------------------------------------------------------------------------
   for ( n=0; n < COMP_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // declare map index variables, their meaning depends on map type:
      //
      //      map type:      RLINE        BETA
      //       index i       alpha       alpha       -> alphaVals
      //       index j       speed        beta       -> betaVals
      //       index k       rline       speed       -> gammaVals
      //----------------------------------------------------------------------
      int i,j,k;
      real alphaVals[];                          // array of alpha values
      real betaVals[];                           // array of beta/speed values
      real gammaVals[];                          // array of speed/rline values
      real mapXK1, mapXK2, mapPRref, mapWCref;   // BETA map constants
      real dataFlow, dataPR, dataEff;            // map dependent data values
      string componentName = COMP_NAMES[n] + "\0";

      // set the map data filename
      DATA_MAP.width = 0;

	   DATA_MAP.filename = "Setup_TMATS/mapData" + COMP_NAMES[n] + ".m";
      DATA_MAP << "%mapname = '" << COMP_NAMES[n] << "'" << endl;

      // determine if the compressor map is RLINE type or BETA type
      //if ( exists( COMP_NAMES[n] + ".S_map.S_eff" ) ) {
	  if ( exists( COMP_NAMES[n] + ".S_map.RlineMapDes" ) ) {
         mapType = "RLINE";
         string map = "parent." + COMP_NAMES[n] + ".S_map";
         string Tbl_flow = map + ".TB_Wc";
         string Tbl_PR   = map + ".TB_PR";
         string Tbl_eff  = map + ".TB_eff";
         string indepNames[] = Tbl_flow->getIndependentNames();
      }
      else {
         mapType = "BETA";
         string map = "parent." + COMP_NAMES[n] + ".S_map";
         string Tbl_flow = map + ".TB_Wc";
         string Tbl_eff  = map + ".TB_eff";
         string indepNames[] = Tbl_flow->getIndependentNames();
         mapXK1 = map->XK1;
         mapXK2 = map->XK2;
         mapPRref = map->PRMapDes;
         mapWCref = map->WcMapDes;
      }

      // write the map type, index and dependent variable headers to the file
      DATA_MAP << "%maptype = '" << mapType << "'" << endl << endl;
      if ( mapType == "BETA" ) {
         DATA_MAP << "%        alpha         beta        speed";
      }
      else {
         DATA_MAP << "%        alpha        speed        Rline";
      }
      DATA_MAP << "        Wcorr           PR          eff  \n";
      DATA_MAP.width = 2; DATA_MAP << "%mapdata=["; DATA_MAP << endl;


      //----------------------------------------------------------------------
      // write the data to the file in nested loops of i, j, and k
      //----------------------------------------------------------------------
      alphaVals = Tbl_flow->getValues( indepNames[0] );

      // for each alpha value
      for ( i=0; i < alphaVals.entries(); ++i ) {
         DATA_MAP.width = 1; DATA_MAP << "%alpha["; DATA_MAP << endl;

         // Extend fan map.
         #ifdef EXTEND_FAN_MAP
             if (componentName.first("fan") > -1)
             {
                betaVals = {0.30, 0.40};
                betaVals.append(Tbl_flow->getValues( indepNames[1], alphaVals[i] ));
             }
             else
             {
                betaVals = Tbl_flow->getValues( indepNames[1], alphaVals[i] );
             }
         #else
             betaVals = Tbl_flow->getValues( indepNames[1], alphaVals[i] );
         #endif

         // for each beta value
         for ( j=0; j < betaVals.entries(); ++j ) {
            DATA_MAP.width = 1; DATA_MAP << COMP_NAMES[n]<<"(:,"<<j+1<<",:,"<<i+1<<")="<< "[" ; DATA_MAP << "";
            gammaVals = Tbl_flow->getValues( indepNames[2], alphaVals[i], betaVals[j] );

            // for each speed value
            for ( k=0; k < gammaVals.entries(); ++k ) {
               DATA_MAP.width = 1; DATA_MAP << "";

               // get value of corrected flow
               dataFlow = Tbl_flow->eval( alphaVals[i], betaVals[j], gammaVals[k] );
               // get value of adiabatic efficiency
               dataEff = Tbl_eff->eval( alphaVals[i], betaVals[j], gammaVals[k] );

               // get value or calculate pressure ratio (Rline or BETA map type)
               if ( mapType == "BETA" ) {
                  dataPR = mapXK1 +( (mapPRref-mapXK1)*( (dataFlow/mapWCref)**1.43
                  - betaVals[j] + 1. ) ); }
               else {
                  dataPR = Tbl_PR->eval( alphaVals[i], betaVals[j], gammaVals[k] );
               }
               if (dataPR < 1.0)
               {
                  dataPR = 1.0;
               }
               if (dataEff < 0.0)
               {
                  dataEff = 0.0;
               }
               //dataFlow = fan.S_map.TB_Wc( alphaVals[i], betaVals[j], gammaVals[k] );
               //dataEff = fan.S_map.TB_eff( alphaVals[i], betaVals[j], gammaVals[k] );
               //dataPR = fan.S_map.TB_PR( alphaVals[i], betaVals[j], gammaVals[k] );

               // write alpha, beta, gamma, Wc, PR, and efficiency to the file
               DATA_MAP.showpoint = TRUE;
               DATA_MAP.precision = 6;
               DATA_MAP.width = 12;
               DATA_MAP << alphaVals[i];
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << betaVals[j];
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << gammaVals[k];
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << dataFlow;
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << dataPR;
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << dataEff;

               // close groups (lists) of data based on Matlab format
               if ( k+1 < gammaVals.entries() ) {
                  DATA_MAP.width = 2; DATA_MAP << ""; }
               else {
                  if ( j+1 < betaVals.entries() ) {
                     DATA_MAP.width = 3; DATA_MAP << "];"; }
                  else {
                     if ( i+1 < alphaVals.entries() ) {
                        DATA_MAP.width = 3; DATA_MAP << "];"; }
                     else {
                        DATA_MAP.width = 3; DATA_MAP << "];"; }
                  }
               }
               DATA_MAP << endl;
            }
         }
      }
      DATA_MAP << endl;

   }

   //-------------------------------------------------------------------------
   // for each turbine, write its map data to a file in Matlab format
   //-------------------------------------------------------------------------
   for ( n=0; n < TURB_NAMES.entries(); ++n ) {

      // set the map data filename
      DATA_MAP.width = 0;

	   DATA_MAP.filename = "Setup_TMATS/mapData" + TURB_NAMES[n] + ".m";
      DATA_MAP << "%mapname = '" << TURB_NAMES[n] << "'" << endl;

      // determine if the turbine map is type 1 or 2
      if ( exists( TURB_NAMES[n] + ".S_map.S_eff" ) ) {
         mapType = "TURBone";
         map = "parent." + TURB_NAMES[n] + ".S_map.S_eff";
         Tbl_flow = map + ".TB_Wp";
         Tbl_eff  = map + ".TB_eff";
         indepNames = Tbl_flow->getIndependentNames();
      }
      else {
         mapType = "TURBtwo";
         map = "parent." + TURB_NAMES[n] + ".S_map";
         Tbl_flow = map + ".TB_Wp";
         Tbl_eff  = map + ".TB_eff";
         indepNames = Tbl_flow->getIndependentNames();
      }

      // write the map type, index and dependent variable headers to the file
      DATA_MAP << "%maptype = '" << mapType << "'" << endl << endl;
      DATA_MAP << "%        alpha        speed           PR";
      DATA_MAP << "        Wcorr          eff  \n";
      DATA_MAP.width = 2; DATA_MAP << "%mapdata=["; DATA_MAP << endl;


      //----------------------------------------------------------------------
      // write the data to the file in nested loops of i, j, and k
      //----------------------------------------------------------------------

      int Numindep = indepNames.entries();

      if (Numindep == 2) {
      alphaVals = {0.0}; 
      }else{
      alphaVals = Tbl_flow->getValues( indepNames[0] );
      }

      // for each alpha value
      for ( i=0; i < alphaVals.entries(); ++i ) {
         DATA_MAP.width = 1; DATA_MAP << ""; DATA_MAP << endl;
         #ifdef EXTEND_TURBINE_MAPS
             betaVals = {20, 30, 40, 50};
         #else
             betaVals = {};
         #endif
         if (Numindep == 2){
         betaVals.append(Tbl_flow->getValues( indepNames[0] ));
         }else{
         betaVals.append(Tbl_flow->getValues( indepNames[1], alphaVals[i] ));
         }

         // for each beta value // beta is likely Nc for turbine.
         for ( j=0; j < betaVals.entries(); ++j ) {
            DATA_MAP.width = 1; DATA_MAP << TURB_NAMES[n]<<"(:,"<<j+1<<",:,"<<i+1<<")="<< "[" ; DATA_MAP << "";
            //gammaVals = Tbl_flow->getValues( indepNames[2], alphaVals[i], betaVals[j] );

            #ifdef EXTEND_TURBINE_MAPS
                gammaVals = {1.00, 1.25, 1.50, 1.75, 2.00, 2.25, 2.50, 2.75};
                
            #else
                gammaVals = {};
            #endif

            if (Numindep == 2){
                gammaVals.append(Tbl_flow->getValues( indepNames[1], betaVals[j] ));
            }else{
                gammaVals.append(Tbl_flow->getValues( indepNames[2], alphaVals[i], betaVals[j] ));
            }

            // for each speed value // gamma is likely PR for turbine.
            for ( k=0; k < gammaVals.entries(); ++k ) {
               DATA_MAP.width = 1; DATA_MAP << "";

               // get value of corrected flow and adiabatic efficiency
               if (Numindep == 2){
                    dataFlow = Tbl_flow->eval( betaVals[j], gammaVals[k] );
                    dataEff = Tbl_eff->eval( betaVals[j], gammaVals[k] );
               }else{
                    dataFlow = Tbl_flow->eval( alphaVals[i], betaVals[j], gammaVals[k] );
                    dataEff = Tbl_eff->eval( alphaVals[i], betaVals[j], gammaVals[k] );
               }

               dataPR = Tbl_PR->eval( alphaVals[i], betaVals[j], gammaVals[k] );

               // get value or calculate pressure ratio (Rline or BETA map type)
//             if ( mapType == "BETA" ) {
//                dataPR = mapXK1 +( (mapPRref-mapXK1)*( (dataFlow/mapWCref)**1.43
//                - betaVals[j] + 1. ) ); }
//             else {
//                dataPR = Tbl_PR->eval( alphaVals[i], betaVals[j], gammaVals[k] );
//             }

               // write alpha, beta, gamma, Wc, PR, and efficiency to the file
               DATA_MAP.showpoint = TRUE;
               DATA_MAP.precision = 6;
               DATA_MAP.width = 12;
               DATA_MAP << alphaVals[i];
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << betaVals[j];
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << gammaVals[k];
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << dataFlow;
//             DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
//             DATA_MAP << dataPR;
               DATA_MAP.width = 1; DATA_MAP << ","; DATA_MAP.width = 12;
               DATA_MAP << dataEff;

               // close groups (lists) of data based on Matlab format
               if ( k+1 < gammaVals.entries() ) {
                  DATA_MAP.width = 2; DATA_MAP << ""; }
               else {
                  if ( j+1 < betaVals.entries() ) {
                     DATA_MAP.width = 4; DATA_MAP << "];"; }
                  else {
                     if ( i+1 < alphaVals.entries() ) {
                        DATA_MAP.width = 6; DATA_MAP << "];"; }
                     else {
                        DATA_MAP.width = 7; DATA_MAP << "];"; }
                  }
               }
               DATA_MAP << endl;
            }
         }
      }
      DATA_MAP << endl;
   }
}
