// 
// Sochastic Network Interaction Model
//
#include "snim.h"

static void show_usage(std::string name)
{
    std::cerr << "\nSNIM Stochactic Network Interaction Model\n\n " 
              << "Usage: " << name << " SimulationParameterFile ModelParameterFile [OutputFileName]\n"
              << std::endl;
}


int main(int argc, char* argv[]){
    using namespace std;
    using namespace snim;

    if (argc < 3) {
        show_usage(argv[0]);
        return 1;
    }
    
    SnimModel mdl;
    mdl.ReadModelParams(argv[2]);

    // Read simulation parameters from file
    //
    SimulationParameters sp(argv[1]);

  
    matrix <size_t> out;
    mdl.SimulTauLeap(sp,out);

    if( argc<3) {
        cout << mdl << endl;
        cout << sp << endl;
        cout << out << endl;
    }
    else {
        ofstream fout(argv[3]);
        fout << out; 
    }
    
  return 0;
}