#include <string>
#include <vector>
#include "gtest/gtest.h"

#include "../ruleengine/Constant_sbx.h"
#include "../ruleengine/KonceptInfo_sbx.h"
#include "../ruleengine/ProductElementValue_sbx.h"
#include "../ruleengine/RuleConstantContainer_sbx.h"
#include "../ruleengine/RuleEngine_sbx.h"
#include "../ruleengine/RuleCatalogue.h"
#include "../ruleengine/Rule.h"
#include "../ruleengine/sbxTypes.h"
#include "../ruleengine/TA_sbx.h"
#include "../ruleengine/ValidationResult.h"

#include "ruleenginetestutils.h"

using namespace std;
using namespace sbx;

class KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50 : public ::testing::Test  {
protected:
    virtual void SetUp() {
        re = RuleEngine();
        re.initConstants(get_file_contents("basedata-ruleconstants.json"));
        re.parseRuleCatalogueJSON(get_file_contents("rule-catalogue.json"));

        KonceptInfo ki {17, // UnderkonceptOid:OSV 25-49
        	{ {11, "true"}, // Parameter-Basis
        	  {1, "true"}, // Solidarisk faellestarif
        	  {15, "true"}, // FG Span - always selected on the KI
			  {6, "true"} // SEB Firmapensionspulje
        	} };
        re.initContext(ki);
    }

    RuleEngine re;
};


// Test KritiskSygdom_I_FG - not selected
TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_False_POSITIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, false );

	auto r = re.validate(ta, (unsigned short) kKritiskSygdom_i_FG_mk);
	cout << r;
	EXPECT_EQ(true, r.isAllOk());
}


// Test KritiskSygdom_I_FG - selected
TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_GAGE_POSITIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygReguleringskode, "Gage");
	ta.setValue(kKritiskSygPctMin, (long) 200);
	ta.setValue(kKritiskSygPctMax, (long) 300);
	ta.setValue(kKritiskSygSuppldaekn_mk, true);

	RuleEngine::_printDebugAtValidation = true;

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygReguleringskode,
			kKritiskSygPctMin,
			kKritiskSygPctMax,
			kKritiskSygspaendpc
	});
	cout << r;

	EXPECT_TRUE(r.isAllOk());
}

// Test KritiskSygdom_I_FG - selected
TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_Pristal_POSITIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygReguleringskode, "Pristal");
	ta.setValue(kKritiskSygBlMin, (long) 400000);
	ta.setValue(kKritiskSygBlMax, (long) 604935);

	RuleEngine::_printDebugAtValidation = true;

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygReguleringskode,
			kKritiskSygBlMin,
			kKritiskSygBlMax,
			kKritiskSygSpaendBl
	});
	cout << r;

	EXPECT_TRUE(r.isAllOk());
}

TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_Pristal_POSITIVE_FullTA) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygReguleringskode, "Pristal");
	ta.setValue(kKritiskSygSuppldaekn_mk, false);
	ta.setValue(kKritiskSygSkattekode, re.getDefaultValue(kKritiskSygSkattekode)->stringValue());
	ta.setValue(kKritiskSygBlMin, (long) 400000);
	ta.setValue(kKritiskSygBlMax, (long) 604935);

	RuleEngine::_printDebugAtValidation = true;

	auto r = re.validate(ta, false);
	cout << r;

	EXPECT_TRUE(r.isAllOk());
}

TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_SupplDaekning_POSITIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygSuppldaekn_mk, false);

	RuleEngine::_printDebugAtValidation = true;
	re.printConstantsInParser();

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygSuppldaekn_mk
	});
	cout << r;

	// all is expected to be ok, as we are only validating two specific pe's
	EXPECT_TRUE(r.isAllOk());
}


TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_Skattekode_POSITIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true );
	ta.setValue(kKritiskSygSkattekode, re.getDefaultValue(kKritiskSygSkattekode)->stringValue() );

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygSkattekode
	});
	cout << r;
	EXPECT_EQ(true, r.isAllOk());
}

//////////////////////////////////////////////////////////
// NEGATIVE SCENARIOS
//////////////////////////////////////////////////////////



