#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

//プロトタイプ宣言
void InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void PrintDebugProc(const char *fmt, ...);
#endif