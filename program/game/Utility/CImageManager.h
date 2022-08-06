#ifndef UTILITY_CIMAGEMANAGER_H_
#define UTILITY_CIMAGEMANAGER_H_

#include <vector>
#include "DxLib.h"

namespace Utility
{
class CImageManager
{
public:
	int LoadGraphEx(const TCHAR* name);
	void DeleteGraphEx(int id);

	static void CreateInstance();
	static void DestroyInstance();
	static CImageManager* GetInstance();

private:
	static CImageManager* s_instance;

	struct IMAGE_DATA {
		TCHAR name[MAX_PATH];
		int imageID;
		int counter;
	};
	std::vector<IMAGE_DATA> images;
};

} // namespace Utility

#endif // #ifndef UTILITY_CIMAGEMANAGER_H_