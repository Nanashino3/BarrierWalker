//****************************************************************************
// ファイル名：CSoundManager(サウンド管理者)
// 作　成　日：2022/08/06
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
			tnl::DebugTrace("Err：CSoundManager KIND : [%d]", info.kind);
			break;
		}
	}
}

//****************************************************************************
// 関数名：CreateInstance
// 概　要：インスタンス生成
// 引　数：なし
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CSoundManager::CreateInstance()
{
	if (s_myInstance == nullptr) { s_myInstance = new CSoundManager(); }
}

//****************************************************************************
// 関数名：GetInstance
// 概　要：インスタンス取得
// 引　数：なし
// 戻り値：インスタンス
// 詳　細：
//****************************************************************************
CSoundManager* CSoundManager::GetInstance()
{
	if (s_myInstance == nullptr) { CreateInstance(); }
	return s_myInstance;
}

//****************************************************************************
// 関数名：DestroyInstance
// 概　要：インスタンス破棄
// 引　数：なし
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CSoundManager::DestroyInstance()
{
	delete s_myInstance;
}

//****************************************************************************
// 関数名：PlaySE
// 概　要：SE再生
// 引　数：なし
// 戻り値：なし
// 詳　細：指定したIDのSEを再生する
//****************************************************************************
void CSoundManager::PlaySE(unsigned int id)
{
	if(id == m_seList[id].id){
		PlaySoundMem(m_seList[id].hdl, DX_PLAYTYPE_BACK);
	}else{
		tnl::DebugTrace("Err：PlaySE ID : [%d]", id);
	}
}

//****************************************************************************
// 関数名：IsPlaySE
// 概　要：SEが再生されているか
// 引　数：なし
// 戻り値：なし
// 詳　細：指定したIDのSEが再生しているか
//****************************************************************************
bool CSoundManager::IsPlaySE(unsigned int id)
{
	bool result = false;
	if(id == m_seList[id].id){
		// true：再生中、false：再生していない
		result = (CheckSoundMem(m_seList[id].hdl) == 1) ? true : false;
	}else{
		tnl::DebugTrace("Err：IsPlaySE ID : [%d]", id);
	}
	return result;
}

//****************************************************************************
// 関数名：StopSE
// 概　要：SEを停止する
// 引　数：なし
// 戻り値：なし
// 詳　細：指定したIDのSEを停止させる
//****************************************************************************
void CSoundManager::StopSE(unsigned int id)
{
	if(id == m_seList[id].id){
		StopSoundMem(m_seList[id].hdl);
	}else{
		tnl::DebugTrace("Err：StopSE ID : [%d]", id);
	}
}

//****************************************************************************
// 関数名：PlayBGM
// 概　要：BGMを再生する
// 引　数：なし
// 戻り値：なし
// 詳　細：指定したIDのBGMを再生する
//****************************************************************************
void CSoundManager::PlayBGM(unsigned int id)
{
	if(id == m_bgmList[id].id){
		PlaySoundMem(m_bgmList[id].hdl, DX_PLAYTYPE_LOOP);
	}else{
		tnl::DebugTrace("Err：PlayBGM ID : [%d]", id);
	}
}

//****************************************************************************
// 関数名：IsPlayBGM
// 概　要：BGMが再生されているか
// 引　数：なし
// 戻り値：なし
// 詳　細：指定したIDのBGMが再生されているか
//****************************************************************************
bool CSoundManager::IsPlayBGM(unsigned int id)
{
	bool result = false;
	if(id == m_bgmList[id].id){
		// true：再生中、false：再生していない
		result = (CheckSoundMem(m_bgmList[id].hdl) == 1) ? true : false;
	}else{
		tnl::DebugTrace("Err：IsPlayBGM ID : [%d]", id);
	}
	return result;
}

//****************************************************************************
// 関数名：StopBGM
// 概　要：BGMを停止する
// 引　数：なし
// 戻り値：なし
// 詳　細：指定したIDのBGMを停止させる
//****************************************************************************
void CSoundManager::StopBGM(unsigned int id)
{
	if(id == m_bgmList[id].id){
		StopSoundMem(m_bgmList[id].hdl);
	}else{
		tnl::DebugTrace("Err：StopBGM ID : [%d]", id);
	}
}

//****************************************************************************
// 関数名：AllStopSound
// 概　要：全てのサウンドを停止する
// 引　数：なし
// 戻り値：なし
// 詳　細：全ての音楽を停止させる
//****************************************************************************
void CSoundManager::AllStopSound()
{
	for(int id = 0; id < m_seList.size(); id++){
		if(CheckSoundMem(m_seList[id].hdl) == 1){
			StopSoundMem(m_seList[id].hdl);
		}else if(CheckSoundMem(m_seList[id].hdl) < 0){
			tnl::DebugTrace("Err：SE ID : [%d]", id);
		}else{
			// 特に処理なし
		}
	}
	for(int id = 0; id < m_bgmList.size(); id++){
		if(CheckSoundMem(m_bgmList[id].hdl) == 1){
			StopSoundMem(m_bgmList[id].hdl);
		}else if(CheckSoundMem(m_bgmList[id].hdl) < 0){
			tnl::DebugTrace("Err：BGM ID : [%d]", id);
		}else{
			// 特に処理なし
		}
	}
}

} // namespace Sound