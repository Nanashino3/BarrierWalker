//****************************************************************************
// �t�@�C�����FCImageManager(�摜�Ǘ���)
// ��@���@���F2022/08/06
#include "CImageManager.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace Utility
{
CImageManager* CImageManager::s_instance = nullptr;

//****************************************************************************
// �֐����FCreateInstance
// �T�@�v�F�C���X�^���X����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
void CImageManager::CreateInstance()
{
	s_instance = new CImageManager();
}

//****************************************************************************
// �֐����FDestroyInstance
// �T�@�v�F�C���X�^���X�j��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
void CImageManager::DestroyInstance()
{
	delete s_instance;
}

//****************************************************************************
// �֐����FGetInstance
// �T�@�v�F�C���X�^���X�擾
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
CImageManager* CImageManager::GetInstance()
{
	if(s_instance == nullptr){ CreateInstance(); }
	return s_instance;
}

//****************************************************************************
// �֐����FLoadGraphEx
// �T�@�v�F�摜���[�h(���b�p�[)
// ���@���F��1����	�ǂݍ��݃t�@�C��
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
int CImageManager::LoadGraphEx(const TCHAR* name)
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

//****************************************************************************
// �֐����FDeleteGraphEx
// �T�@�v�F�摜�j��(���b�p�[)
// ���@���F��1����	�j������摜ID
// �߂�l�F�Ȃ�
// �ځ@�ׁF
//****************************************************************************
void CImageManager::DeleteGraphEx(int id)
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

} // namespace Utility