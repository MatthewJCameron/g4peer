// USER//
#include "PhysicsList.hh"
// GEANT4 //
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmLivermorePhysics.hh"

#include "G4EmProcessOptions.hh"
#include "G4ParticleTypes.hh"
#include "G4ParallelWorldScoringProcess.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4VEmProcess.hh"
#include "G4StepLimiter.hh"

#include "G4RegionStore.hh"
#include "G4PhysicsListHelper.hh"

#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList()
{
  RegisterPhysics(new G4EmLivermorePolarizedPhysics());
  //RegisterPhysics(new G4EmLivermorePhysics());
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{;}


void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
  G4EmProcessOptions em;
  em.SetApplyCuts(true);
  em.SetFluo(false);
  em.SetAuger(false);
  em.SetPIXE(false);
  em.SetVerbose(0);
  //add step limiter for electrons
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  ph->RegisterProcess(stepLimiter, G4Electron::ElectronDefinition());

  G4ParallelWorldScoringProcess* theParallelWorldScoringProcess = new G4ParallelWorldScoringProcess("ParaWorldScoringProc");
  theParallelWorldScoringProcess->SetParallelWorld("ParallelScoringWorld");

  G4ParticleDefinition* particle = G4Gamma::GammaDefinition();
  G4ProcessManager* pmanager = particle->GetProcessManager();
  pmanager->AddProcess(theParallelWorldScoringProcess);
  pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess ,idxAtRest);
  pmanager->SetProcessOrdering(theParallelWorldScoringProcess, idxAlongStep, 1);
  pmanager->SetProcessOrderingToLast(theParallelWorldScoringProcess, idxPostStep);
}

void PhysicsList::SetCuts()
{
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(250*eV, 10*MeV);
  SetCutValue(1 * mm, "gamma");
  SetCutValue(0.01 * mm, "e-");
  SetCutValue(1 * mm, "e+");
  if (verboseLevel>0) DumpCutValuesTable();
}