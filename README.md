*RelIoT* : A Reliability Simulation Framework for ns-3
----------------------
*RelIoT* is a simulation framework implemented in ns-3 which allows practical and large-scale reliability evaluation of IoT networks. To allow reliability simulation in ns-3, *RelIoT* introduces
power, performance, temperature, and reliability modules.

Contributors
----------------------
For a list of contributors see [AUTHORS](https://github.com/UCSD-SEELab/RelIoT/blob/master/AUTHORS). 

Extended documentation
----------------------

Extended documentation available at

    /documentation/ReliabilityFramework_Documentation.pdf
 
Supported environment
---------------------
The supported platform for ns-3 is listed in its official [Installation Guide of ns-3](https://www.nsnam.org/wiki/Installation). *RelIoT* is developed on Ubuntu 18.04 LTS.

Building *RelIoT*
----------------
Clone the repository from github:

    git clone https://github.com/UCSD-SEELab/RelIoT.git

Enter the /reliabilitysim/ns-allinone-3.27 directory, and build the simulator:

    cd RelIoT/ns-allinone-3.27
    ./build.py

Usage
-----
Once the project has been built you will interact directly with *Waf* and we do it in the ns-3.27 directory and not in the ns-3-allinone directory. You can run the scripts you've placed in the ns-3.27/scratch folder by using ./waf command. For example:

    cd ns-3.27 
    ./waf --run  "reliability_example"

Running the above command from the /ns-3.27 directory will execute the "reliability_example.cc" script.
