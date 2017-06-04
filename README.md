
[Cor++-Logo](https://github.com/tudo-astroparticlephysics/Cor-PlusPlus/wiki/images/GitHub_CORR_Logo.png?raw=true)
# Cor++

[![Build Status](https://travis-ci.org/tudo-astroparticlephysics/Cor-PlusPlus.svg?branch=master)](https://travis-ci.org/tudo-astroparticlephysics/Cor-PlusPlus) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/7fbe8beefb06494b8fab3ee0498ebb2f)](https://www.codacy.com/app/rangarik_91/Cor-PlusPlus?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=tudo-astroparticlephysics/Cor-PlusPlus&amp;utm_campaign=Badge_Grade)

This project containes multiple modules for the [CORSIKA](https://www.ikp.kit.edu/corsika/) simulation framework. The modules extend the functionality of the base program with the target to give the user more freedom and an easy API to customize the the simulation for specific experiments without knowledge about F77 or the internal structure of the simulation. The adjustment of the simualtioned to specific experiment allowes the reduction of unnecessary and redundant data created during the simulation.

## Content
1. [Manual](#manual)
2. [Installation](#installation)
3. [Versions](#versions)
4. [Copyright](#copyright)
5. [Contact](#contact)


- - -

## Manual
For more informations, manuals and examples visit the [Wiki](https://github.com/tudo-astroparticlephysics/Cor-PlusPlus/wiki) of this project. 

- - - 
## Installation
[CORSIKA](https://www.ikp.kit.edu/corsika/) already contains the last stable version of this module collection. When you want to update the version delivered with [CORSIKA](https://www.ikp.kit.edu/corsika/) you need to delete all files in the baack directory and overwrite them with the downloaded files. The newest working releases that are compatible with your version are in the table below. 

If you want to **upgrade an old [CORSIKA](https://www.ikp.kit.edu/corsika/) verion** that is incompatible with the needed version of Cor++ a patchfile can be provided upon request.

To compile the source code a compiler with C++11 support is neccesary (gcc > 5.0).

- - -

## Versions
Each version with the same major version number is guaranteed to create the same binary output. When changes due to bugs or further development do not allow to reproduce the same output file with the same steeringcard data the major version number is increased.

| [CORSIKA](https://www.ikp.kit.edu/corsika/) Ver. | Cor++ Version | 
|:------------:|:---------------------------:|
| 7.5          |  1.0.0 (patchfile available)  |
| next version |  1.0.0                        |
 


---

## Copyright
All parts in the "baack" directory are released under [LGPL](https://www.gnu.org/licenses/lgpl-3.0.html) license. In short: You can redistribute, use and modify the code as long as you credit the author in all projects the code or results from the code (e.g. MonteCarlo data) are used.

- - -
## Contact
If any issuse or problems arise fill an github issue. Otherwise you can contact the authoer email written in the [CORSIKA](https://www.ikp.kit.edu/corsika/) manual.


