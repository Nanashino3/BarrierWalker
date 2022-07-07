#include "CSoundManager.h"

#include "../../dxlib_ext/dxlib_ext.h"

namespace {
const std::string SOUND_CSV = "sound/sound_info.csv";
std::vector<std::vector<std::string>> g_sound_csv;

struct SOUND_INFO {
	int id;
	int kind;
	int hdl;
};
std::vector<SOUND_INFO> g_bgm, g_se;
}

namespace Sound
{
CSoundManager* CSoundManager::s_myInstance = nullptr;

void CSoundManager::CreateInstance()
{
	if(s_myInstance == nullptr){ s_myInstance = new CSoundManager(); }
}
CSoundManager* CSoundManager::GetInstance()
{
	if(s_myInstance == nullptr){ CreateInstance(); }
	return s_myInstance;
}

void CSoundManager::DestroyInstance()
{
	delete s_myInstance;
}

CSoundManager::CSoundManager()
{
	g_sound_csv = tnl::LoadCsv(SOUND_CSV);
	for(int i = 0; i < g_sound_csv.size(); i++){
		SOUND_INFO info;
		info.id = atoi(g_sound_csv[i][0].c_str());
		info.kind = atoi(g_sound_csv[i][1].c_str());
		info.hdl = LoadSoundMem(g_sound_csv[i][2].c_str());

		switch(info.kind)
		{
		case 0:	// BGM
			g_bgm.push_back(info);
			break;
		case 1:	// SE
			g_se.push_back(info);
			break;
		default:
			tnl::DebugTrace("Err：CSoundManager KIND : [%d]", info.kind);
			break;
		}
	}
}

//********************
// SE関連処理
//********************
void CSoundManager::PlaySE(unsigned int id)
{
	if(id == g_se[id].id){
		PlaySoundMem(g_se[id].hdl, DX_PLAYTYPE_BACK);
	}else{
		tnl::DebugTrace("Err：PlaySE ID : [%d]", id);
	}
}
bool CSoundManager::IsPlaySE(unsigned int id)
{
	bool result = false;
	if(id == g_se[id].id){
		// true：再生中、false：再生していない
		result = (CheckSoundMem(g_se[id].hdl) == 1) ? true : false;
	}else{
		tnl::DebugTrace("Err：IsPlaySE ID : [%d]", id);
	}
	return result;
}
void CSoundManager::StopSE(unsigned int id)
{
	if(id == g_se[id].id){
		StopSoundMem(g_se[id].hdl);
	}else{
		tnl::DebugTrace("Err：StopSE ID : [%d]", id);
	}
}
//********************
// BGM関連処理
//********************
void CSoundManager::PlayBGM(unsigned int id)
{
	if(id == g_bgm[id].id){
		PlaySoundMem(g_bgm[id].hdl, DX_PLAYTYPE_LOOP);
	}else{
		tnl::DebugTrace("Err：PlayBGM ID : [%d]", id);
	}
}
bool CSoundManager::IsPlayBGM(unsigned int id)
{
	bool result = false;
	if(id == g_bgm[id].id){
		// true：再生中、false：再生していない
		result = (CheckSoundMem(g_bgm[id].hdl) == 1) ? true : false;
	}else{
		tnl::DebugTrace("Err：IsPlayBGM ID : [%d]", id);
	}
	return result;
}
void CSoundManager::StopBGM(unsigned int id)
{
	if(id == g_bgm[id].id){
		StopSoundMem(g_bgm[id].hdl);
	}else{
		tnl::DebugTrace("Err：StopBGM ID : [%d]", id);
	}
}

// サウンドを全て停止
void CSoundManager::AllStopSound()
{
	for(int id = 0; id < g_se.size(); id++){
		if(CheckSoundMem(g_se[id].hdl) == 1){
			StopSoundMem(g_se[id].hdl);
		}else if(CheckSoundMem(g_se[id].hdl) < 0){
			tnl::DebugTrace("Err：SE ID : [%d]", id);
		}else{
			// 特に処理なし
		}
	}
	for(int id = 0; id < g_bgm.size(); id++){
		if(CheckSoundMem(g_bgm[id].hdl) == 1){
			StopSoundMem(g_bgm[id].hdl);
		}else if(CheckSoundMem(g_bgm[id].hdl) < 0){
			tnl::DebugTrace("Err：BGM ID : [%d]", id);
		}else{
			// 特に処理なし
		}
	}
}

} // namespace Sound