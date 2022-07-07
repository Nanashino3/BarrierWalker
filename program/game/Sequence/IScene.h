#ifndef SEQUENCE_ISCENE_H_
#define SEQUENCE_ISCENE_H_

namespace ShareInfo{
	class CDocGameInfo;
}
namespace Sequence{
	class CPrimaryController;
}

namespace Sequence
{
class IScene
{
public:
	IScene(){}
	virtual ~IScene(){}

	virtual IScene* Update(const CPrimaryController&, ShareInfo::CDocGameInfo&) = 0;
};

} // namespace Sequence

#endif // #ifndef SEQUENCE_ISCENE_H_