/**********************************************************************
License terms go here
***********************************************************************/

#include <openbabel/babelconfig.h>
#include <openbabel/obmolecformat.h>

#include "mmtf_parser.h"
#include "msgpack.h"

#include <sstream>

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

	int modelIndex = 0;
	int chainIndex = 0;
	int groupIndex = 0;
	int atomIndex = 0;

	mol.BeginModify();

	int i;
		for (i = 0; i < example->numModels; i++) {
			int modelChainCount = example->chainsPerModel[i];
			printf("modelIndex: %d\n", modelIndex);
			//    # traverse chains
			int j;
			for (j = 0; j < modelChainCount; j++) {
//				printf("chainIndex : %d\n", chainIndex);
//				printf("Chain id: %s\n", example->chainIdList[chainIndex]);
//				printf("Chain name: %s\n", example->chainNameList[chainIndex]);
				int chainGroupCount = example->groupsPerChain[chainIndex];
				//        # traverse groups
				int k;
				for (k = 0; k < chainGroupCount; k++) {
//					printf("groupIndex: %d\n", groupIndex);
//					printf("groupId: %d\n", example->groupIdList[groupIndex]);
//					printf("insCodeList: %c\n",
//							safechar(example->insCodeList[groupIndex]));
//					printf("secStruc: %d\n", example->secStructList[groupIndex]);
//					printf("seqIndex: %i\n", example->sequenceIndexList[groupIndex]);
//					printf("groupType: %d\n", example->groupTypeList[groupIndex]);
					MMTF_GroupType group = example->groupList[example->groupTypeList[groupIndex]];
//					printf("Group name: %s\n", group.groupName);
//					printf("Single letter code: %c\n", group.singleLetterCode);
//					printf("Chem comp type: %s\n", group.chemCompType);
					int atomOffset = atomIndex;
					int l;
					for (l = 0; l < group.bondOrderListCount; l++) {
						// ****** Issue here - > I get print outs of the same each time
//						printf("Atom id One: %d\n", (atomOffset + group.bondAtomList[l * 2])); //  # atomIndex1
//						printf("Atom id Two: %d\n", (atomOffset + group.bondAtomList[l * 2 + 1])); //  # atomIndex2
//						printf("Bond order: %d\n", group.bondOrderList[l]);
					}
					int groupAtomCount = group.atomNameListCount;
					for (l = 0; l < groupAtomCount; l++) {
						printf("atomIndex: %d\n", atomIndex);
						printf("x coord: %f\n", example->xCoordList[atomIndex]);
						printf("y coord: %f\n", example->yCoordList[atomIndex]);
						printf("z coord: %f\n", example->zCoordList[atomIndex]);
						printf("b factor: %f\n", example->bFactorList[atomIndex]);
						printf("atom id: %d\n", example->atomIdList[atomIndex]);
						printf("altLocList: %c\n", safechar(example->altLocList[atomIndex]));
						printf("occupancy: %f\n", example->occupancyList[atomIndex]);
						printf("charge: %d\n", group.formalChargeList[l]);
						printf("atom name: %s\n", group.atomNameList[l]);
						printf("element: %s\n", group.elementList[l]);
						atomIndex++;
					}
					groupIndex++;
				}
				chainIndex++;
			}
			modelIndex++;
		}

  }

  ////////////////////////////////////////////////////////////////

//  bool MMTFFormat::WriteMolecule(OBBase* pOb, OBConversion* pConv)
//  {
//    //write data
//  }

} //namespace OpenBabel