// Test KritiskSygdom_I_FG - selected, but no value
//   Gage is selected but pct is mising and amount is set instead
// Expected result
//   fail with errors
TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_GAGE_NEGATIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygReguleringskode, "Gage");
	ta.setValue(kKritiskSygBlMin, (long) 300000);
	ta.setValue(kKritiskSygBlMax, (long) 400000);

	RuleEngine::_printDebugAtValidation = true;

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygReguleringskode,
			kKritiskSygBlMin,
			kKritiskSygBlMax,
			kKritiskSygSpaendBl
	});
	cout << r;

	EXPECT_FALSE(r.isAllOk());
}

// Test KritiskSygdom_I_FG - selected, but no value
//   Gage is selected but pct is mising and amount is set instead
// Expected result
//   fail with errors
TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_GAGE_NEGATIVE_UnderOverLimit) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygReguleringskode, "Gage");
	ta.setValue(kKritiskSygPctMin, (long) -1);
	ta.setValue(kKritiskSygPctMax, (long) 500);

	RuleEngine::_printDebugAtValidation = true;

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygReguleringskode,
			kKritiskSygPctMin,
			kKritiskSygPctMax,
			kKritiskSygspaendpc
	});
	cout << r;

	EXPECT_FALSE(r.isAllOk());

	auto v = r.getValidationResults(kKritiskSygPctMax);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kValueOverLimit, v.at(0).getValidationCode());

	v = r.getValidationResults(kKritiskSygPctMin);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kValueUnderLimit, v.at(0).getValidationCode());

	v = r.getValidationResults(kKritiskSygspaendpc);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kValueOverLimit, v.at(0).getValidationCode());
}

// Test KritiskSygdom_I_FG - selected, but no value
//   Gage is selected but pct is mising and amount is set instead
// Expected result
//   fail with errors
TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_True_Pristal_NEGATIVE_UnderOverLimit) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, true);
	ta.setValue(kKritiskSygReguleringskode, "Pristal");
	ta.setValue(kKritiskSygBlMin, (long) -1);
	ta.setValue(kKritiskSygBlMax, (long) 604936); // one more than max limit

	RuleEngine::_printDebugAtValidation = true;
	re.printConstantsInParser();

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygReguleringskode,
			kKritiskSygBlMin,
			kKritiskSygBlMax,
			kKritiskSygSpaendBl
	});
	cout << r;

	EXPECT_FALSE(r.isAllOk());

	auto v = r.getValidationResults(kKritiskSygBlMax);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kValueOverLimit, v.at(0).getValidationCode());

	v = r.getValidationResults(kKritiskSygBlMin);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kValueUnderLimit, v.at(0).getValidationCode());

	v = r.getValidationResults(kKritiskSygSpaendBl);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kValueOverLimit, v.at(0).getValidationCode());
}

TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_False_SupplDaekning_NEGATIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, false);
	ta.setValue(kKritiskSygSuppldaekn_mk, true);

	RuleEngine::_printDebugAtValidation = true;
	re.printConstantsInParser();

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygSuppldaekn_mk
	});
	cout << r;

	EXPECT_FALSE(r.isAllOk());

	auto v = r.getValidationResults(kKritiskSygSuppldaekn_mk);
	ASSERT_EQ(2, v.size());
//	EXPECT_EQ(sbx::ValidationCode::kProductElementNotAllowed, v.at(0).getValidationCode());
}

TEST_F(KritiskSygdom_I_FG_CONTEXT_KI_OSV_25_50, KritiskSygdom_I_FG_False_SupplDaekning_False_NEGATIVE) {
	TA ta { "15124040", 4}; // KonceptOid 4 - OSV
	ta.setValue(kKritiskSygdom_i_FG_mk, false);
	ta.setValue(kKritiskSygSuppldaekn_mk, false);

	RuleEngine::_printDebugAtValidation = true;
	re.printConstantsInParser();

	auto r = re.validate(ta, {
			kKritiskSygdom_i_FG_mk,
			kKritiskSygSuppldaekn_mk
	});
	cout << r;

	EXPECT_FALSE(r.isAllOk());

	auto v = r.getValidationResults(kKritiskSygSuppldaekn_mk);
	ASSERT_EQ(1, v.size());
	EXPECT_EQ(sbx::ValidationCode::kProductElementNotAllowed, v.at(0).getValidationCode());
}