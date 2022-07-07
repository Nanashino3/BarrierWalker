//*************************************************************
// �O������UI�N���X
//*************************************************************
#include "CUiTrilemma.h"

#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace{
const std::vector<std::string> g_images = {
	"resource/sticon1b-3.png",	// ��
	"resource/sticon1d-3.png",	// ��
	"resource/sticon1c-3.png"	// ��
};

std::vector<int> g_gfxHdl(MAX_ATTRIBUTE_TYPE);
const float EX_RATE = 0.75f;

// �摜�Ǎ��p�萔
const int MAX_ICON_NUM = 270;
const int X_NUM = 30;
const int Y_NUM = 9;
const int RECT_SIZE = SIZE_32;
int g_iconGfxHdl[MAX_ICON_NUM] = {0};

const int ARROW_LEFT		= 187;
const int ARROW_RIGHT_UP	= 196;
const int ARROW_RIGHT_DOWN	= 202;

}

namespace Ui
{
CUiTrilemma::CUiTrilemma()
{
	// �O�����݉摜�o�^
	for(int i = 0; i < g_images.size(); i++){
		g_gfxHdl[i] = Utility::LoadGraphEx(g_images[i].c_str());
	}
	LoadDivGraph("resource/ui/pipo-emotion.png", MAX_ICON_NUM, X_NUM, Y_NUM, RECT_SIZE, RECT_SIZE, g_iconGfxHdl);
}

CUiTrilemma::~CUiTrilemma()
{
	for(int i = 0; i < g_images.size(); i++){
		Utility::DeleteGraphEx(g_gfxHdl[i]);
	}
	for(int i = 0; i < MAX_ICON_NUM; i++){
		DeleteGraph(g_iconGfxHdl[i]);
	}
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�O������UI�`��
//****************************************************************************
void CUiTrilemma::Draw(ShareInfo::CDocGameInfo&)
{
	DrawRotaGraph(350, 120, EX_RATE, 0, g_gfxHdl[ATTRIBUTE_TYPE_RED],   true); // ��
	DrawRotaGraph(380,  80, EX_RATE, 0, g_gfxHdl[ATTRIBUTE_TYPE_GREEN], true); // ��
	DrawRotaGraph(410, 120, EX_RATE, 0, g_gfxHdl[ATTRIBUTE_TYPE_BLUE],  true); // ��

	DrawRotaGraph(380, 120, EX_RATE, 0, g_iconGfxHdl[ARROW_LEFT], true);
	DrawRotaGraph(360, 100, EX_RATE, 0, g_iconGfxHdl[ARROW_RIGHT_UP], true);
	DrawRotaGraph(395,  95, EX_RATE, 0, g_iconGfxHdl[ARROW_RIGHT_DOWN], true);
}

} // namespace Ui