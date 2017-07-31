
# SNIM: Stochastic Network Interaction model 

This is a C++11 implementation of a stochastic multispecies model. The model has multiple applications to investigate the relationship between ecological interactions (competition, predation) and diversity. It involves a discrete definition
of individuals belonging to a given set of species. The dynamics is essentially probabilistic at the microscopic
scale, but it leads to recognizable macroscopic patterns both in space and time. The model in is actual version is spatially implicit but a spatially explicit version is planned. A more complete definition of the model can be found in:  

1. Sole, R. V, Alonso, D. & Mckane, A. (2002). Self-organized instability in complex ecosystems. Philos. Trans. R. Soc. Lond. B. Biol. Sci., 357, 667–681

## Building	 using cmake

To build using cmake you should create a folder bin and run

```
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make
```

It could be usefull in windows systems to compile with static libraries and MinGW compiler
   
```
   cmake .. -G "MinGW Makefiles" -DLINK_STATIC_LIBS=ON 
   mingw32-make
```
   
   
## License

The code is released under the liberal
[Apache v2](http://www.apache.org/licenses/LICENSE-2.0) license, see **LICENSE.md**.

   Copyright 2017 Leonardo Ariel Saravia <lsaravia@ungs.edu.ar> 

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.



Documentation and examples are released under the
[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/) license.
