//****************************************************************************
// ファイル名：CImageManager(画像管理者)
// 作　成　日：2022/08/06
#include "CImageManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace Utility
{
CImageManager* CImageManager::s_instance = nullptr;

//****************************************************************************
// 関数名：CreateInstance
// 概　要：インスタンス生成
// 引　数：なし
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CImageManager::CreateInstance()
{
	s_instance = new CImageManager();
}

//****************************************************************************
// 関数名：DestroyInstance
// 概　要：インスタンス破棄
// 引　数：なし
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CImageManager::DestroyInstance()
{
	delete s_instance;
}

//****************************************************************************
// 関数名：GetInstance
// 概　要：インスタンス取得
// 引　数：なし
// 戻り値：なし
// 詳　細：
//****************************************************************************
CImageManager* CImageManager::GetInstance()
{
	if(s_instance == nullptr){ CreateInstance(); }
	return s_instance;
}

//****************************************************************************
// 関数名：LoadGraphEx
// 概　要：画像ロード(ラッパー)
// 引　数：第1引数	読み込みファイル
// 戻り値：なし
// 詳　細：
//****************************************************************************
int CImageManager::LoadGraphEx(const TCHAR* name)
{
	// 既に登録済か確認
	for(auto& image : images){
		if(_tcscmp(image.name, name) == 0){
			image.counter++;
			return image.imageID;
		}
	}

	// 新規画像を登録
	IMAGE_DATA data;

	_tcsncpy_s(data.name, name, MAX_PATH - 1);
	data.imageID = LoadGraph(name);
	data.counter = 1;

	images.push_back(data);

	return data.imageID;
}

//****************************************************************************
// 関数名：DeleteGraphEx
// 概　要：画像破棄(ラッパー)
// 引　数：第1引数	破棄する画像ID
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CImageManager::DeleteGraphEx(int id)
{
	// IDが無効値なら抜ける
	if(id < 0){ return; }

	// 指定されたIDの検索
	for(auto iter = images.begin(); iter != images.end(); ++iter){
		auto& image = iter;
		if(image->imageID != id){ continue; }

		// ヒットした場合メモリを開放してVectorからも削除する
		if(image->counter-- <= 0){
			DeleteGraph(image->imageID);
			images.erase(iter);
			break;
		}
	}
}

} // namespace Utility