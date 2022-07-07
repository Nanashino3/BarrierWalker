#include "AFixedObject.h"

namespace GameObject
{
AFixedObject::AFixedObject()
{}

AFixedObject::~AFixedObject()
{}

void AFixedObject::Initialize(ShareInfo::CDocGameInfo&)
{}

void AFixedObject::Move(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&)
{}

void AFixedObject::Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&)
{}

void AFixedObject::Update(ShareInfo::CDocGameInfo&)
{}

void AFixedObject::Draw(ShareInfo::CDocGameInfo&)
{}

} // namespace GameObject