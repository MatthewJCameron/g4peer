#ifndef PhaseSpaceSD_HH
#define PhaseSpaceSD_HH 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4Track;

class RunAction;

//#include "G4ThreeVector.hh"

class PhaseSpaceSD : public G4VSensitiveDetector
{
public:
    PhaseSpaceSD(G4String);
    ~PhaseSpaceSD();
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);

private:
    RunAction* myRunAction;

    G4double PhotonEnergy;
    G4double PhotonWeight;
};


#endif // PhaseSpaceSD_HH

