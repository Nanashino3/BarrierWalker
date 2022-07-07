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
	// Šù‚É“o˜^Ï‚©Šm”F
	for(auto& image : images){
		if(_tcscmp(image.name, name) == 0){
			image.counter++;
			return image.imageID;
		}
	}

	// V‹K‰æ‘œ‚ğ“o˜^
	IMAGE_DATA data;

	_tcsncpy_s(data.name, name, MAX_PATH - 1);
	data.imageID = LoadGraph(name);
	data.counter = 1;

	images.push_back(data);

	return data.imageID;
}

void DeleteGraphEx(int id)
{
	// ID‚ª–³Œø’l‚È‚ç”²‚¯‚é
	if(id < 0){ return; }

	// w’è‚³‚ê‚½ID‚ÌŒŸõ
	for(auto iter = images.begin(); iter != images.end(); ++iter){
		auto& image = iter;
		if(image->imageID != id){ continue; }

		// ƒqƒbƒg‚µ‚½ê‡ƒƒ‚ƒŠ‚ğŠJ•ú‚µ‚ÄVector‚©‚ç‚àíœ‚·‚é
		if(image->counter-- <= 0){
			DeleteGraph(image->imageID);
			images.erase(iter);
			break;
		}
	}
}

}