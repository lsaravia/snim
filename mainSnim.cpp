// 
// Sochastic Network Interaction Model
//
#include "snim.h"

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " SimulationParameterFile ModelParameterFile\n"
              << std::endl;
}


int main(int argc, char* argv[]){
    using namespace std;
    using namespace snim;

    if (argc < 3) {
        show_usage(argv[0]);
        return 1;
    }
    
    // Read simulation parameters from file
    //
    SimulationParameters sp(argv[1]);
        
    SnimModel mdl;
    mdl.ReadModelParams("model.par");
  
    cout << mdl << endl;
    

  return 0;
}