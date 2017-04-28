//
// Gather NPSS function variables used in TMATS Blocks
// Written by Jeffryes Chapman based on work by Jeffrey Chin
//

void DataFilesMTLB() {

    string ELEMENTS[];     // names of elements
    string ISA[];          // type of each element
    string STATIONS[];     // names of flow stations

    string AMB_NAMES[];    // name of ambient element
    string INSTRT_NAMES[]; // name of inletstart element
    string BLEED_NAMES[];  // names of the bleed elements
    string BURN_NAMES[];   // names of the burners
    string COMP_NAMES[];   // names of the compressors
    string DUCT_NAMES[];   // names of ducts
    string GEARBX_NAMES[]; // names of gear boxes
    string INLET_NAMES[];  // names of inlets
    string MIXER_NAMES[];  // names of mixers
    string NOZ_NAMES[];    // names of nozzles
    string SHAFT_NAMES[];  // names of shafts
    string Shaft_Links[];  // names of station links to shaft
    string SPLIT_NAMES[];  // name of ambient element
    string BLEED_OUT[];    // names bleed outputs
    string BLEED_IN[];     // names bleed inputs
    string TURB_NAMES[];   // names of the turbines
    string FldNm;          // name of folder to place files
    string FileNm;         // name of file containing model variables
    string StrNm;          // name of matlab structure

   //-------------------------------------------------------------------------
   // declare DATA streams and variables
   //-------------------------------------------------------------------------
   OutFileStream DATA { }          // component map data stream
   int n;                          // index number 
   int i;                          // index number
   int Iter;                       // index number 

   //-------------------------------------------------------------------------
   // create a list of all elements in this model
   //-------------------------------------------------------------------------
   ELEMENTS   = list("Element",TRUE);
   STATIONS = list( "Link", TRUE );

   AMB_NAMES  = list("Ambient",TRUE);
   INSTRT_NAMES = list("InletStart",TRUE);
   BLEED_NAMES = list("Bleed",TRUE);
   BURN_NAMES = list( "Burner", TRUE );
   COMP_NAMES = list( "Compressor", TRUE ); 
   //COMP_NAMES.append(list( "CompressorG", TRUE));
   DUCT_NAMES = list("Duct",TRUE);
   GEARBX_NAMES = list("Gearbox",TRUE);
   INLET_NAMES = list("Inlet",TRUE);
   MIXER_NAMES = list("Mixer",TRUE);
   NOZ_NAMES = list("Nozzle",TRUE);
   SHAFT_NAMES = list( "Shaft", TRUE );
   SPLIT_NAMES = list("Splitter",TRUE);
   TURB_NAMES = list( "Turbine", TRUE ); 
   TURB_NAMES.append(list( "TurbineTwo", FALSE ));
   BLEED_IN = list("InterStageBleedInPort", TRUE); 
   BLEED_IN.append(list("BleedInPort",TRUE));  
   BLEED_OUT = list("InterStageBleedOutPort", TRUE); 
   BLEED_OUT.append(list("BleedOutPort",TRUE)); 
   FldNm = "Setup_TMATS";
   string FileNmNm = MODELNAME;
   string WorkSpaceNm = "MWS_" + FileNmNm;
   FileNm = FileNmNm + "_Setup";
   StrNm = "TMTS.";

// set the map data filename
      DATA.width = 0;
      DATA.filename = FldNm + "/" + FileNm + ".m";
      DATA <<"function "<<FileNm<< "()" <<endl;
      DATA << "%% This file contains T-MATS block variables gathered from an NPSS model " << MODELNAME <<endl;
      DATA <<StrNm<<"INFO.ModelName = '"<< MODELNAME <<"';" <<endl;
      DATA <<StrNm<<"INFO.WorkSpaceName = '"<<WorkSpaceNm <<"';" <<endl;
      DATA <<StrNm<<"INFO.DateTime = '"<<date<<" : "<<timeOfDay <<"';" <<endl;

//-------------------------------------------------------------------------
// for Ambient, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < AMB_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Amb_alt;                          // altitude
      real Amb_MN;                           // mach number
      real Amb_dT;                          // delta temperature

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for ambient " << AMB_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Amb_altNm = AMB_NAMES[n] + ".alt";
      Amb_alt = evalExpr(Amb_altNm);
      string Amb_MNNm = AMB_NAMES[n] + ".MN";
      Amb_MN = evalExpr(Amb_MNNm);
      string Amb_dTNm = AMB_NAMES[n] + ".dTs";
      Amb_dT = evalExpr(Amb_dTNm);

// Save data to files
      DATA <<StrNm<<AMB_NAMES[n]<<".type = '"<<evalExpr(AMB_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<AMB_NAMES[n]<<".Name = '"<<AMB_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<AMB_NAMES[n]<<".Alt = "<< Amb_alt <<";" <<endl;
      DATA <<StrNm<<AMB_NAMES[n]<<".MN = "<< Amb_MN <<";" <<endl;
      DATA <<StrNm<<AMB_NAMES[n]<<".dT = "<< Amb_dT <<";" <<endl;

      DATA << endl;
    }

//-------------------------------------------------------------------------
// for each Bleed, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < BLEED_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Bld_Wfrac;                                   // flow fraction
      string Bld_BleedOutPortNames[];                   // list of outports
      string Bld_BleedInPortNames[];                    // list of inports

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for bleed " << BLEED_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Burn_BleedOutPortsCmd = BLEED_NAMES[n] + ".bleedOutPortList";
      Bld_BleedOutPortNames = evalExpr(Burn_BleedOutPortsCmd);
      string Burn_BleedInPortsCmd = BLEED_NAMES[n] + ".bleedInPortList";
      Bld_BleedInPortNames = evalExpr(Burn_BleedInPortsCmd);

// Save data to files      
      DATA <<StrNm<<BLEED_NAMES[n]<<".type = '"<<evalExpr(BLEED_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<BLEED_NAMES[n]<<".Name = '"<<BLEED_NAMES[n]<<"';" <<endl;

      DATA <<StrNm<<BLEED_NAMES[n]<<".Wfrac_M = [";
      for ( i=0; i < Bld_BleedOutPortNames.entries(); ++i ) {
        string Bld_WfracCmd = BLEED_NAMES[n] + "." + Bld_BleedOutPortNames[i] + ".fracW";
        Bld_Wfrac = evalExpr(Bld_WfracCmd);
        DATA <<" "<<Bld_Wfrac;
      }
      DATA <<"];"<<endl;

      DATA <<StrNm<<BLEED_NAMES[n]<<".BleedOutNames = {";
      for ( i=0; i < Bld_BleedOutPortNames.entries(); ++i ) {
        DATA <<" '"<<Bld_BleedOutPortNames[i]<<"'";
      }
      DATA <<"};"<<endl;

      DATA <<StrNm<<BLEED_NAMES[n]<<".BleedInNames = {";
      for ( i=0; i < Bld_BleedInPortNames.entries(); ++i ) {
        DATA <<" '"<<Bld_BleedInPortNames[i]<<"'";
      }
      DATA <<"};"<<endl;

      DATA << endl;
    }



//-------------------------------------------------------------------------
// for each Burner, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < BURN_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Burn_dPqP;                          // pressure loss
      real Burn_eff;                           // burner efficiency
      real Burn_LHV;                           // fuel lower heating value
      real Burn_Wf;                            // fuel mass flow

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for burner " << BURN_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Burn_dPqPNm = BURN_NAMES[n] + ".dPqP";
      Burn_dPqP = evalExpr(Burn_dPqPNm);
      string Burn_effNm = BURN_NAMES[n] + ".eff";
      Burn_eff = evalExpr(Burn_effNm);
      string Burn_LHVNm = BURN_NAMES[n] + ".Fu_I.LHV";
      Burn_LHV = evalExpr(Burn_LHVNm);
      string Burn_WfNm = BURN_NAMES[n] + ".Wfuel";
      Burn_Wf = evalExpr(Burn_WfNm);


// Save data to files
      DATA <<StrNm<<BURN_NAMES[n]<<".type = '"<<evalExpr(BURN_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<BURN_NAMES[n]<<".Name = '"<<BURN_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<BURN_NAMES[n]<<".dPnormBurner_M = "<< Burn_dPqP <<";" <<endl;
      DATA <<StrNm<<BURN_NAMES[n]<<".Eff_M = "<< Burn_eff <<";" <<endl;
      DATA <<"% Assume burner energy calculated using lower heating value (LHV)" <<endl;
      DATA <<StrNm<<BURN_NAMES[n]<<".LHV_M = "<< Burn_LHV <<";" <<endl;
      DATA <<StrNm<<BURN_NAMES[n]<<".Wf = "<< Burn_Wf <<";" <<endl;
      DATA << endl;
    }


//-------------------------------------------------------------------------
// for each Compressor, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < COMP_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      real Cmp_NcMapDes;                          // Map design Nc
      real Cmp_effDes;                            // Design eff
      real Cmp_PRdes;                             // Design PR
      real Cmp_RlineDes;                          // Design Rline

      real Cmp_s_Nc;                              // Nc scalar
      real Cmp_s_Wc;                              // Wc scalar
      real Cmp_s_PR;                              // PR scalar
      real Cmp_s_eff;                             // Eff scalar

      real Cmp_RlineStall;                        // Stall line definition
      real Cmp_alphaMapDes;                       // Map alpha value
      
      real Cmp_fracBldW;                          // bleed fraction W          
      real Cmp_fracBldP;                          // bleed fraction P
      real Cmp_fracBldWork;                       // bleed fraction Work

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for compressor " << COMP_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string NcMapDesNm = COMP_NAMES[n] + ".S_map.NcMapDes";
      Cmp_NcMapDes = evalExpr(NcMapDesNm);
      string effDesNm = COMP_NAMES[n] + ".effDes";
      Cmp_effDes = evalExpr(effDesNm);
      string PRdesNm = COMP_NAMES[n] + ".PRdes";
      Cmp_PRdes = evalExpr(PRdesNm);
      string RlineDesNm = COMP_NAMES[n] + ".S_map.RlineMapDes";
      Cmp_RlineDes = evalExpr(RlineDesNm);
      
// Gather map scalars defined by NPSS
      string s_NcNm     = COMP_NAMES[n] + ".S_map.s_NcDes";
      Cmp_s_Nc = evalExpr(s_NcNm);
      //Cmp_s_Nc = 1/Cmp_s_Nc;
      string s_WcDesNm  = COMP_NAMES[n] + ".S_map.s_WcDes";
      Cmp_s_Wc = evalExpr(s_WcDesNm);
      string s_effDesNm = COMP_NAMES[n] + ".S_map.s_effDes";
      Cmp_s_eff = evalExpr(s_effDesNm);
      string s_PRdesNm  = COMP_NAMES[n] + ".S_map.s_PRdes";
      Cmp_s_PR = evalExpr(s_PRdesNm);
      
// Gather stall line value
      string RlineStallNm = COMP_NAMES[n] + ".S_map.RlineStall";
      Cmp_RlineStall = evalExpr(RlineStallNm); 

// Gather design alpha value
      string AlphaNm = COMP_NAMES[n] + ".S_map.alphaMapDes";
      Cmp_alphaMapDes = evalExpr(AlphaNm); 

// Save data to files
      DATA <<StrNm<<COMP_NAMES[n]<<".type = '"<<evalExpr(COMP_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".Name = '"<<COMP_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".NcDes_M = "<< Cmp_NcMapDes <<";" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".EffDes_M = "<< Cmp_effDes <<";" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".PRDes_M = "<< Cmp_PRdes <<";" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".RlineDes_M = "<< Cmp_RlineDes <<";" <<endl;

      DATA <<StrNm<<COMP_NAMES[n]<<".s_C_Nc_M = "<< Cmp_s_Nc <<";" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".s_C_Wc_M = "<< Cmp_s_Wc <<";" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".s_C_Eff_M = "<< Cmp_s_eff <<";" <<endl;
      DATA <<StrNm<<COMP_NAMES[n]<<".s_C_PR_M = "<< Cmp_s_PR <<";" <<endl;
      
      DATA <<StrNm<<COMP_NAMES[n]<<".SRline_M = "<< Cmp_RlineStall <<";" <<endl;

      DATA <<StrNm<<COMP_NAMES[n]<<".alphaMapDes = "<< Cmp_alphaMapDes <<";" <<endl<<endl;

// Populate Bleed Outputs
// Search for bleed ports in element
      string BleedPortList[];
      string BleedPortListCmd = COMP_NAMES[n] + ".bleedPortList";
      BleedPortList = evalExpr(BleedPortListCmd);

      DATA <<StrNm<<COMP_NAMES[n]<<".BleedOutNames = {";
// For each bleed port gather the bleed name
      for ( i=0; i < BleedPortList.entries(); ++i ) {
        string fracBldNm = COMP_NAMES[n] + "." + BleedPortList[i];
        DATA<< " '"<<fracBldNm<<"'";
      }
    DATA<<"};"<<endl;


      DATA <<StrNm<<COMP_NAMES[n]<<".C_FBD_M = [";
// For each bleed port compile the fractional W
      for ( i=0; i < BleedPortList.entries(); ++i ) {
        string fracBldWNm = COMP_NAMES[n] + "." + BleedPortList[i] + ".fracBldW";
        Cmp_fracBldW = evalExpr(fracBldWNm);
        DATA<< Cmp_fracBldW<< "  ";
      }
    DATA<<"];"<<endl;

      DATA <<StrNm<<COMP_NAMES[n]<<".C_BFht_M = [";
// For each bleed port compile the fractional values
      for ( i=0; i < BleedPortList.entries(); ++i ) {
        string fracBldWorkNm = COMP_NAMES[n] + "." + BleedPortList[i] + ".fracBldWork";
        Cmp_fracBldWork = evalExpr(fracBldWorkNm);
        DATA<< Cmp_fracBldWork<< "  ";
      }
    DATA<<"];"<<endl;

      DATA <<StrNm<<COMP_NAMES[n]<<".C_BFPt_M = [";
// For each bleed port compile the fractional values
      for ( i=0; i < BleedPortList.entries(); ++i ) {
        string fracBldPNm = COMP_NAMES[n] + "." + BleedPortList[i] + ".fracBldP";
        Cmp_fracBldP = evalExpr(fracBldPNm);
        DATA<< Cmp_fracBldP<< "  ";
      }
    DATA<<"];"<<endl<<endl;

    DATA <<"if  ~isempty("<<StrNm<<COMP_NAMES[n]<<".C_FBD_M)"<<endl;
    DATA <<"    "<<StrNm<<COMP_NAMES[n]<<".FBLDEN_M = 1;"<<endl<<"end"<<endl;


// Generate matlab code to generate T-MATS maps from NPSS maps
// Specifically alpha will be fixed and NcVec, RVec, WcArray, PRArray, and 
// EffArray will be gathered from the matlab generated full NPSS maps.
// This tool will also assume there are only two alpha values possible.
    DATA <<endl;
    DATA << "if exist('mapData" + COMP_NAMES[n] + ".m','file')==2"<<endl;    
    DATA << "    run('mapData" + COMP_NAMES[n] + ".m');"<<endl<<"else"<<endl;
    DATA << "    disp('Error finding file mapData" + COMP_NAMES[n] + ".m');"<<endl<<"end"<<endl;
    DATA <<endl;
    DATA <<"% Generate T-MATS performance map "<<endl;
    DATA <<"% This tool assumes alphaMapDes is exactly an alpha value. "<<endl;
    DATA <<"i4 = 1;" <<endl;
    DATA <<"if exist('"<<COMP_NAMES[n]<<"','var') ~= 1" <<endl;
    DATA <<"    disp('Error finding variable "<<COMP_NAMES[n]<<"');" <<endl << "end" <<endl;
    DATA <<"for i = 1:length("<<COMP_NAMES[n]<<"(1,1,1,:))" <<endl;
    DATA <<"    if TMTS."<<COMP_NAMES[n]<<".alphaMapDes >= "<<COMP_NAMES[n]<<"(1,1,1,i)"<<endl;
    DATA <<"        i4 = i;"<<endl << "    end"<<endl << "end"<<endl;
    DATA << StrNm<<COMP_NAMES[n]<<".Y_C_Map_NcVec_M = "<<COMP_NAMES[n]<<"(1,:,2,i4);"<<endl;
    DATA << StrNm<<COMP_NAMES[n]<<".X_C_RlineVec_M = "<<COMP_NAMES[n]<<"(:,1,3,i4)';"<<endl;
    DATA << StrNm<<COMP_NAMES[n]<<".T_C_Map_WcArray_M = "<<COMP_NAMES[n]<<"(:,:,4,i4)';"<<endl;
    DATA << StrNm<<COMP_NAMES[n]<<".T_C_Map_PRArray_M = "<<COMP_NAMES[n]<<"(:,:,5,i4)';"<<endl;
    DATA << StrNm<<COMP_NAMES[n]<<".T_C_Map_EffArray_M = "<<COMP_NAMES[n]<<"(:,:,6,i4)';"<<endl;
    DATA << endl;

   }

//-------------------------------------------------------------------------
// for each Duct, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < DUCT_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real dPqP;                          // pressure loss

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for duct " << DUCT_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string dPqPNm = DUCT_NAMES[n] + ".dPqP";
      dPqP = evalExpr(dPqPNm);

// Save data to files
      DATA <<StrNm<<DUCT_NAMES[n]<<".type = '"<<evalExpr(DUCT_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<DUCT_NAMES[n]<<".Name = '"<<DUCT_NAMES[n]<<"';" <<endl;
      DATA << "% This file assumes a fractional drop in pressure accross the duct" << endl <<endl;
      DATA <<StrNm<<DUCT_NAMES[n]<<".dP_M = "<< dPqP <<";" <<endl;
      
      DATA << endl;
    }

//-------------------------------------------------------------------------
// for each Gearbox, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < GEARBX_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real GB_eff;                          // gearbox efficiency
      real GB_gearRatio;                    // gearbox efficiency

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for duct " << GEARBX_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string GB_effNm = GEARBX_NAMES[n] + ".eff";
      GB_eff = evalExpr(GB_effNm);
      string GB_gearRatioNm = GEARBX_NAMES[n] + ".gearRatio";
      GB_gearRatio = evalExpr(GB_gearRatioNm);

// Save data to files
      DATA <<StrNm<<GEARBX_NAMES[n]<<".type = '"<<evalExpr(GEARBX_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<GEARBX_NAMES[n]<<".Name = '"<<GEARBX_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<GEARBX_NAMES[n]<<".Eff_M = "<< GB_eff <<";" <<endl;
      DATA <<StrNm<<GEARBX_NAMES[n]<<".R_M = "<< GB_gearRatio <<";" <<endl;      

      DATA << endl;
    }


//-------------------------------------------------------------------------
// for each Inlet, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < INLET_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real eRamBase;                          // Base eRam

      DATA << "%--------------------------------------------------------" << endl;
      DATA << "%% Setup File for inlet " << INLET_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string eRamBaseNm = INLET_NAMES[n] + ".eRamBase";
      eRamBase = evalExpr(eRamBaseNm);

// Save data to files
      DATA << "% This file assumes a fractional drop in pressure accross the inlet" << endl <<endl;
      DATA <<StrNm<<INLET_NAMES[n]<<".type = '"<<evalExpr(INLET_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<INLET_NAMES[n]<<".Name = '"<<INLET_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<INLET_NAMES[n]<<".eRambase_M = "<< eRamBase <<";" <<endl;
      DATA << endl;
    } 

//-------------------------------------------------------------------------
// for Inlet Start, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < INSTRT_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real InStrt_Wdes;                          // design mass flow

      DATA << "%--------------------------------------------------------" << endl;
      DATA << "%% Setup File for inlet " << INSTRT_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string InStrt_WdesNm = INSTRT_NAMES[n] + ".Wdes";
      InStrt_Wdes = evalExpr(InStrt_WdesNm);

// Save data to files
      DATA <<StrNm<<INSTRT_NAMES[n]<<".type = '"<<evalExpr(INSTRT_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<INSTRT_NAMES[n]<<".Name = '"<<INSTRT_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<INSTRT_NAMES[n]<<".Wdes = "<< InStrt_Wdes <<";" <<endl;
      DATA << endl;
    }

//-------------------------------------------------------------------------
// for Mixer, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < MIXER_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Mix_Aphyp;                        // area of primary input
      real Mix_Aphys;                        // area of secondary input
      real Mix_s_M_Impp;                     // primary flow impulse scalar
      real Mix_s_M_Imps;                     // secondary flow impulse scalar
      real Mix_s_M_Vp;                       // primary flow velocity scalar
      real Mix_s_M_Vs;                       // secondary flow velocity scalar
      real Mix_SWPS;                         // primary flow switch, assume primary flow is second T-MATS input (first NPSS input).
      real Mix_BPRDes;                       // branch pressure ratio design, Ws / Wp
      real Mix_MNDes;                        // mach number design of primary flow

      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for mixer " << MIXER_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Mix_ApNm = MIXER_NAMES[n] + ".Fl_I1.Aphy";
      Mix_Aphyp = evalExpr(Mix_ApNm);
      string Mix_AsNm = MIXER_NAMES[n] + ".Fl_I2.Aphy";
      Mix_Aphys = evalExpr(Mix_AsNm);
      string Mix_sImppNm = MIXER_NAMES[n] + ".s_Impulse1";
      Mix_s_M_Impp = evalExpr(Mix_sImppNm);
      string Mix_sImpsNm = MIXER_NAMES[n] + ".s_Impulse2";
      Mix_s_M_Imps = evalExpr(Mix_sImpsNm);
      string Mix_sVpNm = MIXER_NAMES[n] + ".s_V1";
      Mix_s_M_Vp = evalExpr(Mix_sVpNm);
      string Mix_sVsNm = MIXER_NAMES[n] + ".s_V2";
      Mix_s_M_Vs = evalExpr(Mix_sVsNm);  
      Mix_SWPS = 1; //(primary flow is T-MATS input 2, typically NPSS input 1)
      string Mix_WpNm = MIXER_NAMES[n] + ".Fl_I1.W";
      string Mix_WsNm = MIXER_NAMES[n] + ".Fl_I2.W";
      Mix_BPRDes = evalExpr(Mix_WsNm+"/"+Mix_WpNm);
      string Mix_MNDesNm = MIXER_NAMES[n] + ".Fl_I1.MN";
      Mix_MNDes = evalExpr(Mix_MNDesNm);

// Save data to files
      DATA <<StrNm<<MIXER_NAMES[n]<<".type = '"<<evalExpr(MIXER_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".Name = '"<<MIXER_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".Aphy2_M = "<< Mix_Aphyp <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".Aphy1_M = "<< Mix_Aphys <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".s_M_Imp2_M = "<< Mix_s_M_Impp <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".s_M_Imp1_M = "<< Mix_s_M_Imps <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".s_M_V2_M = "<< Mix_s_M_Vp <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".s_M_V1_M = "<< Mix_s_M_Vs <<";" <<endl;
      // Design variables
      DATA <<StrNm<<MIXER_NAMES[n]<<".SWPS_M = "<< Mix_SWPS <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".BPRDes_M = "<< Mix_BPRDes <<";" <<endl;
      DATA <<StrNm<<MIXER_NAMES[n]<<".MNDes_M = "<< Mix_MNDes <<";" <<endl;
      DATA << endl;
    }



//-------------------------------------------------------------------------
// for each Nozzle, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < NOZ_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Noz_Ath;                         // Throat area
      real Noz_Aexit;                       // Exit area
      real Noz_Cfg;                         // coefficient of thrust
      real Noz_Cv;                          // coefficient of velocity
      real Noz_Cqua;                        // Thermal growth constant 
      real Noz_CdTh;                        // Throat discharge coefficient
      string Noz_swtype;                    // Type of nozzle geometry
      string Noz_swcoef;                    // determines how corrections to ideal thrust are applied
      real Noz_Wdes;                        // Design flow
      string Noz_PsExh;                         // Name of source of exit static pressure

      DATA << "%--------------------------------------------------------" << endl;
      DATA << "%% Setup File for nozzle " << NOZ_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Noz_AthNm = NOZ_NAMES[n] + ".AthCold";
      Noz_Ath = evalExpr(Noz_AthNm);
      string Noz_AexitNm = NOZ_NAMES[n] + ".Aexit";
      Noz_Aexit = evalExpr(Noz_AexitNm);
      string Noz_CfgNm = NOZ_NAMES[n] + ".Cfg";
      Noz_Cfg = evalExpr(Noz_CfgNm);
      string Noz_CvNm = NOZ_NAMES[n] + ".Cv";
      Noz_Cv = evalExpr(Noz_CvNm);
      string Noz_CquaNm = NOZ_NAMES[n] + ".Cqua";
      Noz_Cqua = evalExpr(Noz_CquaNm);
      string Noz_CdThNm = NOZ_NAMES[n] + ".CdTh";
      Noz_CdTh = evalExpr(Noz_CdThNm);
      string Noz_swtypeNm = NOZ_NAMES[n] + ".switchType";
      Noz_swtype = evalExpr(Noz_swtypeNm);
      string Noz_swcoefNm = NOZ_NAMES[n] + ".switchCoef";
      Noz_swcoef = evalExpr(Noz_swcoefNm);
      string Noz_WdesNm = NOZ_NAMES[n] + ".Fl_O.W";
      Noz_Wdes = evalExpr(Noz_WdesNm);
      string Noz_PsExhNm = NOZ_NAMES[n] + ".PsExhName";
      Noz_PsExh = evalExpr(Noz_PsExhNm);

// Save data to files
      DATA <<StrNm<<NOZ_NAMES[n]<<".type = 'Nozzle';" <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".Name = '"<<NOZ_NAMES[n]<<"';" <<endl;
      DATA << "% Define Areas" << endl <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".N_TArea_M = "<< Noz_Ath <<";" <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".N_EArea_M = "<< Noz_Aexit <<";" <<endl<<endl;
      DATA << "% Assume coefficients are constant" << endl;
      DATA <<StrNm<<NOZ_NAMES[n]<< ".X_N_PEQPaVec_M = [0 10000];"<< endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".T_N_CfgArray_M = "<< Noz_Cfg <<"*ones(1,2);" <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".T_N_CvArray_M = "<< Noz_Cv <<"*ones(1,2);" <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".T_N_CdThArray_M = "<< Noz_CdTh <<"*ones(1,2);" <<endl <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<< ".X_N_TtVec2_M = [0 10000];"<< endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".T_N_TGArray_M = "<< Noz_Cqua <<"*ones(1,2);" <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".WDes_M = "<< Noz_Wdes <<";" <<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".Noz_PsExhName = '"<< Noz_PsExh <<"';" <<endl <<endl;

      DATA << "% swtype -> determines nozzle geometry, CON_DIV, CONIC, or FIXED" << endl;
      DATA << "% set TMATS Nozzle parameter SWType_M to Convergent Divergent if swtype == CON_DIV" << endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".swtype = '"<< Noz_swtype <<"';" <<endl<<endl;
      DATA <<"if strcmp("<<StrNm<<NOZ_NAMES[n]<<".swtype, 'CON_DIV')"<<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".SWType_M = 'Convergent-Divergent';"<<endl;
      DATA <<"else"<<endl <<StrNm<<NOZ_NAMES[n]<<".SWType_M = 'Convergent';"<<endl<<"end"<<endl;

      DATA << "% swcoeff -> determines how corrections will be used to adjust ideal thrust" << endl;
      DATA << "% enable TMATS Nozzle parameter CfgEn_M if swcoeff == CFG" << endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".swcoef = '"<< Noz_swcoef <<"';" <<endl;
      DATA <<"if strcmp("<<StrNm<<NOZ_NAMES[n]<<".swtype, 'CFG')"<<endl;
      DATA <<StrNm<<NOZ_NAMES[n]<<".CfgEn_M = 1;"<<endl;
      DATA <<"else"<<endl<<StrNm<<NOZ_NAMES[n]<<".CfgEn_M = 0;"<<endl<<"end"<<endl;

      DATA << endl;
    }

