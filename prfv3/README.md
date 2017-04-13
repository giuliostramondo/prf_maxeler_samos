Polymorphic Register File (PRF) MaxJ implementation
=============================

This repository contains the MaxJ implementation of the PRF and a testbench that allow the user to check the correctness of the design. For a more complete explanation of the implementation and funtionalities please refer to the technical report present in this repository. (PRF_MaxJ_DSE_TechRepo.pdf)
The testbench is integrated in the Maxeler project and can be found in in the CPUCode folder. 
The PRF maxj files are located in prf_maxeler_samos/prfv3/EngineCode/src/exampleproject.

How the testbench works
-----------------------

The PRF is - in this design - linked to the host through PCI-express. The testbench first fills all the capacity of the PRF doing parallel writes with increasing numbers. After the PRF is filled with those numbers it starts to perform all the possible combination of reads, with all the shapes available - defined by the mapping scheme that was set in the PRF configuration file. The number read from the PRF are sent to the host which, simulating the PRF, checks their correctness. 

Usage
-----

* Configuration of the PRF
Our PRF implementation allow the user to customize the capacity, number of lanes, mapping scheme and frequency at wich the design is synthesized. All those paramethers can be set in the PRF configuration file PRFConfiguration.maxj located in prf_maxeler_samos/prfv3/EngineCode/src/exampleproject

* Compilation
To compile the whole project - PRF bitstream and testbench - go in the CPUCode folder and run:

```Shell
make RUNRULE=DFE
```

This process could take more than one hour if the bitstream has not been synthesized yet.

* Running the testbench
Once the design is compiled the testbench binary will be located in /RunRule/DFE/binaries and called ExampleProject. The testbench needs to run with a set of parameters that describe how the PRF was configured. It is possible to do this when running the testbench setting the following parameters:
```
    -N <num>     Change the horizontal size of the input matrix (default 9)
    -M <num>     Change the vertical size of the input matrix (default 9)
    -p <num>     Change the vertical size of the PRF (default 3)
    -q <num>     Change the horizontal size of the PRF (default 3)
    -s <num>     Change the schema used by the PRF (default 0 -> RECTANGLE_ONLY)
                  other schemes 1->RECT_ROW, 2->RECT_COL, 3->ROW_COL, 4->RECT_TRECT
```

You can test the default configuration of the PRF ( the one preset in this repository ) running the testbench from the prfv3 folder with those arguments:
```
./RunRules/DFE/binaries/ExampleProject -p 2 -q 8 -N 256 -M 256 -s 3
```


