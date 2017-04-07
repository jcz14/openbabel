/**********************************************************************
License terms go here
***********************************************************************/

#include <openbabel/babelconfig.h>
#include <openbabel/obmolecformat.h>

#include <mmtf_parser.h>
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
      return "MMTF plugin description goes here\n";
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

  /////////////////////////////////////////////////////////////////
  bool MMTFFormat::ReadMolecule(OBBase* pOb, OBConversion* pConv)
  {
	OBMol* pmol = pOb->CastAndClear<OBMol>();
	if(pmol==NULL)
		return false;

	istream &ifs = *pConv->GetInStream();
	OBMol &mol = *pmol;
	const char* title = pConv->GetTitle();
	MMTF_container* mmtfContainer = ifs;

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
//				printf("groupIndex: %d\n", groupIndex);
//				printf("groupId: %d\n", mmtfContainer->groupIdList[groupIndex]);
//				printf("insCodeList: %c\n", safechar(mmtfContainer->insCodeList[groupIndex]));
//				printf("secStruc: %d\n", mmtfContainer->secStructList[groupIndex]);
//				printf("seqIndex: %i\n", mmtfContainer->sequenceIndexList[groupIndex]);
//				printf("groupType: %d\n", mmtfContainer->groupTypeList[groupIndex]);
				MMTF_GroupType group = mmtfContainer->groupList[mmtfContainer->groupTypeList[groupIndex]];
//				printf("Group name: %s\n", group.groupName);
//				printf("Single letter code: %c\n", group.singleLetterCode);
//				printf("Chem comp type: %s\n", group.chemCompType);
				int atomOffset = atomIndex;

				int l;
				for (l = 0; l < group.bondOrderListCount; l++) {
					// ****** Issue here - I get print outs of the same each time ******
//					printf("Atom id One: %d\n", (atomOffset + group.bondAtomList[l * 2])); //  # atomIndex1
//					printf("Atom id Two: %d\n", (atomOffset + group.bondAtomList[l * 2 + 1])); //  # atomIndex2
//					printf("Bond order: %d\n", group.bondOrderList[l]);
				}
				int groupAtomCount = group.atomNameListCount;
				for (l = 0; l < groupAtomCount; l++) {
//					printf("atomIndex: %d\n", atomIndex);
//					printf("x coord: %f\n", mmtfContainer->xCoordList[atomIndex]);
//					printf("y coord: %f\n", mmtfContainer->yCoordList[atomIndex]);
//					printf("z coord: %f\n", mmtfContainer->zCoordList[atomIndex]);
//					printf("b factor: %f\n", mmtfContainer->bFactorList[atomIndex]);
//					printf("atom id: %d\n", mmtfContainer->atomIdList[atomIndex]);
//					printf("altLocList: %c\n", safechar(mmtfContainer->altLocList[atomIndex]));
//					printf("occupancy: %f\n", mmtfContainer->occupancyList[atomIndex]);
//					printf("charge: %d\n", group.formalChargeList[l]);
//					printf("atom name: %s\n", group.atomNameList[l]);
//					printf("element: %s\n", group.elementList[l]);

					OBAtom *atom  = mol.NewAtom();
					atom->SetAtomicNum(group.elementList[l]);
					atom->SetVector(mmtfContainer->xCoordList[atomIndex], mmtfContainer->yCoordList[atomIndex], mmtfContainer->zCoordList[atomIndex]);

					atomIndex++;
				}
				groupIndex++;
			}
			chainIndex++;
		}
		modelIndex++;
	}
//	printf("Number of inter group bonds: %d\n",	(int) mmtfContainer->bondOrderListCount);
	for (i = 0; i < mmtfContainer->bondOrderListCount; i++) {
		// ****** Issue here - seems too few (two entries for 4HHB).******
//		printf("Atom One: %d\n", mmtfContainer->bondAtomList[i * 2]);
//		printf("Atom Two: %d\n", mmtfContainer->bondAtomList[i * 2 + 1]);
//		printf("Bond order: %d\n", mmtfContainer->bondOrderList[i]);
	}
  }

  ////////////////////////////////////////////////////////////////

//  bool MMTFFormat::WriteMolecule(OBBase* pOb, OBConversion* pConv)
//  {
//
//  }

} //namespace OpenBabel
