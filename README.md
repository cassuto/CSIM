[![Coverage Status](https://coveralls.io/repos/github/cassuto/CSIM/badge.svg)](https://coveralls.io/github/cassuto/CSIM)

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

### Build
Build unit tests with code coverage:
* mkdir build && cd build
* cmake .. -Denable_testcases=ON -Denable_coverage=ON -DCMAKE_BUILD_TYPE=Debug
* make -j

### Run
Running tests with code coverage:
* make coverage

The result will be reported in build/coverage.html

Running tests without coverage:
* make check

or
* export LD_LIBRARY_PATH=\`pwd\`
* ./tstall

### API document
Generating documents:

Before that, Doxygen needs to be installed. If not, rerun cmake after the installation.

* make docs