//-------------------------------------------------------------------------
// for each Shaft, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < SHAFT_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Shft_inertia;                          // shaft inertia
      real Shft_HPX;                              // shaft power debit
      real Shft_fracLoss;                         // shaft fractional loss
      real Shft_Nmech;                            // shaft speed

      DATA << "%--------------------------------------------------------" << endl;
      DATA << "%% Setup data for shaft " << SHAFT_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Shft_inertiaNm = SHAFT_NAMES[n] + ".inertia";
      Shft_inertia = evalExpr(Shft_inertiaNm);
      string HPXNm = SHAFT_NAMES[n] + ".HPX";
      Shft_HPX = evalExpr(HPXNm);
      string fracLossNm = SHAFT_NAMES[n] + ".fracLoss";
      Shft_fracLoss = evalExpr(fracLossNm);
      string Shft_NmechNm = SHAFT_NAMES[n] + ".Nmech";
      Shft_Nmech = evalExpr(Shft_NmechNm);

// Save data to files
      DATA <<StrNm<<SHAFT_NAMES[n]<<".type = '"<<evalExpr(SHAFT_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<SHAFT_NAMES[n]<<".Name = '"<<SHAFT_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<SHAFT_NAMES[n]<<".Shaft_Inertia_M = "<< Shft_inertia <<";" <<endl;
      DATA <<StrNm<<SHAFT_NAMES[n]<<".HPX = "<< Shft_HPX <<";" <<endl;
      DATA <<StrNm<<SHAFT_NAMES[n]<<".fracLoss = "<< Shft_fracLoss <<";" <<endl;
      DATA <<StrNm<<SHAFT_NAMES[n]<<".Nmech = "<< Shft_Nmech <<";" <<endl;
      
      DATA << endl;
    }

