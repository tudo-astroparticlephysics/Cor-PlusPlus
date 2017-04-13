# Cor++

[![Build Status](https://travis-ci.org/tudo-astroparticlephysics/Cor-PlusPlus.svg?branch=master)](https://travis-ci.org/tudo-astroparticlephysics/Cor-PlusPlus)

This project containes multiple modules for the [CORSIKA](https://www.ikp.kit.edu/corsika/) simulation framework. The modules extend the functionality of the base program with the target to give the user more freedom and an easy API to customize the the simulation for specific experiments without knowledge about F77 or the internal structure of the simulation. The adjustment of the simualtioned to specific experiment allowes the reduction of unnecessary and redundant data created during the simulation.

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

## More Informations
For more informations, manuals and examples visit the [Wiki](https://github.com/tudo-astroparticlephysics/Cor-PlusPlus/wiki) of this project.

- - -
## Copyright
All parts in the "baack" directory are released under [LGPL](https://www.gnu.org/licenses/lgpl-3.0.html) license. In short: You can redistribute, use and modify the code as long as you credit the author in all prjects the code or results from the code (e.g. MonteCarlo data) is used.
The other directorys in this project are released under different licenses, see "Original Readme" further down or the specific license in the respective directory.

- - -
## Contact
If any issuse or problems arise fill an github issue. Otherwise you can contact my via GitHUB private message or under dominik.baack@udo.edu


