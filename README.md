# Private project for DynStack and RemoteControl development

[![Build Status](https://travis-ci.com/matyro/Cor-Mod.svg?token=zx68VQcwCmuM64VQoFqM&branch=master)](https://travis-ci.com/matyro/Cor-Mod)

This project containes two modules to speedup the [CORSIKA](https://www.ikp.kit.edu/corsika/) simulation framework. Both modules create an easy API to customize the simulation for specific experiments without knowledge about F77 or the internal structure of the simulation. The implemented knowledge about your specific problem allowes the reduction of unnecessary and redundant data created during the simulation.

## Content
1. [Requirements](#requiremnts)
2. [Modules](#modules)
  * [DynStack](#dynstack-module)
  * [RemoteControl](#remotecontrol-module)
  * [Basic](#basic-module)
3. [Usage](#usage)
  * [DynStack](#dynstack-usage)
  * [RemoteControl](#remotecontrol-usage)
  * [Basic](#basic-usage)
4. [Steering-Card](#steering-card)
5. [Installation](#installation)
6. [Tests](#tests)
7. [Showerview](#showerview)
8. [Copyright](#copyright)
9. [Contact](#contact)

- - - 
## Requirements
To download this repository GitHUBs [large file extension](https://git-lfs.github.com/) is needed. When you dont want to install this plugin you need to copy the run directory of the originial [CORSIKA](https://www.ikp.kit.edu/corsika/) into the run directory of the downloaded repository and select overwrite.
Additional to compile the source code a compiler with C++11 support is neccesary (gcc > 5.0).

- - -

## Modules
This projects contains 3 modules that are described in the following sections. You can find the source code of each module in the directory with matching name. Additional there is a _lib_ directory which includes parts that are needed in multiple modules.

### DynStack Module
The _DynamicStack_ module allowes an easy and fast customization of the internal particle stack. This gives the user access to every particle created in CORSIKA.

### RemoteControl Module
The _RemoteControl_ module implements a threaded communication layer inside of CORSIKA. It is now possible to communicate to external applications over TCP. This allowes the sampling of initial particle from external sources, the recieving from steeringcard parameter and the transmission of informations to and from the connected application.

### Basic Module
The _Basic_ module contains different classes that are used as interface to CORSIKA. It is used to get access to fundamental data and constants like the particle mass. Additional you can register callbacks to modify the four file header EVTH,EVTE,RUNH and RUNE. 

- - -
## Usage
Currently only DynStack and RemoteControl allow an easy customization. For this you can modify the "setup.h" file in each module directory. You can find examples in the dedicated examples directory that is included in every module.
### DynStack Usage


### RemoteControl Usage


### Basic Usage

- - -
## Steering Card

| Module        | Setting           | Parameter  | Description |
| ------------- |:-------------:| -----:| ----------|
| Dynstack      | DYNSTACK      | N1 ... Nn |  I user chosen number of integers that represent the size of the stack, it gets directly passed to the setup routine and the internal parts.         |
|               | DYNSTACKP |   N X1 ... XN |           |
| RemoteControl | REMOTECONTROL     |  DNS  |   Forward the remote servers address to the internal parts. Format can be an ip or a normal dns adress.        |

- - - 
## Installation
The installation is currently a two step process, because the make routine is not fully integrated in CORSIKA's configure routine. 
The first step is to execute **coconut** as normal and select all modules you need. Additional you want to select _DynStack_ (10a) or _RemoteControl_ (10b). Basic is automatically included when one of those options is choosen.
The next step is to finish the selection and chose **compile and keep source**. The script now tries to compile CORSIKA but it will fail with multiple missing functions.

<details>
<summary>>Old method (click me)<</summary>
The last step is to compile everything with the correct linking to the modules. For this access the baack directory and **execute makeBaack** (./makeBaack). When its finished you should get a executable named "corsika" at the normal location in the run directory.
</details>
Replacement method until I word out how to add my scripts to CORSIKAs ./configure:
The best way currently is to copy the last line that coconut prints before the error message apears and past it into a script in the corsika/src directory. It should look similar to 
```
/bin/bash ../libtool  --tag=F77   --mode=link gfortran -O0 -lm -I/opt/Fluka/gfor64/flukapro  -g -std=legacy -Wtabs  -D_FILE_OFFSET_BITS=64 -o corsika corsika-corsikacompilefile.o tobuf.o   corsika-qgsjet-II-04.o      timerc.o -L/home/dominik/workspace/Cor-Mod/lib/unknown -liact -lbern -latmext     -L/opt/Fluka/gfor64 -lflukahp
```
Now you need do add following things at the end of the line:
```
-L../baack/build/library/ -lDynstack -lRemote -lBasic -lLib -lm -lstdc++
```
Execute the script and CORSIKA should compile into the src directory. Copy the executable "corsika" into the run directory.



When you change the customization in the setup routines or and other part of this modules there is no need to rerun coconut. Simply execute makeBaack and the "corsika" executable will be created. The only reason to rerun coconut is when you changed Fortran source code or want to change modules.
Recompile the current module selection with **coconut -b**
Clear coconut cache when you change modules with **coconut -d**
- - -
## Tests
This project includes multiple tests that guarantee a working implementation of all modules. The makefile in the dbaack directory contains scripts to compile and execute all tests with **make Test**.
When additional tests are needed to check custom additions to internal parts of DynStack or RemoteControl you can find the files under baack/test.

- - -
## Showerview
![Shower Image](https://github.com/matyro/Showerview/blob/Raytracing_medium/examples/Schauer_2TeV.PNG)
[Showerview](https://github.com/matyro/Showerview) is a seperate project that allowes the display of [CORSIKA](https://www.ikp.kit.edu/corsika/) generated shower. It uses the dynstack implementation to write all particle information needed to a new file. You can find the implementation under [dynstack/examples/showerview.h](baack/dynstack/examples/showerview.h) and [dynstack/examples/showerview.cpp](baack/dynstack/examples/showerview.cpp).



- - -
## Copyright
All parts in the "baack" directory are released under [LGPL](https://www.gnu.org/licenses/lgpl-3.0.html) license. In short: You can redistribute, use and modify the code as long as you credit the author in all prjects the code or results from the code (e.g. MonteCarlo data) is used.
The other directorys in this project are released under different licenses, see "Original Readme" further down or the specific license in the respective directory.

- - -
## Contact
If any issuse or problems arise fill an github issue. Otherwise you can contact my via GitHUB private message or under dominik.baack@udo.edu

















<details>
<summary>Original CORSIKA Readme</summary>

To install CORSIKA, please use : ./coconut (without argument) and follow instructions.

The CORSIKA 7.5600 set consists of several files:

* a file README giving these short instructions how to proceed,

* an executable shell script "./coconut" to be used to install CORSIKA,

* a file AUTHORS giving the names of authors,

* a file ChangeLog giving CORSIKA history,

* a file COPYING giving copyright instructions,

* a file INSTALL giving instructions how to use "./coconut" to install 
  CORSIKA,

* a file NEWS giving latest news about CORSIKA,

* a file cDO_NOT_RUN_CONFIGURE to remind NOT to use ./configure but 
  ./coconut (without argument) instead,

* files Makefile.am, Makefile.in, acinclude.m4, aclocal.m4, configure.in, 
  configure, and subdirectory config/ needed for installation. These files 
  and directories should not be changed unless you know exactly what you 
  are doing.  All subdirectories have a Makefile.am and a Makefile.in 
  needed by ./configure to create proper Makefiles.

* a subdirectory bernlohr/ containing bernlohr package for Imaging
  Atmospheric Cherenkov Telescopes and extended atmospheres 
  (bernlohr-1.49.tar.gz is automatically extracted if needed).

* a subdirectory coast/ in which the COAST package is included.
  The COAST package consists of C++-routines which are the basis
  for the INCLINED, COREAS, COASTUSERLIB, and ROOTOUT options.

* a subdirectory conex/ containing the files which 
  might be used in combination with the CONEX option (conex-4.5000.tar.gz is 
  automatically extracted if needed),

* a subdirectory doc/ containing:
	* a .pdf file containing the physics description (FZKA 6019)
          CORSIKA_PHYSICS.pdf,
	* a .pdf file CORSIKA_GUIDE75600.pdf giving a description, how 
          to install and run CORSIKA,
	* a file references.tex which gives the actual references to
          CORSIKA and the hadronic interaction models in LaTeX format
          (to be used in your publications),
        * a file MPI-Runner_GUIDE.pdf which gives short instructions to
          run CORSIKA on a multi-core system using MPI (The file
          MPI-Runner_GUIDE.doc contains it in .doc format.),
        * a file MPI-Runner_PHYSICS.pdf describing the
          functionality of the PARALLEL option,

* a subdirectory dpmjet/ containing source files of the DPMJET2.55 model 
  (dpmjet253bc.f, dpmjet253c.f, dpmjet254.f, dpmjet255c.f, dpmjet256c.f),

* a subdirectory epos/ containing the source package for the EPOS model
  (epos-lhc-v3400_cors.tar.gz is automatically extracted if needed.),

* a subdirectory herwig/ where to install HERWIG 6510 and containing a
  README file inside,

* a subdirectory include/ containing the config.h.in file which is needed 
  by configure to do config.h. The file config.h contains all the 
  preprocessor commands needed by CORSIKA.

* a subdirectory lib/ is created when compiling CORSIKA. It contains all
  Makefiles, object files and libraries.

* a subdirectory nexus/ containing the source package for the NEXUS model
  (nexus3.97_cors.tar.gz is automatically extracted if needed.),

* a subdirectory pythia/ containing the modified and extended PYTHIA 
  package,

* a subdirectory run/ to be used to run CORSIKA containing:
        * 14 data sets containing the energy dependent cross sections for 
          nucleon-nucleus processes (NUCNUCCS), electromagnetic inter-
          actions (6 sets EGSDAT6_x.x), for QGSJET01 (QGSDAT01 and SECTNU),
	  for QGSJET-II (sectnu-II-04 and qgsdat-II-04),
          numerical data to be used by the VENUS routines (VENUSDAT),
	  cross section data for UrQMD (UrQMD-<ver>-xs.dat), 
          and for the DPMJET routines the Glauber tables (GLAUBTAR.DAT) 
          and the nuclear data (NUCLEAR.BIN).  [The NUCLEAR.BIN file 
          may be used only with those computers (DEC-UNIX, LINUX) 
          applying  the IEEE standard for direct access read and write. 
          For other computers (DEC-VAX, HP) a different NUCLEAR.BIN 
          file is necessary which is available from CERN or from the 
          author of DPMJET <johannes.ranft@cern.ch>.],
	* 8 input examples (all-inputs*, conex-3D-inputs, and parallel-inputs) 
          to steer the simulation with any model (with/out THIN, with STACKIN,
          with EPOS, with NEXUS, with CONEX, or with PARALLEL),
	* 3 data sets atmabs.dat, mirreff.dat, and quanteff.dat to take 
      	  into account the atmospheric absorption, mirror refletivity,
      	  and quantum efficiency of Cherenkov radiation,
	* the executable binary files of CORSIKA and utility programs 
          after compilation.

* a subdirectory src/ containing:
	* the major part of the source code (corsika.F and corsika.h) which 
          is a Fortran code file with some C preprocessor commands, that 
          contains the code of CORSIKA including the EGS4 and HDPM model 
          routines. It contains also the interfaces to FLUKA, GHEISHA, UrQMD, 
          DPMJET, EPOS, neXus, QGSJET, SIBYLL, and VENUS, but without those 
          hadronic interaction codes,
	* 5 separate files with the pure Fortran code of the GHEISHA, 
          QGSJET01D, QGSJET-II, SIBYLL2.3, and VENUS routines, respectively
          (gheisha_2002d.f, qgsjet01d.f, qgsjet-II-04.f, sibyll2.3.f, and 
          venus.f),
	* a separate file with the pure C-code of the EFIELD option 
          (elfield.c),
	* 3 special C-routines for compilation (timerc.c, tobuf.c, and 
          trapfpe.c), 
        * a subsubdirectory parallel/ containing the shell scripts and
          programs to run CORSIKA distributed on many CPUs in parallel.
          This directory also houses the userguide-pll-*.txt files
          which describe the parallel handling of CORSIKA on various
          CPU-clusters using these shell scripts, and some Fortran programs
          to handle the results of a parallel simulation.
        * a subsubdirectory preshower2.0-rev/ containing the C-programs for
          production of gamma-induced preshowers before reaching the top
          of atmosphere (preshw.c, utils.c, utils.h, veto.c, and veto.h),
        * a subsubdirectory utils/ containing utility programs:
            * a C++ routine to read the binary particle output file
              (corsikaread.cpp),
            * a Fortran routine cors2input.f to reestablish the used
              input from the binary particle output file,
	    * 3 Fortran routines to read the binary particle output files
              (corsikaread.f, corsikaread_history.f, and corsikaread_thin.f) 
            * a Fortran routine corsplitevts.f to split a large particle
              output file into several files containing single events each,
            * a C-routine map2png.c to visualize shower plots established 
              with the option PLOTSH2.
            * a Fortran routine modelprint.f which displays the used 
              high-energy and low-energy models from the binary particle
              output file,
            * a Fortran routine (plottracks3c.f) and its include file 
              (work.inc) to visualize shower plots established with the 
              option PLOTSH.
            * a Fortran routine readcsk2asci.f converting a binary
              particle output file into a (readable) ASCII file,
            * a Fortran routine readtimes.f to sum up the CPU times of 
              several .lst files (useful at parallel runs),
            * 2 Fortran routines showsimprods.f and showsimulist.f to 
              establish a tabular of several simulations giving 
              characteristic values of each simulation 
              (primary-id, energy, angles of incidence, ... ) 
              from the binary particle output files,
            * a Fortran routine sumlistnkginfo.f to sum up the NKG-output 
              of .lst files to get XMAX averaged over the used showers,
            * a Fortran routine sumlongifiles.f to sum up the content of 
              several .longi files (also in case of parallel simulations),
            * several other shell scripts and data handling programs to
              treat the CORSIKA output files.
   
* a subdirectory urqmd/ containing source package of the URQMD model
  (urqmd1.3_cors.tar.gz is automatically extracted if needed.).


Be aware, that many files have been modified since version CORSIKA 6.900. 
Use the more recent files with CORSIKA 7.5600.

The CORSIKA code and the files belonging to it can be obtained from 
KARLSRUHE INSTITUTE OF TECHNOLOGY, CAMPUS NORTH. 
You may take the CORSIKA75600 version including all files belonging
to it from our server  ikp-ftp.ikp.kit.edu  . The access to this server
is protected by password.  You may access this server:

 - Either using an internet browser going to the URL:
     ftp://ikp-ftp.ikp.kit.edu
giving username : corsika
and as password : (which you get by e-mail)
and change to the appropriate subdirectory by clicking on:
     corsika-v750
Finally you click on:
      corsika-75600.tar.gz
to get the most most recent version of corsika-75600.

 - Alternatively you access this server without browser, but using
linux ftp-commands.
Assuming lftp is installed on your client host (e.g. by the command
  sudo apt-get install lftp )
you give the commands for the secured connection:
  lftp
  lftp :~> set ssl:verify-certificate yes
  lftp :~> set ssl-allow yes
  lftp :~> open corsika@ikp-ftp.ikp.kit.edu
  Password: (which you have got by e-mail)
  lftp corsika@ikp-ftp.ikp.kit.edu:~> dir
Now you change to the subdirectory corsika-v750 by the command:
  lftp corsika@ikp-ftp.ikp.kit.edu:/> cd corsika-v750
and transfer the package by the command:
  lftp corsika@ikp-ftp.ikp.kit.edu:/corsika-v750> get corsika-75600.tar.gz
To exit the lftp give the command quit:
  lftp corsika@ikp-ftp.ikp.kit.edu:/corsika-v750> quit

************************************************************************
** If your download aborts often, it is possible that KIT-SCC intrusion
** detection has reset the connections because non secure transfers
** are no longer allowed in KIT.
** This leads to the message: connection reset by peer.
** Therefore we recommend to use lftp.
************************************************************************

Before using .gz files, you first have to decompress the files 
applying the gunzip procedure.

In case of problems with the data transfer or with the CORSIKA 
version 7.5xxx, please contact T. Pierog <tanguy.pierog@kit.edu> or 
D. Heck <dieter.heck@partner.kit.edu>.

The description of the physics used in the CORSIKA code (D. Heck et 
al., Report FZKA 6019 (1998) is available from the web page:
    https://www.ikp.kit.edu/corsika/70.php
In your publications you might use this report as reference to CORSIKA.



19.08.2016 by T. Pierog and D.Heck, KIT, IKP, Karlsruhe 


</details>
