//****************************************************************************
// �t�@�C�����FCSoundManager(�T�E���h�Ǘ���)
// ��@���@���F2022/08/06
#include "CSoundManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace Sound
{
CSoundManager* CSoundManager::s_myInstance = nullptr;

CSoundManager::CSoundManager()
{
	m_soundList = tnl::LoadCsv("sound/sound_info.csv");
	for(int i = 0; i < m_soundList.size(); i++){
		SOUND_INFO info;
		info.id = atoi(m_soundList[i][0].c_str());
		info.kind = atoi(m_soundList[i][1].c_str());
		info.hdl = LoadSoundMem(m_soundList[i][2].c_str());

		switch(info.kind)
		{
		case 0:	// BGM
			m_bgmList.push_back(info);
			break;
		case 1:	// SE
			m_seList.push_back(info);
			break;
		default:
			tnl::DebugTrace("Err�FCSoundManager KIND : [%d]", info.kind);
			break;
		}
	}
}

//****************************************************************************
// �֐����FCreateInstance
// �T�@�v�F�C���X�^���X����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
void CSoundManager::CreateInstance()
{
	if (s_myInstance == nullptr) { s_myInstance = new CSoundManager(); }
}

//****************************************************************************
// �֐����FGetInstance
// �T�@�v�F�C���X�^���X�擾
// ���@���F�Ȃ�
// �߂�l�F�C���X�^���X
// �ځ@�ׁF
//****************************************************************************
CSoundManager* CSoundManager::GetInstance()
{
	if (s_myInstance == nullptr) { CreateInstance(); }
	return s_myInstance;
}

//****************************************************************************
// �֐����FDestroyInstance
// �T�@�v�F�C���X�^���X�j��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
void CSoundManager::DestroyInstance()
{
	delete s_myInstance;
}

//****************************************************************************
// �֐����FPlaySE
// �T�@�v�FSE�Đ�
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�w�肵��ID��SE���Đ�����
//****************************************************************************
void CSoundManager::PlaySE(unsigned int id)
{
	if(id == m_seList[id].id){
		PlaySoundMem(m_seList[id].hdl, DX_PLAYTYPE_BACK);
	}else{
		tnl::DebugTrace("Err�FPlaySE ID : [%d]", id);
	}
}

//****************************************************************************
// �֐����FIsPlaySE
// �T�@�v�FSE���Đ�����Ă��邩
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�w�肵��ID��SE���Đ����Ă��邩
//****************************************************************************
bool CSoundManager::IsPlaySE(unsigned int id)
{
	bool result = false;
	if(id == m_seList[id].id){
		// true�F�Đ����Afalse�F�Đ����Ă��Ȃ�
		result = (CheckSoundMem(m_seList[id].hdl) == 1) ? true : false;
	}else{
		tnl::DebugTrace("Err�FIsPlaySE ID : [%d]", id);
	}
	return result;
}

//****************************************************************************
// �֐����FStopSE
// �T�@�v�FSE���~����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�w�肵��ID��SE���~������
//****************************************************************************
void CSoundManager::StopSE(unsigned int id)
{
	if(id == m_seList[id].id){
		StopSoundMem(m_seList[id].hdl);
	}else{
		tnl::DebugTrace("Err�FStopSE ID : [%d]", id);
	}
}

//****************************************************************************
// �֐����FPlayBGM
// �T�@�v�FBGM���Đ�����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�w�肵��ID��BGM���Đ�����
//****************************************************************************
void CSoundManager::PlayBGM(unsigned int id)
{
	if(id == m_bgmList[id].id){
		PlaySoundMem(m_bgmList[id].hdl, DX_PLAYTYPE_LOOP);
	}else{
		tnl::DebugTrace("Err�FPlayBGM ID : [%d]", id);
	}
}

//****************************************************************************
// �֐����FIsPlayBGM
// �T�@�v�FBGM���Đ�����Ă��邩
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�w�肵��ID��BGM���Đ�����Ă��邩
//****************************************************************************
bool CSoundManager::IsPlayBGM(unsigned int id)
{
	bool result = false;
	if(id == m_bgmList[id].id){
		// true�F�Đ����Afalse�F�Đ����Ă��Ȃ�
		result = (CheckSoundMem(m_bgmList[id].hdl) == 1) ? true : false;
	}else{
		tnl::DebugTrace("Err�FIsPlayBGM ID : [%d]", id);
	}
	return result;
}

//****************************************************************************
// �֐����FStopBGM
// �T�@�v�FBGM���~����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�w�肵��ID��BGM���~������
//****************************************************************************
void CSoundManager::StopBGM(unsigned int id)
{
	if(id == m_bgmList[id].id){
		StopSoundMem(m_bgmList[id].hdl);
	}else{
		tnl::DebugTrace("Err�FStopBGM ID : [%d]", id);
	}
}

//****************************************************************************
// �֐����FAllStopSound
// �T�@�v�F�S�ẴT�E���h���~����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�S�Ẳ��y���~������
//****************************************************************************
void CSoundManager::AllStopSound()
{
	for(int id = 0; id < m_seList.size(); id++){
		if(CheckSoundMem(m_seList[id].hdl) == 1){
			StopSoundMem(m_seList[id].hdl);
		}else if(CheckSoundMem(m_seList[id].hdl) < 0){
			tnl::DebugTrace("Err�FSE ID : [%d]", id);
		}else{
			// ���ɏ����Ȃ�
		}
	}
	for(int id = 0; id < m_bgmList.size(); id++){
		if(CheckSoundMem(m_bgmList[id].hdl) == 1){
			StopSoundMem(m_bgmList[id].hdl);
		}else if(CheckSoundMem(m_bgmList[id].hdl) < 0){
			tnl::DebugTrace("Err�FBGM ID : [%d]", id);
		}else{
			// ���ɏ����Ȃ�
		}
	}
}

} // namespace Sound