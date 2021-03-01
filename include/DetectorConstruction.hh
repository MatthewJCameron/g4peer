#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh 1
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
//#include "G4VSensitiveDetector.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;
class DetectorConstruction : public G4VUserDetectorConstruction {

public:
DetectorConstruction();
~DetectorConstruction();
G4VPhysicalVolume* Construct();
virtual void UpdateGeometry();
G4double GetWorldX(){return worldX;};
void SetSourceToPSFDistance(G4double val);
void deactivatePSF();
G4double GetSourceX(){return SourceX;}

private:
DetectorMessenger* detectorMessenger;
G4double worldX;
G4VPhysicalVolume* worldPhys;
G4double SourceX;
};
#endif