//-------------------------------------------------------------------------
// for each Splitter, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < SPLIT_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      
      real Split_BPR;                          // branch flow ratio


      DATA << "%--------------------------------------------------------"  << endl;
      DATA << "%% Setup data for ambient " << SPLIT_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string Split_BPRNm = SPLIT_NAMES[n] + ".BPR";
      Split_BPR = evalExpr(Split_BPRNm);

// Save data to files
      DATA <<StrNm<<SPLIT_NAMES[n]<<".type = '"<<evalExpr(SPLIT_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<SPLIT_NAMES[n]<<".Name = '"<<SPLIT_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<SPLIT_NAMES[n]<<".BPR = "<< Split_BPR <<";" <<endl;

      DATA << endl;
    }


//-------------------------------------------------------------------------
// for each Turbine, write its design data to a file in MATLAB format
//-------------------------------------------------------------------------
   for ( n=0; n < TURB_NAMES.entries(); ++n ) {

      //----------------------------------------------------------------------
      // Declare parameters to be used for T-MATS
      //----------------------------------------------------------------------
      real T_NpMapDes;                          // Map design corrected shaft speed (NpMap)
      real T_effDes;                            // Design eff
      real T_PRmapDes;                          // Map design PR
      real T_Ndes;                              // Design shaft speed (N)
      real T_PRdes;                             // design PR

      real T_s_Np;                              // Np scalar
      real T_s_Wp;                              // Wp scalar
      real T_s_PR;                              // PR scalar
      real T_s_eff;                             // Eff scalar

      real T_alphaMapDes;                       // Map alpha value      
      real T_Pfract;                            // fractional bleed position

      DATA << "%--------------------------------------------------------" << endl;
      DATA << "%% Setup data for turbine " << TURB_NAMES[n] << endl;
      DATA << "%--------------------------------------------------------" << endl;

// Gather design values
      string T_NpMapDesNm = TURB_NAMES[n] + ".S_map.NpMapDes";
      T_NpMapDes = evalExpr(T_NpMapDesNm);
      string T_effDesNm = TURB_NAMES[n] + ".effDes";
      T_effDes = evalExpr(T_effDesNm);
      string T_PRmapDesNm = TURB_NAMES[n] + ".S_map.PRmapDes";
      T_PRmapDes = evalExpr(T_PRmapDesNm);
      string T_NdesNm = TURB_NAMES[n] + ".Ndes";
      T_Ndes = evalExpr(T_NdesNm);
      string T_PRdesNm = TURB_NAMES[n] + ".PRdes";
      T_PRdes = evalExpr(T_PRdesNm);
      string T_AMDNm = TURB_NAMES[n] + ".S_map.alphaMapDes";
      T_alphaMapDes = evalExpr(T_AMDNm);

// Gather map scalars defined by NPSS
      string T_s_NpNm     = TURB_NAMES[n] + ".S_map.s_NpDes";
      T_s_Np = evalExpr(T_s_NpNm);
      string T_s_WpDesNm  = TURB_NAMES[n] + ".S_map.s_WpDes";
      T_s_Wp = evalExpr(T_s_WpDesNm);
      string T_s_effDesNm = TURB_NAMES[n] + ".S_map.s_effDes";
      T_s_eff = evalExpr(T_s_effDesNm);
      string T_s_PRdesNm  = TURB_NAMES[n] + ".S_map.s_PRdes";
      T_s_PR = evalExpr(T_s_PRdesNm);

// Save data to files
      DATA <<StrNm<<TURB_NAMES[n]<<".type = '"<<evalExpr(TURB_NAMES[n] +".isA()")<<"';" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".Name = '"<<TURB_NAMES[n]<<"';" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".NcDes_M = "<< T_NpMapDes <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".EffDes_M = "<< T_effDes <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".PRDes_M = "<< T_PRmapDes <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".NDes_M = "<< T_Ndes <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".PRdes = "<< T_PRdes <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".alphaMapDes = "<< T_alphaMapDes <<";" <<endl<<endl;

      DATA <<StrNm<<TURB_NAMES[n]<<".s_T_Nc_M = "<< T_s_Np <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".s_T_Wc_M  = "<< T_s_Wp <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".s_T_Eff_M = "<< T_s_eff <<";" <<endl;
      DATA <<StrNm<<TURB_NAMES[n]<<".s_T_PR_M = "<< T_s_PR <<";" <<endl;
      
      DATA << endl;

// Populate Bleed Inputs
// Search for bleed ports in element
      string BleedPortListT[];
      string BleedPortListTCmd = TURB_NAMES[n] + ".bleedPortList";
      BleedPortListT = evalExpr(BleedPortListTCmd);
      DATA<<"% Note: T-MATS does not support Interstage Turbine bleeds. All fractional"<<endl<<"% Bleed fraction will be rounded to either 1 (exit) or 0 (inlet) for T-MATS.";
      DATA<<endl<<"% Additionally, it should also be noted that in NPSS 1 is for inlet and 0 is for exit, opposite that of T-MATS."<<endl;

      DATA <<StrNm<<TURB_NAMES[n]<<".BleedInNames = {";
// For each bleed port gather the bleed name
      for ( i=0; i < BleedPortListT.entries(); ++i ) {
        string BldNm = TURB_NAMES[n] + "." + BleedPortListT[i];
        DATA<< " '"<<BldNm<<"'";
      }
    DATA<<"};"<<endl;

      DATA <<StrNm<<TURB_NAMES[n]<<".T_CoolFlwPos_M = [";
// For each bleed port compile Position
      for ( i=0; i < BleedPortListT.entries(); ++i ) {
        string PfractNm = TURB_NAMES[n] + "." + BleedPortListT[i] + ".Pfract";
        T_Pfract = evalExpr(PfractNm); 
        if (T_Pfract > 0.5) {
            DATA<<"0 ";}
        else {
            DATA<<"1 ";}
      }
    DATA<<"];"<<endl<<endl;
    
    DATA <<"if ~isempty("<<StrNm<<TURB_NAMES[n]<<".T_CoolFlwPos_M)"<<endl;
    DATA <<"    "<<StrNm<<TURB_NAMES[n]<<".CFlwEn_M = 1;"<<endl<<"end"<<endl;

// Generate matlab code to generate T-MATS maps from NPSS maps
// Specifically alpha will be fixed and NpVec, PRVec, WpArray, and 
// EffArray will be gathered from the matlab generated full NPSS maps.
// This tool will also assume there is only one alpha value.
    DATA <<endl;
    DATA << "if exist('mapData" + TURB_NAMES[n] + ".m','file')==2"<<endl;    
    DATA << "    run('mapData" + TURB_NAMES[n] + ".m');"<<endl<<"else"<<endl;
    DATA << "    disp('Error finding file mapData" + TURB_NAMES[n] + ".m');"<<endl<<"end"<<endl;
    DATA <<endl;
    DATA <<"% Generate T-MATS performance map, assume alpha == alpha design "<<endl;
    DATA <<"i4 = 1;"<<endl;
    DATA << "if exist('"<<TURB_NAMES[n]<<"','var') ~= 1" <<endl;
    DATA << "    disp('Error finding variable " + TURB_NAMES[n] + "');"<<endl<<"end"<<endl<<endl;
    DATA <<"for i = 1:length("<<TURB_NAMES[n]<<"(1,1,1,:))" <<endl;
    DATA <<"    if TMTS."<<TURB_NAMES[n]<<".alphaMapDes >= "<<TURB_NAMES[n]<<"(1,1,1,i)"<<endl;
    DATA <<"        i4 = i;"<<endl << "    end"<<endl << "end"<<endl;
    DATA << StrNm<<TURB_NAMES[n]<<".Y_T_Map_NcVec_M = "<<TURB_NAMES[n]<<"(1,:,2,i4);"<<endl;
    DATA << StrNm<<TURB_NAMES[n]<<".X_T_Map_PRVec_M = "<<TURB_NAMES[n]<<"(:,1,3,i4)';"<<endl;
    DATA << StrNm<<TURB_NAMES[n]<<".T_T_Map_WcArray_M = "<<TURB_NAMES[n]<<"(:,:,4,i4)';"<<endl;
    DATA << StrNm<<TURB_NAMES[n]<<".T_T_Map_EffArray_M = "<<TURB_NAMES[n]<<"(:,:,5,i4)';"<<endl;
    DATA << endl;
    }
