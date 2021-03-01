#include "RunAction.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include <string.h>

RunAction::RunAction(){
  jobID = "";
  phaseSpaceOutputFilename = "./psftest";
  phaseSpaceScoringActive = true;
}

RunAction::~RunAction(){
  phaseSpaceOutputFile.close();
}

void RunAction::BeginOfRunAction(const G4Run* aRun){
  timerRun.Start();
  phaseSpaceOutputFilename = "psf_" + jobID + ".dat";
  if (phaseSpaceScoringActive){
    phaseSpaceOutputFile.open(phaseSpaceOutputFilename.c_str(),ios::out | ios::binary);
    if(phaseSpaceOutputFile.fail()) {G4cout << "Failed to open phaseSpaceOutputFile  " << phaseSpaceOutputFilename << " for writing " << G4endl;}
    else {G4cout << "*** Writing phase space information to " << phaseSpaceOutputFilename << G4endl;}
  }
}

void RunAction::EndOfRunAction(const G4Run* ){
  if(phaseSpaceOutputFile.is_open()) phaseSpaceOutputFile.close();
  timerRun.Stop();
  G4cout  << "*** Timer "  << timerRun << G4endl;
  G4cout <<  "*** Real " << timerRun.GetRealElapsed() << G4endl;
}

void RunAction::TallyPhaseSpaceHit(const G4ThreeVector& pos, const G4ThreeVector& mom,G4double E){
  G4double x = pos.x();
  G4double y = pos.y();
  G4double z = pos.z();
  G4double px = mom.x();
  G4double py = mom.y();
  G4double pz = mom.z();
  //G4cout << "* writing phase space pos " << pos << " mom " << mom << " E " << E << G4endl;
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&x),sizeof(G4double));
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&y),sizeof(G4double));
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&z),sizeof(G4double));
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&px),sizeof(G4double));
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&py),sizeof(G4double));
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&pz),sizeof(G4double));
  phaseSpaceOutputFile.write(reinterpret_cast<char *>(&E),sizeof(G4double));
}
