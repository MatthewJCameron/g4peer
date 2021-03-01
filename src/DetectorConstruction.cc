#include "G4Box.hh"
#include "G4ChordFinder.hh"
#include "G4FieldManager.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4TransportationManager.hh"
#include "G4Trd.hh"
#include "G4UImanager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4VisManager.hh"
#include "globals.hh"

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "ParallelWorld.hh"

DetectorConstruction::DetectorConstruction(){
  worldPhys = NULL;
  worldX=0*cm;
  detectorMessenger = new DetectorMessenger(this);
  //define materials
  G4NistManager* nist_manager = G4NistManager::Instance();
  G4Material* air = nist_manager->FindOrBuildMaterial("G4_AIR");
  //G4Material* Stainless = nist_manager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* Pb = nist_manager->FindOrBuildMaterial("G4_Pb");
  // Laboratory vacuum: Dry air (average composition)
  G4double density = 1.7836*mg/cm3 ;       // STP
  G4Material* ArgonGas = new G4Material( "Argon", density, ncomponents=1);
  ArgonGas->AddElement(Ar, 1);
  density = 1.25053*mg/cm3 ;       // STP
  G4Material* NitrogenGas = new G4Material( "N2", density, ncomponents=1);
  NitrogenGas->AddElement(N, 2);
  density = 1.4289*mg/cm3 ;       // STP
  G4Material* OxygenGas = new G4Material( "O2", density, ncomponents=1);
  OxygenGas->AddElement(O, 2);
  density  = 1.2928*mg/cm3 ;       // STP
  density *= 1.0e-8 ;              // pumped vacuum
  G4double temperature = STP_Temperature;
  G4double pressure = 1.0e-8*STP_Pressure;// heck this, may make difference over 100 m!
  G4Material* LaboratoryVacuum = new G4Material("LaboratoryVacuum", density,ncomponents=3, kStateGas,temperature,pressure);
  G4double fractionmass = 0.0;
  LaboratoryVacuum->AddMaterial( NitrogenGas, fractionmass = 0.7557 ) ;
  LaboratoryVacuum->AddMaterial( OxygenGas,   fractionmass = 0.2315 ) ;
  LaboratoryVacuum->AddMaterial( ArgonGas,    fractionmass = 0.0128 ) ;
  //constructing the geometry
  worldX=5*m;//
  G4double worldY=100*cm;
  G4double worldZ=100*cm;
  G4Box* worldBox = new G4Box("worldBox",worldX/2.,worldY/2.,worldZ/2.);
  G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, air,"worldLog",0,0,0);
  G4PhysicalVolume* worldPhys = new G4PVPlacement(0, G4ThreeVector(), worldLog,"worldPhys",0,false,0);
  worldLog->SetVisAttributes(G4VisAttributes::Invisible);
  //making lead box
  G4double leadX = 10*cm;
  G4Box* leadBox = new G4Box("leadBox",leadX/2.,leadY/2.,leadZ/2.);
  G4LogicalVolume* leadLog = new G4LogicalVolume(leadBox, Pbb,"leadLog",0,0,0);
  G4double posX=10*cm;
  G4double posY=10*cm;
  G4double posZ=10*cm;
  new G4PVPlacement(0, G4ThreeVector(posX,posY,posZ),"leadPhys", leadLog,worldPhys,"worldPhys",false,0,true);
}

DetectorConstruction::~DetectorConstruction(){
  if(detectorMessenger) delete detectorMessenger;
}

//this is called everytime we update the geometry (between runs)
void DetectorConstruction::UpdateGeometry(){
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  //need to tell run manager change in geometry, important for smart
  //voxel navigation (google it) used to optimise transport process
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  //in case we change material as well, need to recalc range cuts and physics tables
  #ifdef G4VIS_USE
  if(G4VVisManager::GetConcreteInstance()){
    G4cout << "*** found vis manager, re-drawing geometry *** " << G4endl;
    G4UImanager::GetUIpointer()->ApplyCommand("/control/execute macros/visrefresh.mac");
  }
  #endif
}

void DetectorConstruction::SetSourceToPSFDistance(G4double d){
  ((ParallelWorld*)GetParallelWorld(0))->SetScorerDistanceToSource(d);
}

void DetectorConstruction::deactivatePSF(){
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->Activate("aPhaseSpaceSD", false);
}