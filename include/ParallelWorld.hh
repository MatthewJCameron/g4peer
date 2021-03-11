#ifndef ParallelWorld_h
#define ParallelWorld_h

#include "globals.hh"
#include "G4VUserParallelWorld.hh"
class PhaseSpaceSD;
class G4VPhysicalVolume;
class DetectorConstruction;

class ParallelWorld : public G4VUserParallelWorld
{
  public:
    ParallelWorld(G4String worldName, DetectorConstruction*);
    virtual ~ParallelWorld();

  public:
    virtual void Construct();
    PhaseSpaceSD* aPhaseSpaceSD;
    void SetScorerDistanceToSource(G4double);

    private:
    G4VPhysicalVolume* psfScorerPhys;
    DetectorConstruction* myDC;
};

#endif