//-------------------------------------------------------------------------
// Write ELMENT and LINK information to a file in MATLAB format
//-------------------------------------------------------------------------
   DATA << "%--------------------------------------------------------" << endl;
   DATA << "%% Generate ELEMENT and Type Vectors" << endl;
   DATA << "%--------------------------------------------------------" << endl;  
   
// Save ELEMENT data to files
   DATA <<StrNm<<"INFO.ELEMENT = {..."<<endl;
   Iter = 1;
   for ( n=0; n < ELEMENTS.entries(); ++n ) {
      DATA <<"'" <<ELEMENTS[n]<<"' ";
      if (Iter == 5) {
          DATA<<"..."<<endl;
          Iter = 0;
      }
      Iter = Iter +1;
    }
    DATA<<"};"<<endl <<endl;

// Save ELMENT Type data to files
    DATA <<StrNm<<"INFO.ELEMENT(2,:) = {..."<<endl;
       Iter = 1;
   for ( n=0; n < ELEMENTS.entries(); ++n ) {
      DATA <<"'" <<evalExpr(ELEMENTS[n] +".isA()")<<"' ";
      if (Iter == 5) {
          DATA<<"..."<<endl;
          Iter = 0;
      }
      Iter = Iter +1;
    }
    DATA<<"};"<<endl <<endl;

   DATA << "%--------------------------------------------------------" << endl;
   DATA << "%% Generate Station array" << endl;
   DATA << "%--------------------------------------------------------" << endl;  


    DATA <<"% Station Inputs"<<endl;
    DATA <<StrNm<<"INFO.STATIONS(1,:) = {..."<<endl;
       Iter = 1;
   for ( n=0; n < STATIONS.entries(); ++n ) {
      DATA <<"'" << evalExpr(STATIONS[n]+".getP1Name()")<<"' ";
      if (Iter == 5) {
          DATA<<"..."<<endl;
          Iter = 0;
      }
      Iter = Iter +1;
    }
    DATA<<"};"<<endl;

