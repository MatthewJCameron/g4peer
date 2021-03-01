#include "G4ParticleTable.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(){
  particleGun = new G4GeneralParticleSource();//we use two types of particle guns depending on run phase
  G4double energy = 100*MeV;
  G4double de = 0.02*energy;//percent spread in beam energy
  G4double spotsizeY = 30*mm;
  G4double spotsizeZ = 30*mm;
  G4ThreeVector position = (0,0,0);
  G4ThreeVector momentumDirection = (0,0,0);
  G4RunManager* theRunManager =  G4RunManager::GetRunManager();
  DetectorConstruction* myDC = (DetectorConstruction*)theRunManager->GetUserDetectorConstruction();
  G4double worldX = myDC->GetWorldX();
  position.setX(-worldX/2.0);
  G4double yoffset=0.0*mm;
  position.setY(yoffset);//yoffset and thetayOffset use to steer electron beam such that
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("e-");
  momentumDirection.set(1,0,0);//ditto for mom direction
  
  particleGun->GetCurrentSource()->SetParticleDefinition(particle);
  particleGun->GetCurrentSource()->GetPosDist()->SetPosDisType("Beam");
  particleGun->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Gauss");

  particleGun->GetCurrentSource()->GetPosDist()->SetPosRot1(G4ThreeVector(0,1,0));
  particleGun->GetCurrentSource()->GetPosDist()->SetPosRot2(G4ThreeVector(0,0,1));
  particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(momentumDirection);
  particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(position);
  particleGun->GetCurrentSource()->GetPosDist()->SetBeamSigmaInX(spotsizeY);//symmetric
  particleGun->GetCurrentSource()->GetPosDist()->SetBeamSigmaInY(spotsizeZ);//symmetric
  particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy);
  particleGun->GetCurrentSource()->GetEneDist()->SetBeamSigmaInE(de);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  if(particleGun) delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent){
    particleGun->GeneratePrimaryVertex(anEvent);
}
