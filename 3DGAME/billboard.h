#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#define MAX_BILLBOARD (128)		//�e�̍ő吔

//�r���{�[�h�\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 posOld;							//�ʒu
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	float rot;									//����
	int nLife;									//����
	int nIndex;
	bool bUse;									//�g�p���Ă��邩�ǂ���
}Billboard;

//�v���g�^�C�v�錾
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float rot);
Billboard * GetBillboard(void);
#endif