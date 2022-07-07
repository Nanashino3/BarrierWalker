#ifndef FUNCTION_AFUNCTION_H_
#define FUNCTION_AFUNCTION_H_

namespace ShareInfo{ class CDocGameInfo; }
namespace Function
{
class AFunction
{
public:
	AFunction(unsigned int type) : m_function_type(type){}
	virtual ~AFunction(){}
 
	virtual void Initialize(){};
	virtual void Update(ShareInfo::CDocGameInfo&){};
	virtual void Draw(ShareInfo::CDocGameInfo&){};

protected:
	unsigned int m_function_type;
};

} // namespace Function

#endif // #ifndef FUNCTION_AFUNCTION_H_