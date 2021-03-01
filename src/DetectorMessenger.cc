  #include "G4UIcmdWithAString.hh"
  #include "G4UIcmdWithoutParameter.hh"
  #include "G4UIdirectory.hh"
  #include "G4UIcmdWithADoubleAndUnit.hh"
  #include "G4UIcmdWith3VectorAndUnit.hh"
  #include "G4RunManager.hh"

  #include "DetectorConstruction.hh"
  #include "DetectorMessenger.hh"
  #include "RunAction.hh"
  #include "PrimaryGeneratorAction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* DC){

  detectorConstruction = DC;
 
  Dir = new G4UIdirectory("/g4peer/");
  Dir->SetGuidance("UI commands of the g4peer code");

  UpdateCmd = new G4UIcmdWithoutParameter("/g4imbl/dc/updateGeometry",this);
  UpdateCmd->SetGuidance("Update visualisation of geometry.");
  UpdateCmd->AvailableForStates(G4State_Idle);

  SetSourceToPSFDistanceCmd = new G4UIcmdWithADoubleAndUnit("/g4imbl/setSourceToPSFDistance",this);
  SetSourceToPSFDistanceCmd->SetGuidance("Set source to phase space scorer distance. ");
  SetSourceToPSFDistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  SetSourceToPSFDistanceCmd->SetDefaultUnit("mm");

  deactivatePSFCmd = new G4UIcmdWithoutParameter("/g4imbl/dc/deactivatePhaseSpaceScoring",this);
  deactivatePSFCmd->SetGuidance("deactivate phase space scoring (not needed for dose calcs)");
  deactivatePSFCmd->AvailableForStates(G4State_Idle);
}

DetectorMessenger::~DetectorMessenger(){
    delete Dir;
	  delete UpdateCmd;
    delete SetSourceToPSFDistanceCmd;
    delete deactivatePSFCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
  G4RunManager*  theRunManager =  G4RunManager::GetRunManager();
  RunAction* ra = (RunAction*)theRunManager->GetUserRunAction();
  if( command == UpdateCmd ) detectorConstruction->UpdateGeometry();
  else if(command == SetSourceToPSFDistanceCmd){detectorConstruction->SetSourceToPSFDistance( SetSourceToPSFDistanceCmd->GetNewDoubleValue(newValue));}
  else if(command == deactivatePSFCmd){
    detectorConstruction->deactivatePSF();
	  G4cout << "DC deactive" << G4endl;
    ra->DeactivatePhaseSpaceScoring();  
	  G4cout << "RA deactive" <<G4endl;
  }
}
