#ifndef _OBLECT00_H_
#define _OBLECT00_H_

//���f���̍\����
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 posOld;							//�ʒu
	D3DXVECTOR3 move;							//�ʒu
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	bool bUse;						//���f���̍ő�
}Object00;

//�v���g�^�C�v�錾
void InitObject00(void);
void UninitObject00(void);
void UpdateObject00(void);
void DrawObject00(void);
void SetObject00(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot);
Object00 * GetObject00(void);
#endif