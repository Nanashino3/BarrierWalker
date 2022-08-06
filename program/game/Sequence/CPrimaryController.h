#ifndef SEQUENCE_CPRIMARYCONTROLLER_H_
#define SEQUENCE_CPRIMARYCONTROLLER_H_

namespace Sequence{ class IScene; }
namespace ShareInfo{ class CDocGameInfo; }

namespace Sequence
{
class CPrimaryController
{
public:
	CPrimaryController();
	~CPrimaryController();

	bool Update(ShareInfo::CDocGameInfo&);

private:
	IScene* m_currentScene;
};

} // namespace Sequence

#endif // #ifndef SEQUENCE_CPRIMARYCONTROLLER_H_