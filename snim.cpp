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

#include "snim.h"

namespace snim{ 

void SnimModel::SimulTauLeap(const SimulationParameters& sp, matrix<size_t>& N){
    using namespace std;
    
    // Check matrix size number of species must be the same 
    assert( omega.rows() == N.rows());
    
    // Initialize output matrix with initial populations
    //
    N(0,0)=communitySize;
    for( size_t i=1; i<N.rows(); ++i){
        N(i,0)= nIni[i];
        N(0,0)-=nIni[i];
//        cout << i << " - " << N(i,0) <<endl;
    }
//    cout << 0 << " - " << N(0,0) <<endl;
    
    // Setup random number generator with random seed
    // 
    auto rng = std::mt19937_64(sp.rndSeed);
   
    // Number of steps for each model evaluation 
    auto nSteps = 1.0 / sp.tau;
    
    // Number of species
    auto nSpecies = omega.rows();
    
    // Change in number of individuals for each event
    //
    matrix<size_t> intDelta(nSpecies,nSpecies);
    
    // Vector with only the non-zero elements of the matrix
    //
    vector< pair<size_t,size_t>> actualInteractions;
            
    for(auto s=1; s<nSpecies; ++s)
            for( auto r=0; r<nSpecies; ++r)
                if(omega(s,r)>0) {
                    actualInteractions.emplace_back(s,r);//  
                } 

    // Vectors for extinction and immigration 
    //
   
             
    // Calculate the transitions with poison random numbers
    //
    for (auto y = 0; y < sp.nEvals ; ++y){
        for(auto n=0; n < nSteps; ++n) {
            
            // Calculate interactions between species 
            //
            // Species 0 is the empty space
            for(auto t:actualInteractions){
                auto s=t.first;
                auto r=t.second;
                cout << s << " - " << r << " - " << omega(s,r) << " - " << omega(r,s) << " - " << N(s,n) << " - " ;
                double evRate =(omega(s,r)-omega(r,s))*N(s,n); //  
                auto pois = std::poisson_distribution<size_t>(evRate*sp.tau);
                intDelta(s,r) = pois(rng);
                cout <<  evRate << " - "<< intDelta(s,r) << endl;
                
            }
            cout << intDelta << endl;
            
            matrix<size_t>actN(nSpecies,1);
            actN.data() = N.col_cpy(n);
            
            for(auto s=1; s<nSpecies; ++s){
                
                // Calculate extinction
                double evRate = 0;
                evRate = actN(s)*e[s-1];
                auto pois = std::poisson_distribution<size_t>(evRate*sp.tau);
                auto exDelta= pois(rng);
                
                // Calculate immigration 
                evRate = N(0,n)*u[s-1];
                auto imPois = std::poisson_distribution<size_t>(evRate*sp.tau);
                auto imDelta= pois(rng);
                
                // Calculate new population values
                auto totDelta = imDelta - exDelta + intDelta.row_sum(s) - intDelta.col_sum(s);
                if( actN(s)+totDelta >0)
                    actN(s) += totDelta;
                else
                    actN(s) =0;
                
                if( actN(0) - totDelta <0)
                    actN(0) = 0;
                else 
                    actN(0) -= totDelta;
                        
                cout << s << " - " << imDelta << " - " << exDelta << " - " << intDelta.row_sum(s) << " - " << intDelta.col_sum(s) << " - " 
                                << N(s,n) << " - " <<  actN(s) << " - " << actN(0) << endl;
            
//                for(auto r=0; r<nSpecies; ++r){
//
//                    evDelta(s,r) = pois(rng);
//                    
//                    nDelta(s) += evDelta(s,r);
//                }
//                for(auto r:nSpecies){
//                    nDelta(s) -= evDelta(r,s);
//                }
                
            }

        }

    }
            
}
       

std::string SnimModel::ReadSimulParams(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

std::string SnimModel::ReadModelParams(const char *filename) {
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}


std::ostream& operator<<(std::ostream& os,  const SnimModel &  s) {
  os << "[Omega]\n" << s.omega << std::endl;
  
  os << "[Extinction]\n";
  os << s.e << std::endl;
  
  os << "[Immigration]\n";
  os << s.u << std::endl;

  os << "[Initial population]\n";
  os << s.nIni << std::endl;
  
  os << "[Total Size]\n[";
  os << s.communitySize << "]\n";
  
  return os;
}

} // end namespace