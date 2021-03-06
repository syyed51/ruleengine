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
#include "testRuleEngineInitialiser.cpp"

using namespace std;
using namespace sbx;

class Invaliditetsdaekning_KI_OSV_25_49 : public RuleEngineInitialiser {
protected:
    virtual void SetUp() {
    	RuleEngineInitialiser::SetUp();

        KonceptInfo ki {4, 30, 0, // UnderkonceptOid:OSV 25-49
        	{ {11, "true"}, // Parameter-Basis
        	  {1, "true"}, // Solidarisk faellestarif
			  {6, "true"} // SEB Firmapensionspulje
        	} };
        re.initContext(ki, OUTSIDE);
    }
};

class Invaliditetsdaekning_ITPension_50 : public RuleEngineInitialiser {
protected:
    virtual void SetUp() {
    	RuleEngineInitialiser::SetUp();

        KonceptInfo ki {ITPENSION, 50, 0, // UnderkonceptOid:OSV 25-49
        	{ {11, "true"}, // Parameter-Basis
        	  {1, "true"}, // Solidarisk faellestarif
			  {6, "true"} // SEB Firmapensionspulje
        	} };
        re.initContext(ki, OUTSIDE);
    }
};

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_Kr_Ingen) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Ingen");

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 100000);
	ta.setValue(kTAEBlOblMax, 200000); // now set amounts and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); //

}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_Kr_Ingen_NEGATIVE) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Ingen");

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 200000); // the span is too big, so should complain
	ta.setValue(kTAEBlOblMax, 900000); // now set max amounts to more than allowed
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk()); //

	EXPECT_EQ(2, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kValueOverLimit));
}


TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_Kr_Pristal) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Pristal");

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 100000);
	ta.setValue(kTAEBlOblMax, 200000); // now set amounts and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); //

}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_Kr_Pristal_NEGATIVE) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Pristal");

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 200000);
	ta.setValue(kTAEBlOblMax, 900000); // now set amounts and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk()); //

	EXPECT_EQ(2, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kValueOverLimit));
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_Kr_Pristal_Scientific_Output) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Pristal");

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 200000);
	ta.setValue(kTAEBlOblMax, 800000000); // now set amounts and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk()); //

	EXPECT_EQ(2, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kValueOverLimit));
}


TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_PCT_Gage) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Gage");

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(4, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEPctGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEPctOblMax, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kTAESpaendPct, kProductElementRequired));

	ta.setValue(kTAEPctGrMin, 40);
	ta.setValue(kTAEPctOblMax, 50); // now set pct and it should be only complain about missing amount
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk()); //
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 500000); // now set the min amount and execpt it to be ok
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); // expect to find TAEBlGrMin
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_PCT_Gage_NEGATIVE) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Gage");
	ta.setValue(kTAEPctGrMin, 10); // 20 is min
	ta.setValue(kTAEPctOblMax, 90); // 80 is max

	auto r = re.validate(ta, { kTAEReguleringskode, kTAEPctGrMin, kTAEPctOblMax, kTAESpaendPct});
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEPctGrMin, kValueUnderLimit));
	EXPECT_TRUE(r.hasMessages(kTAEPctOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendPct, kValueOverLimit));

	ta.setValue(kTAEPctGrMin, 60);
	ta.setValue(kTAEPctOblMax, 70); // now set allowed pct and it should be only complain about missing amount
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk()); // expect to find TAEBlGrMin message
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));

	ta.setValue(kTAEBlGrMin, 100000); // now set the min amount and execpt it to be ok
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); // expect to find TAEBlGrMin
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_PCT_Gage_NEGATIVE_Partial) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Gage");
	ta.setValue(kTAEPctGrMin, 10); // 20 is min
	ta.setValue(kTAEPctOblMax, 90); // 80 is max

	auto r = re.validate(ta, false);
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(4, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEPctGrMin, kValueUnderLimit));
	EXPECT_TRUE(r.hasMessages(kTAEPctOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendPct, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kProductElementRequired));

	ta.setValue(kTAEPctGrMin, 60);
	ta.setValue(kTAEPctOblMax, 70); // now set vallowed pct and it should be happy
	ta.setValue(kTAEBlGrMin, 200000); // now set vallowed pct and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); //
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_PCT_Ingen_NEGATIVE) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Ingen");
	ta.setValue(kTAEBlGrMin, -1); // 0 is min
	ta.setValue(kTAEBlOblMax, 800001); // 800000 is max

	auto r = re.validate(ta, { kTAEReguleringskode, kTAEBlGrMin, kTAEBlOblMax, kTAESpaendBl});
	EXPECT_FALSE(r.isAllOk());
//	if (!r.isAllOk())
		cout << r;
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kValueUnderLimit));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kValueOverLimit));

	ta.setValue(kTAEBlGrMin, 600000);
	ta.setValue(kTAEBlOblMax, 700000); // now set allowed amount and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); //
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_I_PCT_Ingen_NEGATIVE_Partial) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Ingen");
	ta.setValue(kTAEBlGrMin, -1); // 20 is min
	ta.setValue(kTAEBlOblMax, 800001); // 80 is max

	auto r = re.validate(ta, false);
//	if (!r.isAllOk())
		cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kValueUnderLimit));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAESpaendBl, kValueOverLimit));

	ta.setValue(kTAEBlGrMin, 600000);
	ta.setValue(kTAEBlOblMax, 700000); // now set allowed amount and it should be happy
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk()); //
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_SoliMax_TAEDaekningstype) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAESoliMax, "Tegningsmaks");

	auto r = re.validate(ta, false);
