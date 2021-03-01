#ifndef _DetectorMessenger_H
#define _DetectorMessenger_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

class DetectorMessenger: public G4UImessenger
{

public:

    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();
    void SetNewValue(G4UIcommand*, G4String);

private:

    DetectorConstruction* detectorConstruction;
    G4UIdirectory*             Dir;
    G4UIcmdWithoutParameter*   UpdateCmd;
    G4UIcmdWithADoubleAndUnit* SetSourceToPSFDistanceCmd;   
    G4UIcmdWithoutParameter*   deactivatePSFCmd;
};

#endif
