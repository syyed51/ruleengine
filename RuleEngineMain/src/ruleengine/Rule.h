/*
 * Rule.h
 *
 *  Created on: 16/05/2015
 *      Author: jespe_000
 */

#ifndef RULEENGINE_RULE_H_
#define RULEENGINE_RULE_H_

#include <string>
#include <vector>
#include <memory>

namespace sbx {
class RuleCatalogue;

class Rule
{
public:
	Rule();
	Rule(const std::string& ruleId, const std::string& expr, std::shared_ptr<sbx::Rule>, const std::string& _positiveMessage, const std::string& _negativeMessage, const std::string& preCalcExpr = "");

	const std::string& getRuleId() const;
	const std::string& getExpr() const;
	const std::string& getPreCalcExpr() const;
	const std::string& getPositiveMessage() const;
	const std::string& getNegativeMessage() const;

	std::shared_ptr<sbx::Rule> getRequiredIfRule();
	std::shared_ptr<sbx::Rule> getNotAllowedIfRule();
	void setNotAllowedIfRule(std::shared_ptr<sbx::Rule>);
//	void setIsRequiredIfTrue(std::shared_ptr<sbx::Rule>);
	void addProductElementOid(unsigned short productElementOid);
	void addPreCalcRequiredPEOid(unsigned short requiredPEOid);
	const std::vector<unsigned short>& getProductElementOids() const;
	const std::vector<unsigned short>& getPreCalcRequiredPEOids() const;

//	void setRequiredExpr(const std::string& requiredExpr);
	void setPositiveRuleCatalogue(sbx::RuleCatalogue*);
	void setNegativeRuleCatalogue(sbx::RuleCatalogue*);

	sbx::RuleCatalogue* getPositiveRuleCatalogue();
	sbx::RuleCatalogue* getNegativeRuleCatalogue();

	virtual ~Rule();
	unsigned short getNegativeValCode() const;
	void setNegativeValCode(unsigned short negativeValCode);
	unsigned short getPositiveValCode() const;
	void setPositiveValCode(unsigned short positiveValCode);
	const std::vector<std::string>& getNegativeMessageParameters() const;
	void setNegativeMessageParameters(const std::vector<std::string>& negativeMessageParameters);
	void addNegativeMessageParameter(const std::string& parameter);

	const std::vector<std::string>& getPositiveMessageParameters() const;
	void setPositiveMessageParameters(const std::vector<std::string>& positiveMessageParameters);
	void addPositiveMessageParameter(const std::string& parameter);

	std::shared_ptr<sbx::Rule> getEvaluateExprIfRule() const;
	void setEvaluateExprIfRule(std::shared_ptr<sbx::Rule> evaluateExprIfRule);

private:
	std::string _ruleId;
	std::string _preCalcExpr;
	std::string _expr;
	std::vector<unsigned short> _productElementOids;
	std::vector<unsigned short> _preCalcRequiredPEOids;
	std::string _positiveMessage;
	std::string _negativeMessage;
	std::vector<std::string> _negativeMessageParameters;
	std::vector<std::string> _positiveMessageParameters;
	unsigned short _positiveValCode;
	unsigned short _negativeValCode;

	std::shared_ptr<sbx::Rule> _evaluateExprIfRule;
	std::shared_ptr<sbx::Rule> _requiredIfRule;
	std::shared_ptr<sbx::Rule> _notAllowedIfRule;

	sbx::RuleCatalogue* _positiveRuleCatalogue;
	sbx::RuleCatalogue* _negativeRuleCatalogue;
};

} /* namespace sbx */

#endif /* RULEENGINE_RULE_H_ */
