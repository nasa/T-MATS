//
//------------------------------------------------------------------------
//                                                                       |
//   File Name:     JT9D.fnc                                             |
//   Date(s):       March 31, 2011                                       |
//   Author:        Scott M. Jones                                       |
//                                                                       |
//   Description:   This file declares the model solver variables and    |
//                  sets up functions for activating them for different  |
//                  engine operating conditions                          |
//                                                                       |
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//                    User-Defined Tables and Functions
//------------------------------------------------------------------------


//------------------------------------------------------------------------
//              Independents, Dependents, and Constraints
//------------------------------------------------------------------------

Independent Burner_FAR { varName = "Burner.FAR"; }
Independent Bypass_Ratio { varName = "Splitter.BPRdes"; }

Dependent Velocity_Ratio {
   eq_lhs = "PERF.VjetRatio";
   eq_rhs = "1.300"; }

Dependent Target_T4 {
   eq_lhs = "HPT.Fl_I.Tt";
   eq_rhs = "3000."; }

Dependent Target_Fnet {
   eq_lhs = "PERF.Fn";
   eq_rhs = "TargetThrust"; }

Dependent Max_LP_RPM { 
   eq_lhs = "LP_Shaft.Nmech";
   eq_rhs = "4000.0"; }

Dependent Max_NcFan { 
   eq_lhs = "Fan.S_map.NcMap";
   eq_rhs = "1.10"; }

Dependent Max_T4 {
   eq_lhs = "HPT.Fl_I.Tt";
   eq_rhs = "3260.0"; }


Max_NcFan.addConstraint("Max_LP_RPM", "MAX", 1, 1);
Max_NcFan.addConstraint("Max_T4", "MAX", 1, 1);


//------------------------------------------------------------------------
//               Define full power and part power operation
//------------------------------------------------------------------------
// maximum power: (varies fuel flow)
//    run to maximum fan corrected speed constrained by
//      - T4 limit
//      - LP physical RPM
//
// partial power: (varies fuel flow)
//    run to target input percentage of maximum power


real MaxThrust, PercentThrust, TargetThrust; 


void RunMaxPower() { 
   autoSolverSetup(); 
   solver.addIndependent( "Burner_FAR" );
   solver.addDependent( "Max_NcFan" );

   run(); 
   MaxThrust = PERF.Fn; 
   PercentThrust = PERF.Fn / MaxThrust * 100.;
}


void RunPartPower() { 
   autoSolverSetup(); 
   solver.addIndependent( "Burner_FAR" );
   solver.addDependent( "Target_Fnet" );

   TargetThrust = MaxThrust * ( PERF.PowerCode/100. ); 
   run();
   PercentThrust = PERF.Fn / MaxThrust * 100.;
} 


void RunThrottleHook( real Mach, real altitude ) { 
   Ambient.MN_in = Mach;
   Ambient.alt_in = altitude;

   cout << "Mach  " << Mach << "    altitude " << altitude << "\n";

   PERF.PowerCode = 100.; RunMaxPower();  page.display(); ++CASE;
   PERF.PowerCode =  90.; RunPartPower(); page.display(); ++CASE;
   PERF.PowerCode =  80.; RunPartPower(); page.display(); ++CASE;
   PERF.PowerCode =  60.; RunPartPower(); page.display(); ++CASE;
   PERF.PowerCode =  40.; RunPartPower(); page.display(); ++CASE;
   PERF.PowerCode =  30.; RunPartPower(); 
   PERF.PowerCode =  20.; RunPartPower(); page.display(); ++CASE;

   if ( Ambient.alt > 14000. ) { 
      PERF.PowerCode = 5.; RunPartPower(); page.display(); ++CASE;
   } 

   // run back to maximum power for convergence purposes
   PERF.PowerCode =  60.; RunPartPower();
   PERF.PowerCode =  80.; RunPartPower();
   PERF.PowerCode = 100.; RunMaxPower();
} 

