#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4SDManager.hh"

#include "G4RunManager.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"

#include "DetectorConstruction.hh"
#include "ParallelWorld.hh"
#include "PhaseSpaceSD.hh"

#include "G4Colour.hh"
#include "G4VisAttributes.hh"

ParallelWorld::ParallelWorld(G4String worldName, DetectorConstruction* dc)
:G4VUserParallelWorld(worldName)
{
  aPhaseSpaceSD=NULL;
  psfScorerPhys=NULL;
  myDC = dc;//need to copy pointer to  member pointer of this class
}

ParallelWorld::~ParallelWorld()
{
      if(aPhaseSpaceSD) delete aPhaseSpaceSD;
}

void ParallelWorld::Construct()
{
  G4VPhysicalVolume* worldPhysical = GetWorld();
  G4LogicalVolume* worldLogical = worldPhysical->GetLogicalVolume();
  worldLogical->SetVisAttributes(G4VisAttributes::Invisible);
  G4Box * psfScorerSolid = new G4Box("psfScorerSolid", 1.*mm, 30.*mm, 2*mm);
  G4LogicalVolume * psfScorerLogical   = new G4LogicalVolume(psfScorerSolid, 0, "psfScorerLogical", 0, 0, 0);
  psfScorerLogical->SetVisAttributes(G4Color::Red());
  G4Box* worldBox = (G4Box*)worldLogical->GetSolid();
  G4double worldhalfX = worldBox->GetXHalfLength();
   G4double posX = worldhalfX - 10*mm; //
  //Place phase space scorer at end of world volume by default
  //this will need to be modified using
  //UI commands.
  psfScorerPhys  = new G4PVPlacement(0, G4ThreeVector(posX,0,0), psfScorerLogical,"psfScorerPhysical", worldLogical, 0, 0);
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(!aPhaseSpaceSD)  {
        aPhaseSpaceSD = new PhaseSpaceSD( "aPhaseSpaceSD" );
        SDman->AddNewDetector( aPhaseSpaceSD );
    }
  psfScorerLogical->SetSensitiveDetector(aPhaseSpaceSD);
}

void ParallelWorld::SetScorerDistanceToSource(G4double distance)
{
  G4cout << "Changing psf distance to " << distance/m << " m from source " << G4endl;
  G4VPhysicalVolume* worldPhysical = GetWorld();
  G4Box* worldBox = (G4Box*)worldPhysical->GetLogicalVolume()->GetSolid();
  G4double worldhalfX = worldBox->GetXHalfLength();
  G4double xpos = - worldhalfX + myDC->GetSourceX()/2.0 + distance;
  G4cout << "New position of PSF scorer " << xpos/m << " m " << G4endl;
  psfScorerPhys->SetTranslation(G4ThreeVector(xpos,0.,0.));
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
