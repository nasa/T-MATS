AGTF30, located at nasa/AGTF30 on github

The Advanced Geared Turbofan 30,000 (AGTF30) is a geared turbofan simulation that utilizes the 
Toolbox for the Modeling and Analysis of Thermodynamic Systems (T-MATS) to create a steady-state 
and dynamic engine model within MATLAB/Simulink. The engine model is based upon a futuristic 
geared turbofan concept and allows steady-state operation throughout the flight envelope. 
Dynamic operation is currently limited to a manual inputs, however the simulation architecture 
contains the framework for adding a full engine controller by the user. Design tools included within 
the package include system linearization and automated plotting scripts.

AGTF30 installation:

The AGTF30 may be downloaded at https://github.com/nasa/AGTF30

Install Matlab and Simulink, Developed in MATLAB 2015aSP1
Install the Toolbox for the Modeling and Analyais of Thermodyanmic Systems, developed with 
T-MATS version 1.2 (https://github.com/nasa/T-MATS/releases)

Running AGTF30 simulation:

Navigate to the AGTF30 folder
Run script "setup_everything.m", this will generate all required variables within MATLAB (namely: bus variables and the engine parameter structure 'MWS').
Open AGTF30 system simualtion: dynamic(Dyn), Linearization (Lin), or steady-state (SS).
Run the specified model.
Changing run conditions:

Open setup_everyting and adjust Input. structure as appropriate

or

Verify setup_everything sets Input.UseExcel = 1, update excel document define_inputs.xlsx

or

Verify setup_everything sets Input.UseExcel = 0, update define_inputs.m

System inputs are defined (within the MWS structure) as vectors with each value matching with a time vector. The linearization and steady-state systems will only make use of the initial values, Note: Linearization and Dynamic systems will use the steady-state solver to generate initial conditions.

Outputs:

Steady state simulation outputs structure: out_SS, containing a large amount of data. Additionally it generate specific structures out_*
Linearization simulation outputs structure: out_Lin, containing a large amount of data. (ABCD matricies for state-space model are located within the structure)
Dynamic simulation outputs structure: out_Dyn, containing a large amount of data. Additionally it generate specific structures out_*
Auto plotting scripts:

The AGTF30 makes use of T-MATS autoplotting scrips. These are defined within PlotDyn.m and PlotSS.m. After running the Dyn model or SS model, run the appropriate script to generate the appropriate plots.

References:

Jeffryes W. Chapman and Jonathan S. Litt. "Control Design for an Advanced Geared Turbofan Engine", 53rd AIAA/SAE/ASEE Joint Propulsion Conference, AIAA Propulsion and Energy Forum, (AIAA 2017-4820)
Jones, S.M., Haller, W.J., Tong, M.T., “An N+3 Technology Level Reference Propulsion System”, NASA/TM-2017-219501, 2017.