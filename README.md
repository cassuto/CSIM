# CSIM

This is a SPICE-like analog circuit simulator, for DC/AC Sweep and Transient (time-domain) analysis.

It mainly supports the following device models:
* BSIM3v3.2 (MOSFET Model)
* CCCS (Current Controlled Current Source)
* CCVS (Current Controlled Voltage Source)
* VCCS (Voltage Controlled Current Source)
* VCVS (Voltage Controlled Voltage Source)
* DC and AC voltage and current sources
* Resistor
* Capacitor
* Inductor
* PN Junction
* and more (by the dynamic link library loader)

And the following sweep methods:
* Linear
* Decade
* Octave
* Logarithm

What I have done:
* MNA (Modified Nodal Analysis) with Newton-Raphson iteration
* GEAR numerical integration
* LU decomposition
* HSPICE-like model parser
* Some unit tests

TODO:
* CLI/GUI Frontend
* Improve the sparse matrix algorithm
* Parallel computing acceleration (with GPU)
* Mixed digital domain (with Verilator, IcarusVerilog, etc.)

## Guides

Build:
* mkdir build && cd build
* cmake .. -Denable_testcases=ON -DCMAKE_BUILD_TYPE=Debug    (or Release)
* make -j8

Build documents:
* Doxygen needs to be installed. If not, rerun cmake after the installation
* make docs

Run regression tests:
* ./tstall
