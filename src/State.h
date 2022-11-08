
#ifndef _SN_APPSTATE_H_
#define _SN_APPSTATE_H_

// Allocation macros
#define SN_Alloc(TYPE, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), 0)
//#define SN_AllocAlign(TYPE, NAME, ALIGN) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), ALIGN)
#define SN_AllocAlign(TYPE, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE), 8)
#define SN_AllocArray(TYPE, COUNT, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, 0)
//#define SN_AllocArrayAlign(TYPE, COUNT, NAME, ALIGN) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, ALIGN)
#define SN_AllocArrayAlign(TYPE, COUNT, NAME) TYPE* NAME = (TYPE*)State_Alloc(sizeof(TYPE) * COUNT, 8)

#define SN_ReAlloc(PTR, NEWSIZE) State_ReAlloc(PTR, NEWSIZE, 0)
#define SN_ReAllocAlign(PTR, NEWSIZE, ALIGN) State_ReAlloc(PTR, NEWSIZE, ALIGN)
#define SN_ReAllocAutoAlign(PTR, NEWSIZE) State_ReAlloc(PTR, NEWSIZE, NEWSIZE)

#define SN_Free(PTR) State_Free(PTR)

// malloc/realloc/free syntax
#define SN_malloc(SIZE) State_Alloc(SIZE, 8)
#define SN_realloc(PTR, NEWSIZE) State_ReAlloc(PTR, NEWSIZE, 8)
#define SN_free(PTR) State_Free(PTR)

typedef struct
{
	u64 MemSize; // total memsize
	u64 UsedMem; // currently allocated mem

	void* Data; // byte where user data starts
	void* DataEnd; // last byte of usable memory (do not access beyond this byte)

	SDL_version AppVersion;
	SDL_version SDLVersion;
	SDL_version MixVersion;
	SDL_version NetVersion;
	SDL_version GLVersion;

	char* PrefPath;
	char* BasePath;
	int ArgC;
	char** ArgV;

	SN_S_Log Log;
	SN_Timer Timer;
	SN_Input Input;
	SN_Window Window;
	//SN_Audio Audio;

	bool Running;
} SN_AppState;

// Pass 0 for default (sizeof SN_AppState + DEF_ALLOCSIZE)
void* State_Init(u64 _Size);
void State_Quit(void);

void* State_Alloc(u64 _Size, u64 _Align);
void* State_ReAlloc(void* _Ptr, u64 _NewSize, u64 _Align);
void State_Free(void* _Ptr);

#endif //_SN_APPSTATE_H_