#include "AMoveObject.h"

namespace GameObject
{
AMoveObject::AMoveObject()
{}
	
AMoveObject::~AMoveObject()
{}

void AMoveObject::Initialize(ShareInfo::CDocGameInfo&)
{}

void AMoveObject::Move(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&)
{}

void AMoveObject::Update(ShareInfo::CDocGameInfo&)
{}

void AMoveObject::Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&)
{}

void AMoveObject::Draw(ShareInfo::CDocGameInfo&)
{}

} // namespace GameObject