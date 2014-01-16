T-MATS
==========

Toolbox for the Modeling and Analysis of Thermodynamic Systems

==========

The Toolbox for the Modeling and Analysis of Thermodynamic Systems (T-MATS) 
is a Simulink toolbox intended for use in the modeling and simulation of thermodynamic 
systems and their controls. T-MATS contains generic thermodynamic and controls 
components that may be combined with a variable input iterative solver and optimization 
algorithm to create complex systems to meet the needs of a developer. Developement of this tool
was completed on behalf of the NASA Aviation Safety Program's Vehicle Systems Safety Technologies
(VSST) located at the Glenn Research Center in Cleveland, OH.

The goal of the T MATS software is to provide a toolbox for the development of thermodynamic 
system models, which contains a simulation framework, multi-loop solver techniques, and modular 
thermodynamic simulation blocks. While much of the capability in T MATS is in transient 
thermodynamic simulation, the developer’s main interests are in aero-thermal applications; 
as such, one highlight of the T-MATS software package is the turbomachinery block set. This 
set of Simulink blocks gives a developer the tools required to create virtually any steady 
state or dynamic turbomachinery simulation, e.g., a gas turbine simulation. In systems where 
the control or other related systems are modeled in MATLAB/Simulink, the T-MATS developer has 
the ability to create the complete system in a single tool.

T-MATS is written in MATLAB/Simulink (The Mathworks, Inc.), is open source; 
and is intended for use by industry, government, and academia. All T-MATS equations 
were developed from public sources and all default maps and constants provided in the 
T-MATS software package are nonproprietary and available to the public. The software 
is released under the Apache V2.0 license agreement. 

T-MATS is contained in the following folder structure:

	Releases - Contains stable T-MATS releases. Generally a software version from 
	this folder should be used when installing T-MATS.
	
	Trunk - Contains T-MATS developmenet software.  This version should be used 
	by T-MATS developers.
	
	Resources - Contains T-MATS reference material and is not required for 
	operation.

