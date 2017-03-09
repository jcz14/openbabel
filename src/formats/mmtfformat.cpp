/**********************************************************************
License terms go here
***********************************************************************/

#include <openbabel/babelconfig.h>
#include <openbabel/obmolecformat.h>

#include <openbabel/mmtf_parser.h>

#include <sstream>

using namespace std;
namespace OpenBabel
{

  class MMTFFormat : public OBMoleculeFormat
  {
  public:
    //Register this format type ID
    MMTFFormat()
    {
      OBConversion::RegisterFormat("mmtf", this, "chemical/x-mmtf");
    }

    virtual const char* Description() //required
    {
      return "MMTF plugin description goes here\n";
    };

    virtual const char* SpecificationURL()
    {return "http://openbabel.org/wiki/MMTF";}; //optional

    virtual const char* GetMIMEType()
    { return "chemical/x-mmtf"; };

    //*** This section identical for most OBMol conversions ***
    ////////////////////////////////////////////////////
    /// The "API" interface functions
    virtual bool ReadMolecule(OBBase* pOb, OBConversion* pConv);
    virtual bool WriteMolecule(OBBase* pOb, OBConversion* pConv);
  };
  //***

  //Make an instance of the format class
  MMTFFormat theMMTFFormat;

  /////////////////////////////////////////////////////////////////
  bool MMTFFormat::ReadMolecule(OBBase* pOb, OBConversion* pConv)
  {
    //read data
  }

  ////////////////////////////////////////////////////////////////

  bool MMTFFormat::WriteMolecule(OBBase* pOb, OBConversion* pConv)
  {
    //write data
  }

} //namespace OpenBabel
