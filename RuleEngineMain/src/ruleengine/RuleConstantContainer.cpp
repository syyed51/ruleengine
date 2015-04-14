/*
 * RuleConstantContainer.cpp
 *
 *  Created on: 20/03/2015
 *      Author: jfsvak
 */

#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

#include "RuleConstantContainer.h"
#include "sbxTypes.h"
#include "Constant.h"

using namespace std;

namespace sbx {

  /**
   * Copies the vector of Constants into the _globalConstants vector
   */
  void RuleConstantContainer::initGlobalConstants(const vector<Constant> &global_constants)
  {
	  // load the global constants into the container using copy-constructor
	  _globalConstants = global_constants;

	  // initialise the 3 maps of constant types
	  for ( auto constant : _globalConstants) {

  		  // equals goes into _ukOptionsMap as shared_ptr to the Constant in a vector
		  // min/max values goes into min/max-map as a shared_ptr to the Constant
		  switch(constant.getComparisonType()) {
		  	  case kEquals:
//		  		  _optionsMap[constant.getProductElement()].push_back(constant.stringValue());
		  		  _ukOptionsMap[constant.getUnderKonceptOid()][constant.getProductElement()].push_back(make_shared<Constant>(constant));
		  		  break;
		  	  case kMin:
			  	  _ukMinValuesMap[constant.getUnderKonceptOid()][constant.getProductElement()] = make_shared<Constant>(constant);
		  		  break;
			  case kMax:
				  _ukMaxValuesMap[constant.getUnderKonceptOid()][constant.getProductElement()] = make_shared<Constant>(constant);;
				  break;
		  	  default:
		  		  break;
		  }
	  }
  }

  /**
   * Initialises the local, current context for this RuleConstantContainer
   */
  void RuleConstantContainer::initContext(const short underKonceptOid, const short unionAgreementOid)
  {
	  // since three maps containing pointers to Constants are created in the initialisation of the globalConstants, switching context is simply to just set the underKonceptOid and unionAgreementOid
	  _underKonceptOid = underKonceptOid;
	  _unionAgreementOid = unionAgreementOid;
	  _contextInitialised = true;
  }

  /**
   * Gets a vector of strings with option values for productElement
   */
  vector<string> RuleConstantContainer::getOptions(const sbx::ProductElementNames productElement)
  {
	  if (_contextInitialised) {
		  std::vector<std::shared_ptr<Constant>> constantList = _ukOptionsMap[_underKonceptOid][productElement];
		  // create new vector of strings only
		  std::vector<string> stringOptions { };

		  for (auto constant : constantList) {
			  stringOptions.push_back(constant->stringValue());
		  }

		  return stringOptions;
	  }

	  throw domain_error("Context not initialised!");
  }

  /**
   * Gets a vector of shared_ptr->Constant
   */
  std::vector<std::shared_ptr<Constant>> RuleConstantContainer::getOptionsList(const sbx::ProductElementNames productElement)
  {
	  if (_contextInitialised) {
		  return _ukOptionsMap[_underKonceptOid][productElement];
	  }

	  throw domain_error("Context not initialised!");
  }

  /**
   * Gets a shared_ptr to a single Constant for the productElement and comparisonType
   */
  std::shared_ptr<sbx::Constant> RuleConstantContainer::getConstant(const sbx::ProductElementNames productElement, const sbx::ComparisonTypes comparisonType)
  {
	  if (_contextInitialised) {
		  switch(comparisonType)
		  {
		  case kMin:
			  return _ukMinValuesMap[_underKonceptOid][productElement];
			  break;
		  case kMax:
			  return _ukMaxValuesMap[_underKonceptOid][productElement];
			  break;
		  default:
			  throw domain_error("Only ComparisonType (Min, Max) supported");
		  }
	  }

	  throw domain_error("Context not initialised!");
  }

  /**
   * Outputs to cout the entire content of Constant Container
   */
  void RuleConstantContainer::printConstants() const
  {
	  cout << "\n       ---======== Global Constants =======--- \n\n";
	  printConstantHeader();

	  for (const auto& c : _globalConstants)
	  {
		  printConstant(make_shared<Constant>(c));
	  }

	  cout << "\n\nInitialised context: Underkoncept[" << _underKonceptOid << "] / UnionAgreement[" << _unionAgreementOid << "]" << endl;
	  cout << "\n\n       ---======== String Options =======--- \n";
	  printConstantHeader();

	  for (const auto& ukit : _ukOptionsMap)
	  {

		  for (const auto& peit : ukit.second)
		  {
			  for (const auto& c : peit.second)
			  {
				  printConstant(c);
			  }
		  }
	  }
	  cout << "\n\n       ---======== Min values Options =======--- \n\n";
	  printConstantHeader();

	  for (const auto& ukit : _ukMinValuesMap)
	  {
		  for (const auto& peit : ukit.second)
		  {
			  const auto c = peit.second;
			  printConstant(c);
		  }
	  }
	  cout << "\n\n       ---======== Max values Options =======--- \n\n";
	  printConstantHeader();

	  for (const auto& ukit : _ukMaxValuesMap)
	  {
		  for (const auto& peit : ukit.second)
		  {
			  const auto c = peit.second;
			  printConstant(c);
		  }
	  }
  }

  void RuleConstantContainer::printConstantHeader() const
  {
	  cout << "Address   UK  UA  PE  CT    strValue     double       long  isDefault" << endl;
	  cout << "--------  --  --  --  --  ----------  ---------  ---------  ---------" << endl;
  }
  void RuleConstantContainer::printConstant(const std::shared_ptr<sbx::Constant> c) const
  {
	  cout << c << setw(4) << c->getUnderKonceptOid() << setw(4) << c->getUnionAgreementOid() << setw(4) << c->getProductElement() << setw(4) << c->getComparisonType();
	  cout << right << setw(12) << c->stringValue() << right << setw(11) << c->doubleValue() << right << setw(11) << c->longValue() << setw(9) << (c->isDefault() ? "*" : "") << endl;
  }
} // sbx namespace end
