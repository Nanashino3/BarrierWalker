#ifndef SEQUENCE_PLAYGAME_ISCENE_H_
#define SEQUENCE_PLAYGAME_ISCENE_H_

namespace ShareInfo{
	class CDocGameInfo;
}

namespace Sequence{
namespace PlayGame{
	class CSecondaryController;
}}

namespace Sequence
{
namespace PlayGame
{
class IScene
{
public:
	virtual IScene* Update(CSecondaryController&, ShareInfo::CDocGameInfo&) = 0;
};

} // namespace PlayGame

} // namespace Sequence

#endif // #ifndef SEQUENCE_PLAYGAME_ISCENE_H_