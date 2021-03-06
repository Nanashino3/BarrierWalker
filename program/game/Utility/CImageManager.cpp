#include "CImageManager.h"

#include "../../dxlib_ext/dxlib_ext.h"
#include <vector>

namespace{
struct IMAGE_DATA {
	TCHAR name[MAX_PATH];
	int imageID;
	int counter;
};
std::vector<IMAGE_DATA> images;
}

namespace Utility
{
int LoadGraphEx(const TCHAR* name)
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

void DeleteGraphEx(int id)
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

}