#include <stdlib.h>

//include definition of geant4 classes
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//include header files for user defined classes
#include "DetectorConstruction.hh"
#include "ParallelWorld.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

int main(int argc,char** argv){

  //fetch environment variables and assign to
  G4String jobid("0");
  char* pID = NULL;
  pID = getenv ("PBS_JOBID");//for the case of PBS queuing system
  //if (pID == NULL) pID = getenv ("SLURM_ARRAY_TASK_ID");//in case we use the new SLURM system on MASSIVE
  if (pID == NULL) pID = getenv ("SLURM_TASK_PID");//in case we use the new SLURM system on MASSIVE
  if (pID == NULL) pID = getenv ("ID");//in case someone is still using the old method on the local machine
  if (pID != NULL) {
    jobid.assign(pID);
  }

  G4cout << "Job id for this process " << jobid << G4endl;
  //Sset random number generator seeds based on job id
  G4int jobid_int = 0;
  istringstream ( jobid ) >> jobid_int;
  G4cout << "Seeding random number generator with " << jobid_int << G4endl;

  CLHEP::RanecuEngine randomEngine;
  CLHEP::HepRandom::setTheEngine(&randomEngine);
  CLHEP::HepRandom::setTheSeed(jobid_int);

  G4RunManager* runManager = new G4RunManager();
  DetectorConstruction* geometry = new DetectorConstruction();

  //register parallel world for scoring phase space
  geometry->RegisterParallelWorld(new ParallelWorld("ParallelScoringWorld", geometry));

  runManager->SetUserInitialization(geometry);
  //
  PhysicsList* physics = new PhysicsList();
  runManager->SetUserInitialization(physics);

  PrimaryGeneratorAction* beam = new PrimaryGeneratorAction();
  runManager->SetUserAction(beam);

  RunAction* runaction = new RunAction();
  //here we convert jobid from an integer to a string
  G4String jobid_int_str = static_cast<ostringstream*>( &(ostringstream() << jobid_int) )->str();
  runaction->SetJobID(jobid_int_str);
  runManager->SetUserAction(runaction);

  runManager->Initialize();

  G4UImanager* UI = G4UImanager::GetUIpointer();

  if (argc!=1)   // batch mode
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

  else //define visualization and UI terminal for interactive mode
  {
    #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    #endif

    #ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
    UI->ApplyCommand("/control/execute macros/vis.mac");
    ui->SessionStart();
    delete ui;
    #endif

    #ifdef G4VIS_USE
    delete visManager;
    #endif
  }

 // delete UI;
  delete runManager;//will automatically delete user classes as well
  return 0;

}
