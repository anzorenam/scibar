#ifndef SciCRTScintSD_h
#define SciCRTScintSD_h 1

#include "SciCRTScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class SciCRTScintSD : public G4VSensitiveDetector
{
  public:

    SciCRTScintSD(G4String name);
    virtual ~SciCRTScintSD();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
 
  private:

    SciCRTScintHitsCollection* fScintCollection;
 
};

#endif
