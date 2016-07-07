#include "TestGeometry/CalorimeterSD.h"
#include "TestGeometry/CalorimeterHit.h"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

namespace test {
CalorimeterSD::CalorimeterSD(std::string name): G4VSensitiveDetector(name),
                                             m_HitsCollection(0),
                                             m_CellNo(1) {
  collectionName.insert("ECalorimeterColl");
}

CalorimeterSD::CalorimeterSD(std::string name, G4int aCellNoInAxis): G4VSensitiveDetector(name),
                                                                  m_HitsCollection(0),
                                                                  m_CellNo(aCellNoInAxis) {
  collectionName.insert("ECalorimeterColl");
}

CalorimeterSD::~CalorimeterSD() {}

void CalorimeterSD::Initialize(G4HCofThisEvent* hce) {
  m_HitsCollection = new CalorimeterHitsCollection(SensitiveDetectorName,collectionName[0]);
  hce->AddHitsCollection(G4SDManager::GetSDMpointer()->GetCollectionID(m_HitsCollection), m_HitsCollection);

  // fill calorimeter hits with zero energy deposition
  for (G4int ix=0;ix<m_CellNo;ix++)
    for (G4int iy=0;iy<m_CellNo;iy++)
      for (G4int iz=0;iz<m_CellNo;iz++)
      {
        CalorimeterHit* hit = new CalorimeterHit();
        m_HitsCollection->insert(hit);
      }
}

G4bool CalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());

  G4int yNo = touchable->GetCopyNumber(1);
  G4int xNo = touchable->GetCopyNumber(2);
  G4int zNo = touchable->GetCopyNumber(0);

  G4int hitID = m_CellNo*m_CellNo*xNo+m_CellNo*yNo+zNo;
  CalorimeterHit* hit = (*m_HitsCollection)[hitID];

  if(hit->GetXid()<0)
  {
    hit->SetXid(xNo);
    hit->SetYid(yNo);
    hit->SetZid(zNo);
    G4int depth = touchable->GetHistory()->GetDepth();
    G4AffineTransform transform = touchable->GetHistory()->GetTransform(depth);
    transform.Invert();
    hit->SetRot(transform.NetRotation());
    hit->SetPos(transform.NetTranslation());
  }
  hit->AddEdep(edep);
  return true;
}
}
