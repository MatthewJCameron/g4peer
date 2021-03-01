#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <vector>
#include <fstream>
using namespace std;

class G4GeneralParticleSource;
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
    void GeneratePrimaries(G4Event*);
private:
    G4GeneralParticleSource* particleGun;//use this one for primary electron beam definition
};

#endif
