#include "PhaseSpaceSD.hh"
#include "RunAction.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4String.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"

PhaseSpaceSD::PhaseSpaceSD(G4String name):G4VSensitiveDetector(name)
{
    G4RunManager* theRunManager =  G4RunManager::GetRunManager();
    myRunAction = (RunAction*)theRunManager->GetUserRunAction();
    PhotonEnergy=0;
    PhotonWeight=0;
}

PhaseSpaceSD::~PhaseSpaceSD(){
}

void PhaseSpaceSD::Initialize(G4HCofThisEvent* )
{
}

G4bool PhaseSpaceSD::ProcessHits(G4Step* theStep,G4TouchableHistory*)
{
    if(theStep->GetTrack()->GetParticleDefinition()==G4Gamma::Gamma()){
        PhotonEnergy = theStep->GetTrack()->GetKineticEnergy();
        G4ThreeVector hitposition(theStep->GetPostStepPoint()->GetPosition());
        G4ThreeVector hitmomentum(theStep->GetPostStepPoint()->GetMomentumDirection());
        myRunAction->TallyPhaseSpaceHit(hitposition, hitmomentum, PhotonEnergy);
        //theStep->GetTrack()->SetTrackStatus(fStopAndKill); // this will kill the particle once recorded
    }else {
        G4cout << "**** non-photon hit to PSS, ignoring" << G4endl;
    }
    return true;
}

void PhaseSpaceSD::EndOfEvent(G4HCofThisEvent*){
}