// Save Flow STATION data to files
    DATA <<"% Station Names"<<endl;
    DATA <<StrNm<<"INFO.STATIONS(2,:) = {..."<<endl;
       Iter = 1;
   for ( n=0; n < STATIONS.entries(); ++n ) {
      DATA <<"'" <<STATIONS[n]<<"' ";
      if (Iter == 5) {
          DATA<<"..."<<endl;
          Iter = 0;
      }
      Iter = Iter +1;
    }
    DATA<<"};"<<endl;

// Save Flow STATION outputs data to files
    DATA <<"% Station Outputs"<<endl;
    DATA <<StrNm<<"INFO.STATIONS(3,:) = {..."<<endl;
       Iter = 1;
   for ( n=0; n < STATIONS.entries(); ++n ) {
      DATA <<"'" << evalExpr(STATIONS[n]+".getP2Name()")<<"' ";
      if (Iter == 5) {
          DATA<<"..."<<endl;
          Iter = 0;
      }
      Iter = Iter +1;
    }
    DATA<<"};"<<endl;

   DATA << "%--------------------------------------------------------" << endl;
   DATA << "%% Generate Shaft Connection array" << endl;
   DATA << "%--------------------------------------------------------" << endl;  


// Save Shaft connection data
    DATA <<"% Shaft Linking"<<endl;
  
   for ( n=0; n < SHAFT_NAMES.entries(); ++n ) {
        DATA <<StrNm<<"INFO.SHAFT{"<<n+1<<"} = {";
        DATA <<"'"<<SHAFT_NAMES[n]<<"'";
        Shaft_Links = evalExpr(SHAFT_NAMES[n]+".shaftPortList");
        for (i=0; i<Shaft_Links.entries(); ++i){
            DATA<<",'"<<SHAFT_NAMES[n]<<"."<<Shaft_Links[i]<<"'";
        }
    DATA<<"};"<<endl;
    }
    
   DATA << "%--------------------------------------------------------" << endl;
   DATA << "%% Generate Bleed array" << endl;
   DATA << "%--------------------------------------------------------" << endl; 

// Save Bleed Out connection data
   DATA <<"% Bleed Linking"<<endl;
   DATA <<StrNm<<"INFO.BLEED_OUT = {";
   for ( n=0; n < BLEED_OUT.entries(); ++n ) {
        DATA <<" '"<<BLEED_OUT[n]<<"'";
    }
    DATA<<"};"<<endl;


   DATA <<StrNm<<"INFO.BLEED_IN = {";
   for ( n=0; n < BLEED_IN.entries(); ++n ) {
        DATA <<" '"<<BLEED_IN[n]<<"'";
    }
    DATA<<"};"<<endl;


DATA<<endl;

string OStrNm = StrNm.trim(".");
DATA<<"assignin('base', '"<<WorkSpaceNm<<"',"<< OStrNm <<");"<<endl;
} 
 