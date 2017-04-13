# Cor++

[![Build Status](https://travis-ci.org/tudo-astroparticlephysics/Cor-PlusPlus.svg?branch=master)](https://travis-ci.org/tudo-astroparticlephysics/Cor-PlusPlus)

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
[CORSIKA](https://www.ikp.kit.edu/corsika/) already contains the last stable version of this module collection. When you want to update the version delivered with [CORSIKA](https://www.ikp.kit.edu/corsika/) you need to delete all files in the baack directory and overwrite them with the downloaded files. The newest working releases are in the table below.

Additional to compile the source code a compiler with C++11 support is neccesary (gcc > 5.0).

- - -

## Versions
Each version with the same major version number is guaranteed to create the same binary output. When changes due to bugs or further development do not allow to reproduce the same output file with the same steeringcard data the major version number is increased.

| Corsika Ver. | Cor++ Version (Major.Minor) | 
|:------------:|:---------------------------:|
| 7.602        |  1.0                        |
|              |                             |
 


---

## Copyright
All parts in the "baack" directory are released under [LGPL](https://www.gnu.org/licenses/lgpl-3.0.html) license. In short: You can redistribute, use and modify the code as long as you credit the author in all prjects the code or results from the code (e.g. MonteCarlo data) is used.
The other directorys in this project are released under different licenses, see "Original Readme" further down or the specific license in the respective directory.

- - -
## Contact
If any issuse or problems arise fill an github issue. Otherwise you can contact my via GitHUB private message or under dominik.baack@udo.edu


