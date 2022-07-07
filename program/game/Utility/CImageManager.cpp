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
	// ���ɓo�^�ς��m�F
	for(auto& image : images){
		if(_tcscmp(image.name, name) == 0){
			image.counter++;
			return image.imageID;
		}
	}

	// �V�K�摜��o�^
	IMAGE_DATA data;

	_tcsncpy_s(data.name, name, MAX_PATH - 1);
	data.imageID = LoadGraph(name);
	data.counter = 1;

	images.push_back(data);

	return data.imageID;
}

void DeleteGraphEx(int id)
{
	// ID�������l�Ȃ甲����
	if(id < 0){ return; }

	// �w�肳�ꂽID�̌���
	for(auto iter = images.begin(); iter != images.end(); ++iter){
		auto& image = iter;
		if(image->imageID != id){ continue; }

		// �q�b�g�����ꍇ���������J������Vector������폜����
		if(image->counter-- <= 0){
			DeleteGraph(image->imageID);
			images.erase(iter);
			break;
		}
	}
}

}