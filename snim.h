/*
 * Copyright 2017 Leonardo A. Saravia <lsaravia@ungs.edu.ar>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
  \file   snim.h
  \brief  Stochastic network interaction model class definition
 */
#ifndef SNIM_CLASS_HH_
#define SNIM_CLASS_HH_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <random>
#include <initializer_list>
#include <fstream>
#include <string>
#include <sstream>
#include <cerrno>
#include <utility>

#include "matrix.h"

namespace snim {

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almostEqual(T x, T y, int ulp=10)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
    // unless the result is subnormal
           || std::abs(x-y) < std::numeric_limits<T>::min();
}
    
    
struct SimulationParameters {
    size_t rndSeed=0;               // Seed of the random generator
    size_t nEvals=0;                // number of evaluations steps
    double tau=0.0;                   // Tau method steps  
    std::vector<size_t> iniCond;                 // Initial conditions 

    // size_t nSteps = nEvals*tau;     // total number of steps
    
    // Read simulations parameters from configuration file
    SimulationParameters(const std::string &fName); 
    SimulationParameters(): rndSeed(0),nEvals(0),tau(0.0), iniCond(){};
};

class SnimModel {

    // Model parameters 
    //
    
    matrix <float> omega;       // Interaction matrix
    std::vector<float> e;       // extinction vector
    std::vector<float> u;       // immigration parameter 
    size_t communitySize=0;           // Total size of the community
    size_t nSpecies=0;                 // Number of species
    

    // Simulation Parameters 
    SimulationParameters simPar;

    void ReadModelParamsLine(const std::string &line, size_t const lineNo);

    
public:
  SnimModel() : omega(), e(),u(), communitySize(0), nSpecies(0) {}

  SnimModel(size_t nsp, size_t comSize) : omega(nsp+1,nsp+1), e(nsp),u(nsp), communitySize(comSize), nSpecies(nsp) {}
  
  SnimModel(const SnimModel& s) : omega(s.omega), e(s.e),u(s.u), simPar(s.simPar), communitySize(s.communitySize), nSpecies(s.nSpecies) {}

  SnimModel& operator=(const SnimModel& s){
    if(this == &s )
        return *this;
    assert(omega.size()== s.omega.size());
    omega = s.omega;
    e = s.e;
    u = s.u;
    simPar = s.simPar;
    nSpecies = s.nSpecies;
    return *this;
  }
  
  /**
  \brief Set Interaction matrix in row-wise order
  */
  void SetOmega( std::initializer_list<float> const& om) {
//      matrix<float> temp(omega.rows(),omega.cols(),om);  
//      omega = temp;
      if (om.size() != omega.size()) {
            std::ostringstream message;
            message << "Different no. of elements: "
                    << "expected " << omega.size() << ", "
                    << "got " << om.size() << ".";

            throw std::invalid_argument(message.str());
        }

   
      // Standard assign omega = om is column-wise so I have to do it manually
      // in row-wise order
      //
      auto it=begin(om); 
      for (auto i = 0; i<omega.rows(); ++i)
        for (auto j = 0; j<omega.cols(); j++)
            omega(i,j) = *it++;
  };
  
  /**
  \brief Set Extinction vector 
  */
    void SetExtinction(std::initializer_list<float> const& om){
        assert(e.size()==om.size());
      e = om;
  };
  
  /**
  \brief Set Inmigration vector 
  */
  void SetInmigration(std::initializer_list<float> const& om){
      assert(u.size()==om.size());
      u = om;
  };
  
  /**
  \brief Set Initial value of species' populations  
  */
//  void SetInitialN(std::initializer_list<float> const& om ){
//    using namespace std;
//    assert((nIni.size()-1)==om.size());
//    copy(begin(om),end(om),begin(nIni)+1);
//  };
  
 
  void  ReadModelParams(const std::string &fName);

  /**
  \brief Simulate the model using the Tau-leap method   
  */
  void SimulTauLeap(const SimulationParameters & sp, matrix<size_t> & N );
  
  friend std::ostream& operator<<(std::ostream&,  const SnimModel&);
};

template<typename T>
std::ostream &operator <<(std::ostream &os, const std::vector<T> &v) {
    
    os << "[" << v[0];
    for (auto r=1u; r<v.size(); ++r)
    {
        os << ", " << v[r];
    }
    os << "]\n";

   return os;
}


} /* end namespace */

#endif