//	if (!r.isAllOk())
		cout << r;
	EXPECT_TRUE(r.isAllOk()); //

	ta.setValue(kTAESoliMax, "Obligatorisk maks");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEDaekningstype, kProductElementRequired));

	ta.setValue(kTAEDaekningstype, re.getDefaultValue(kTAEDaekningstype)->stringValue());
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());

	ta.setValue(kTAESoliMax, "Tegningsmaks");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEDaekningstype, kProductElementNotAllowed));

	r = re.validate(ta, false);
	cout << r;

	ta.remove(kTAEDaekningstype);
	ta.setValue(kTAESoliMax, "Ingen");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAEDaekningstype, kProductElementRequired));
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_Kortvarig_Overbygning_Frivillig) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kKortTAE_Daekning, "Frivillig");

	auto r = re.validate(ta, false);
//	if (!r.isAllOk())
		cout << r;
	EXPECT_FALSE(r.isAllOk()); // should be missing both KortTAE_Min_obl_faktor and KortTAE_Daekningsperiode
	EXPECT_EQ(2, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kKortTAE_Min_obl_faktor, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kKortTAE_Daekningsperiode, kProductElementRequired));

	ta.setValue(kKortTAE_Min_obl_faktor, "1");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(3, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kKortTAE_Min_obl_faktor, kValueNotAllowed));
	EXPECT_TRUE(r.hasMessages(kKortTAE_Daekningsperiode, kProductElementRequired));

	ta.setValue(kKortTAE_Min_obl_faktor, "0");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kKortTAE_Daekningsperiode, kProductElementRequired));

	ta.setValue(kKortTAE_Daekningsperiode, "6");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_Kortvarig_Overbygning_Obligatorisk) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kKortTAE_Daekning, "Obligatorisk");

	auto r = re.validate(ta, false);
//	if (!r.isAllOk())
		cout << r;
	EXPECT_FALSE(r.isAllOk()); // should be missing both KortTAE_Min_obl_faktor and KortTAE_Daekningsperiode
	EXPECT_EQ(2, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kKortTAE_Min_obl_faktor, kProductElementRequired));
	EXPECT_TRUE(r.hasMessages(kKortTAE_Daekningsperiode, kProductElementRequired));

	ta.setValue(kKortTAE_Min_obl_faktor, "2");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kKortTAE_Daekningsperiode, kProductElementRequired));

	ta.setValue(kKortTAE_Daekningsperiode, "6");
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());

	ta.setValue(kKortTAE_Min_obl_faktor, "4");
	re.printConstantsInParser();
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kKortTAE_Min_obl_faktor, kValueNotAllowed));

}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_TAEUdloebsalder_POSITIVE) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAE_Udlobsforskellig_MK, false);

	auto r = re.validate(ta, false);
//	if (!r.isAllOk())
		cout << r;
	EXPECT_TRUE(r.isAllOk()); // should be missing both KortTAE_Min_obl_faktor and KortTAE_Daekningsperiode

	ta.setValue(kTAE_Udlobsforskellig_MK, true);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAE_Udlobsalder, kProductElementRequired));

	ta.setValue(kTAE_Udlobsalder, 46);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_EQ(1, r.getValidationResults().size());
	EXPECT_TRUE(r.hasMessages(kTAE_Udlobsalder, kValueNotAllowed));

	ta.setValue(kTAE_Udlobsalder, 65);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());
}

TEST_F(Invaliditetsdaekning_KI_OSV_25_49, Invaliditetsdaekning_TAEBlGrMin_OverLimit) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Pristal");
	ta.setValue(kTAEBlGrMin, 800001);
	ta.setValue(kTAEBlOblMax, 800001);

	auto r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_TRUE(r.hasMessages(kTAEBlGrMin, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAEBlOblMax, kValueOverLimit));

	ta.setValue(kTAEBlGrMin, 800000);
	ta.setValue(kTAEBlOblMax, 800000);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());

	ta.setValue(kTAEBlGrMin, 799999);
	ta.setValue(kTAEBlOblMax, 799999);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());
}

TEST_F(Invaliditetsdaekning_ITPension_50, Invaliditetsdaekning_TAEBlGrMin_NotRequired) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Gage");
	ta.setValue(kTAEPctGrMin, 40);
	ta.setValue(kTAEPctOblMax, 50);

	cout << "is taeblgrmin allowed : " << boolalpha << re.isProductElementAllowed(kTAEBlGrMin) << endl;
	auto r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.hasMessages(kTAEBlGrMin));

	r = re.validate(ta, true);
	cout << r;
	EXPECT_FALSE(r.hasMessages(kTAEBlGrMin));
}

TEST_F(Invaliditetsdaekning_ITPension_50, Invaliditetsdaekning_TAEPctGrMin_OverLimit) {
	RuleEngine::_printDebugAtValidation = true;
	TA ta { "15124040" };
	ta.setValue(kTAEReguleringskode, "Gage");
	ta.setValue(kTAEPctGrMin, 61);
	ta.setValue(kTAEPctOblMax, 61);

	auto r = re.validate(ta, false);
	cout << r;
	EXPECT_FALSE(r.isAllOk());
	EXPECT_TRUE(r.hasMessages(kTAEPctGrMin, kValueOverLimit));
	EXPECT_TRUE(r.hasMessages(kTAEPctOblMax, kValueOverLimit));

	ta.setValue(kTAEPctGrMin, 60);
	ta.setValue(kTAEPctOblMax, 60);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());

	ta.setValue(kTAEPctGrMin, 59);
	ta.setValue(kTAEPctOblMax, 59);
	r = re.validate(ta, false);
	cout << r;
	EXPECT_TRUE(r.isAllOk());
}










