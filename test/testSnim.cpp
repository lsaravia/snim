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

#include <gtest/gtest.h>
#include "snim.h"

TEST(snimTauLeap, Initial0_Final0){
    using namespace snim;

    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "No initial individuals, no individuals at end" << std::endl;
    
    SnimModel mdl(3,10000);

    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 1.0,
                   0.1, 0.0, 0.0, 0.3,
                   0.1, 0.0, 0.2, 0.0} );
    mdl.SetExtinction({0.5,0.5,0.5});
    mdl.SetInmigration({0.0,0.0,0.0});
    
    std::cout << mdl << std::endl;
    
    SimulationParameters sp {1234,1,0.01,
                                    0,0,0};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);
    EXPECT_EQ(10000,out(0,1));
    EXPECT_EQ(0,out(1,1));
    EXPECT_EQ(0,out(2,1));
    EXPECT_EQ(0,out(3,1));
       
}

TEST(snimTauLeap, Initial100_Im0_Out0){
     using namespace snim;

    
    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "Predator dies if there is no preys" << std::endl;
    
    SnimModel mdl(3,10000);

    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 1.0,
                   0.1, 0.0, 0.0, 0.3,
                   0.1, 0.0, 0.2, 0.0} );
    mdl.SetExtinction({2,0.5,0.5});
    mdl.SetInmigration({0.0,0.0,0.0});

    std::cout << mdl << std::endl;


    SimulationParameters sp = {1234,5,0.01,
                                    1000,0,0};
    std::cout << sp << std::endl;
    
    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 


    EXPECT_EQ(10000,out(0,5));
    EXPECT_EQ(0,out(1,5));
    EXPECT_EQ(0,out(2,5));
    EXPECT_EQ(0,out(3,5));
       
}


TEST(snimTauLeap, Initial10000_Im0_Ext0_Prey2_0){
     using namespace snim;

    
    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "Predator eats all prey 2 and never dies" << std::endl;
    std::cout << "Extinction [0,0,0]" << std::endl;
    SnimModel mdl(3,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 2.0, 0.0,
                   4.0, 0.0, 0.0, 0.0,
                   1.7, 0.0, 0.0, 0.0} );
    mdl.SetExtinction({0.0,0,0.0});
    mdl.SetInmigration({0.0,0.0,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                    2000,4000,4000};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_EQ(0   ,out(0,100));
    EXPECT_EQ(6000,out(1,100));
    EXPECT_EQ(0   ,out(2,100));
    EXPECT_EQ(4000,out(3,100));
       
}


TEST(snimTauLeap, Initial10000_Im0_Ext0_AllPrey_0){
     using namespace snim;

    
    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "Predator eats all preys and never dies" << std::endl;
    std::cout << "Extinction [0,0,0]" << std::endl;
    SnimModel mdl(3,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 2.0, 2.0,
                   4.0, 0.0, 0.0, 0.0,
                   1.7, 0.0, 0.0, 0.0} );
    mdl.SetExtinction({0.0,0,0.0});
    mdl.SetInmigration({0.0,0.0,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                            2000,4000,4000};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_EQ(0   ,out(0,100));
    EXPECT_EQ(10000,out(1,100));
    EXPECT_EQ(0   ,out(2,100));
    EXPECT_EQ(0   ,out(3,100));
       
}



TEST(snimTauLeap, Initial10000_Im0_Ext0_Prey2_win){
     using namespace snim;

    
    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "Predator don't eat and die, preys compete 2 win over 3" << std::endl;
    std::cout << "Extinction [0,0,0]" << std::endl;
    SnimModel mdl(3,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 0.0,
                   4.0, 0.0, 0.0, 0.3,
                   2.0, 0.0, 0.2, 0.0} );
    mdl.SetExtinction({1.0,0,0.0});
    mdl.SetInmigration({0.0,0.0,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                            2000,4000,4000};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_EQ(0   ,out(0,100));
    EXPECT_EQ(0,out(1,100));
    EXPECT_EQ(10000 ,out(2,100));
    EXPECT_EQ(0   ,out(3,100));
       
}

TEST(snimTauLeap, Initial1000_Im0_Ext121_Prey2_win){
     using namespace snim;

    
    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "Predator don't eats the two prey, preys don't compete" << std::endl;
    SnimModel mdl(3,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 3.0, 2.0,
                4.0, 0.0, 0.0, 0.0,
                 2.0, 0.0, 0.0, 0.0} );
    mdl.SetExtinction({1.0,2.0,1.0});
    mdl.SetInmigration({0.0,0.0,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                            200,400,400};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_GT(out(0,100),0);
    EXPECT_GT(out(1,100),0);
    EXPECT_GT(out(2,100),0);
    EXPECT_EQ(out(3,100),0);
       
}


TEST(snimTauLeap, Initial10000_Im0_Ext111_AllDie){
     using namespace snim;

    
    std::cout << "3 species 1 Predator 2 prey" << std::endl;
    std::cout << "Predator eats preys but preys don't growth everybody die" << std::endl;
    SnimModel mdl(3,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 3.0, 2.0,
                   0.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 0.0} 
    );
    mdl.SetExtinction({1.0,1.0,1.0});
    mdl.SetInmigration({0.0,0.0,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                            2000,4000,4000};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_EQ(out(0,100),10000);
    EXPECT_EQ(out(1,100),0);
    EXPECT_EQ(out(2,100),0);
    EXPECT_EQ(out(3,100),0);
       
}


TEST(snimTauLeap, Initial4000_Im0_Ext11_PredDie_Prey05){
     using namespace snim;

    
    std::cout << "2 species 1 Predator 1 prey" << std::endl;
    std::cout << "Predator don't eats preys die, preys grow at extinction/growth=2 density of preys=0.5" << std::endl;
    SnimModel mdl(2,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0,
                   2.0, 0.0, 0.0} 
    );
    mdl.SetExtinction({1.0,1.0});
    mdl.SetInmigration({0.0,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                            4000};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_NEAR(out(0,100),5000,100);
    EXPECT_EQ(out(1,100),0);
    EXPECT_NEAR(out(2,100),5000,100);
       
}


TEST(snimTauLeap, Initial4000_Im001_Ext11_Pred300_Prey4000){
     using namespace snim;

    
    std::cout << "2 species 1 Predator 1 prey" << std::endl;
    std::cout << "Predator don't survive if immigrations 0, preys grow at extinction/growth=2 density of preys less than 0.5" << std::endl;
    SnimModel mdl(2,10000);
    mdl.SetOmega( {0.0, 0.0, 0.0,
                   0.0, 0.0, 2.0,
                   2.0, 0.0, 0.0} 
    );
    mdl.SetExtinction({1.0,1.0});
    mdl.SetInmigration({0.01,0.0});

    std::cout << mdl << std::endl;

    SimulationParameters sp = {1234,100,0.01,
                                            4000};
    std::cout << sp << std::endl;

    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    std::cout << out << std::endl; 

    EXPECT_NEAR(out(0,100),5000,500);
    EXPECT_NEAR(out(1,100),300,100);
    EXPECT_NEAR(out(2,100),4000,500);
       
}