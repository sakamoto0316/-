#ifndef _WALL_H_
#define _WALL_H_

#define MAX_WALL (1)	//�e�̍ő吔
#define MAX_WIDTH (50.0f)	//�e�̉���
#define MAX_HEIGHT (100.0f)	//�e�̏c��

//�Ǎ\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
}Wall;

//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 min, D3DXVECTOR3 max);
void CollisionBillboard(Wall*pWall);
Wall * GetWall(void);
#endif