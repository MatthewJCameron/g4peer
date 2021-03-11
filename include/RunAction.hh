#ifndef RUNACTION
#define RUNACTION 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4Timer.hh"

#include <fstream>

using namespace std;

class RunAction : public G4UserRunAction {

public:
RunAction();
~RunAction();

void BeginOfRunAction(const G4Run* aRun);
void EndOfRunAction(const G4Run* aRun);

void TallyPhaseSpaceHit(const G4ThreeVector&, const G4ThreeVector&, G4double);
void SetJobID(G4String val){jobID = val;};
void DeactivatePhaseSpaceScoring(){phaseSpaceScoringActive = false;};
G4String GetJobID(){ return jobID; };

private:
G4String phaseSpaceOutputFilename;
G4String jobID;
fstream phaseSpaceOutputFile;
G4int NumberOfHistories;
G4Timer timerRun;
G4bool phaseSpaceScoringActive;

};

#endif
