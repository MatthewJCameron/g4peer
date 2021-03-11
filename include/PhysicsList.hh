#ifndef PhysicsList_h
#define PhysicsList_h 1

// GEANT4 //
#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class PhysicsList: public G4VModularPhysicsList
{

public:
    PhysicsList();
    ~PhysicsList();
protected:
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();

};

#endif

