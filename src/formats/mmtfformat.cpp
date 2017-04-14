/**********************************************************************
License terms go here
***********************************************************************/

#include <openbabel/babelconfig.h>
#include <openbabel/obmolecformat.h>

#include "mmtf_parser.h"
#include <msgpack.h>

#include <sstream>
#include <stdio.h>
#include <string.h>

using namespace std;
namespace OpenBabel
{

  class MMTFFormat : public OBMoleculeFormat
  {
  public:
    MMTFFormat()
    {
      OBConversion::RegisterFormat("mmtf", this, "chemical/x-mmtf");
    }

    //Write support not yet implemented
    virtual unsigned int Flags()
    {
        return NOTWRITABLE;
    };

    virtual const char* Description()
    {
      return "MMTF - Macromolecular Transmission Format\n"
    		 "https://mmtf.rcsb.org/\n";
    };

    virtual const char* SpecificationURL()
    {return "http://openbabel.org/wiki/MMTF";};

    virtual const char* GetMIMEType()
    { return "chemical/x-mmtf"; };

    //*** This section identical for most OBMol conversions ***
    ////////////////////////////////////////////////////
    /// The "API" interface functions
    virtual bool ReadMolecule(OBBase* pOb, OBConversion* pConv);
    //virtual bool WriteMolecule(OBBase* pOb, OBConversion* pConv);
  };
  //***

  //Make an instance of the format class
  MMTFFormat theMMTFFormat;

  //Create an instance of the MMTF container
  MMTF_container* mmtfContainer = MMTF_container_new();

  /////////////////////////////////////////////////////////////////
  bool MMTFFormat::ReadMolecule(OBBase* pOb, OBConversion* pConv)
  {
	OBMol* pmol = pOb->CastAndClear<OBMol>();
	if(pmol==NULL)
		return false;

	istream &ifs = *pConv->GetInStream();
	OBMol &mol = *pmol;
	const char* title = pConv->GetTitle();

	char* input = new char [pConv->GetInLen()];
	ifs.read(input, pConv->GetInLen());
	MMTF_unpack_from_string(input, pConv->GetInLen(), mmtfContainer);

	// initialize index counters
	int modelIndex = 0;
	int chainIndex = 0;
	int groupIndex = 0;
	int atomIndex = 0;

	mol.SetTitle(title);
	mol.SetChainsPerceived();
	mol.BeginModify();

	// traverse models
	int i;
	for (i = 0; i < mmtfContainer->numModels; i++) {
		int modelChainCount = mmtfContainer->chainsPerModel[i];
//		printf("modelIndex: %d\n", modelIndex);
		// traverse chains
		int j;
		for (j = 0; j < modelChainCount; j++) {
//			printf("chainIndex : %d\n", chainIndex);
//			printf("Chain id: %s\n", mmtfContainer->chainIdList[chainIndex]);
//			printf("Chain name: %s\n", mmtfContainer->chainNameList[chainIndex]);
			int chainGroupCount = mmtfContainer->groupsPerChain[chainIndex];
			// traverse groups
			int k;
			for (k = 0; k < chainGroupCount; k++) {
				OBResidue *residue = mol.NewResidue();
//				printf("groupIndex: %d\n", groupIndex);
//				printf("groupId: %d\n", mmtfContainer->groupIdList[groupIndex]);
				residue->SetNum(mmtfContainer->groupIdList[groupIndex]);
//				printf("insCodeList: %c\n", safechar(mmtfContainer->insCodeList[groupIndex]));
//				printf("secStruc: %d\n", mmtfContainer->secStructList[groupIndex]);
//				printf("seqIndex: %i\n", mmtfContainer->sequenceIndexList[groupIndex]);
//				printf("groupType: %d\n", mmtfContainer->groupTypeList[groupIndex]);
				MMTF_GroupType group = mmtfContainer->groupList[mmtfContainer->groupTypeList[groupIndex]];
//				printf("Group name: %s\n", group.groupName);
				residue->SetName(group.groupName);
//				printf("Single letter code: %c\n", group.singleLetterCode);
//				printf("Chem comp type: %s\n", group.chemCompType);

				int atomOffset = atomIndex;
				int l;
				int groupAtomCount = group.atomNameListCount;

				for (l = 0; l < groupAtomCount; l++) {
					OBAtom *atom = mol.NewAtom();
					atom->SetTitle(group.atomNameList[l]);
					atom->SetId(mmtfContainer->atomIdList[atomIndex]);
					atom->SetVector(mmtfContainer->xCoordList[atomIndex], mmtfContainer->yCoordList[atomIndex], mmtfContainer->zCoordList[atomIndex]);
					atom->SetAtomicNum(etab.GetAtomicNum(group.elementList[l]));
					atom->SetFormalCharge(group.formalChargeList[l]);
					residue->AddAtom(atom);
					atomIndex++;
				}

				for (l = 0; l < group.bondOrderListCount; l++) {
					// ****** Issue here - I get print outs of the same each time ******
					OBAtom *firstAtom = mol.GetAtom(atomOffset + group.bondAtomList[l * 2] + 1);
					OBAtom *connectedAtom = mol.GetAtom(atomOffset + group.bondAtomList[l * 2 + 1] + 1);
					OBBond *bond = mol.GetBond(firstAtom, connectedAtom);
					bond->SetBondOrder(group.bondOrderList[l]);
				}
				groupIndex++;
			}
			chainIndex++;
		}
		modelIndex++;
	}
	for (i = 0; i < mmtfContainer->bondOrderListCount; i++) {
		// ****** Issue here - seems too few (two entries for 4HHB).******
		OBAtom *firstAtom = mol.GetAtom(mmtfContainer->bondAtomList[i * 2] + 1);
		OBAtom *connectedAtom = mol.GetAtom(mmtfContainer->bondAtomList[i * 2 + 1] + 1);
		OBBond *bond = mol.GetBond(firstAtom, connectedAtom);
		bond->SetBondOrder(mmtfContainer->bondOrderList[i]);
	}
	mol.EndModify();
	return(true);
  }

  ////////////////////////////////////////////////////////////////

//  bool MMTFFormat::WriteMolecule(OBBase* pOb, OBConversion* pConv)
//  {
//
//  }

} //namespace OpenBabel
