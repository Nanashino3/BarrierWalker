////////////////////////////////////////////
// �t�@�C�����FCTitleScene(�^�C�g�����)
// ��@���@���F2022/05/25

#include "CTitleScene.h"
#include "PlayGame/CSecondaryController.h"

#include "../Utility/CMenu.h"
#include "../Utility/CUtility.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace{
const std::string START = "�Q�[���J�n";
const std::string HELP  = "�w���v";
const std::string END   = "�Q�[���I��";

const int FIRST_LABEL_X = 450;	// �������x�����W�FX��
const int FIRST_LABEL_Y = 384;	// �������x�����W�FY��

const int ALIGNMENT_X = 20;		// ���x���\���Ԋu�FX��
const int ALIGNMENT_Y = 20;		// ���x���\���Ԋu�FY��

const std::vector<std::string> message_text = { START, HELP, END };

std::tuple<bool, std::string> result;
}

namespace Sequence
{
CTitleScene::CTitleScene()
: m_gfxHdl(0)
, m_titleGfxHdl(0)
, m_alphaValue(0)
, m_elapsed(0.0f)
{
	// �w�i�摜�̓ǂݍ���
	m_gfxHdl = Utility::LoadGraphEx("resource/pipo-bg003a.jpg");
	m_soundManager = Sound::CSoundManager::GetInstance();

	m_titleGfxHdl = Utility::LoadGraphEx("resource/string/title1.png");
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}

CTitleScene::~CTitleScene()
{
	Utility::DeleteGraphEx(m_gfxHdl);
	Utility::DeleteGraphEx(m_titleGfxHdl);
}

// ��ʂ̒��g���X�V
IScene* CTitleScene::Update(const CPrimaryController& control, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;

	// �w�i�摜�\��
	DrawRotaGraph(512, 384, 1.6f, 0, m_gfxHdl, true);

	// BGM���Ȃ��Ă��Ȃ��Ƃ������Đ�
	if(!m_soundManager->IsPlayBGM(BGM_ID_TITLE)){ m_soundManager->PlayBGM(BGM_ID_TITLE); }

	// �^�C�g����ʂ̃A�j���[�V����
	if(PriTitleAnimation(info)){ return next_scene; }
	
	// ���j���[�`��
	Utility::MenuDraw(message_text,
					  FIRST_LABEL_X, FIRST_LABEL_Y,
					  ALIGNMENT_X, ALIGNMENT_Y,
					  GetColor(255, 255, 0), GetColor(255, 255, 0));

	// �I���������j���[���̏���
	if(!std::get<0>(result)){
		// ���j���[�I��
		result = Utility::MenuSelect(message_text);
	}else{
		std::string text = std::get<1>(result);
		if(text == START){
			if(!Utility::FadeOut()){ return next_scene; }

			next_scene = new PlayGame::CSecondaryController(PlayGame::CSecondaryController::SECONDARY_ID_LOADING);
			m_soundManager->StopBGM(BGM_ID_TITLE);
		}else if(text == HELP){
			next_scene = new PlayGame::CSecondaryController(PlayGame::CSecondaryController::SECONDARY_ID_HELP);
		}else if(text == END){
			return nullptr;
		}else{
			tnl::DebugTrace("CTitleScene->Error No Text\n");
		}
		info.SetPrevScene(TITLE_SCENE);
	}

	return next_scene;
}

bool CTitleScene::PriTitleAnimation(ShareInfo::CDocGameInfo& info)
{
	m_elapsed += info.GetDeltaTime();
	if(m_elapsed > 0.05f){
		m_elapsed = 0.0f;
		if(m_alphaValue != 255){
			m_alphaValue += 5;
		}else{
			m_alphaValue = 255;
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaValue);
	DrawRotaGraph(512, 300, 1.0f, 0, m_titleGfxHdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return (m_alphaValue != 255) ? true: false;
}

} // namespace Sequence