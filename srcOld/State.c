#include "StarshipNavigator.h"

typedef struct
{
	void* Prev;
	void* Next;
	u64 Size;
} BlockHeader;

void* State_Init(u64 _Size)
{
	u64 Size = _Size;

	if ((Size < sizeof(SN_AppState) + DEF_ALLOCSIZE) ||
		((Size % 8) != 0))
	{
		if (Size < sizeof(SN_AppState) + DEF_ALLOCSIZE)
		{
			Size = sizeof(SN_AppState) + DEF_ALLOCSIZE;
		}

		while ((Size % 8) != 0)
		{
			Size++;
		}
	}

	void* Result = SDL_malloc(Size);

	if (!Result)
	{
		return 0;
	}

	else
	{
		SN_AppState* StateResult = (SN_AppState*)Result;
		SDL_memset(StateResult, 0, sizeof(SN_AppState));

		StateResult->MemSize = Size;
		StateResult->Data = (void*)((u64)StateResult + (u64)sizeof(SN_AppState));
		StateResult->DataEnd = (void*)((u64)StateResult + Size - 1);
		StateResult->UsedMem = 0;

		BlockHeader* First = (BlockHeader*)StateResult->Data;
		First->Prev = 0;
		First->Next = 0;
		First->Size = 0;
	}

	return Result;
}

void State_Quit(void)
{
	if (State)
	{
		if (State->PrefPath) SDL_free(State->PrefPath);
		if (State->BasePath) SDL_free(State->BasePath);
		SDL_free(State);
	}
}

void* State_malloc(u64 _Size)
{
	if (!State || !_Size) return 0;

	BlockHeader* Block = (BlockHeader*)State->Data;

	while ((u64)Block + (u64)sizeof(BlockHeader) + _Size < (u64)State->DataEnd)
	{
		if (Block->Next == 0)
		{
			Block->Next = (void*)((u64)Block + (u64)sizeof(BlockHeader) + _Size);
			BlockHeader* Next = (BlockHeader*)Block->Next;
			Next->Next = 0;
			Next->Prev = (void*)Block;
			Next->Size = 0;

			State->UsedMem += (u64)Block + (u64)sizeof(BlockHeader) + _Size;
			SN_Log(LOGLEVEL_DEBUG, "STATE: Allocated MemBlock (0x%x) : Remaining (%u bytes)",
				   (void*)((u64)Block + (u64)sizeof(BlockHeader)), State->MemSize - State->UsedMem);
			return (void*)((u64)Block + (u64)sizeof(BlockHeader));
		}

		else
		{
			if ((u64)Block + (u64)sizeof(BlockHeader) + Block->Size < (u64)Block->Next)
			{
				if ((u64)Block->Next - ((u64)Block + (u64)sizeof(BlockHeader) + Block->Size)
					>= (u64)sizeof(BlockHeader) + _Size)
				{
					// we can squeeze in here
					BlockHeader* Prev = (BlockHeader*)Block;
					BlockHeader* Next = (BlockHeader*)Block->Next;
					Block = (BlockHeader*)Block->Next;
					Next->Prev = (void*)((u64)Prev + (u64)sizeof(BlockHeader) + Prev->Size);
					Prev->Next = (void*)((u64)Prev + (u64)sizeof(BlockHeader) + Prev->Size);
					Block->Prev = (void*)Prev;
					Block->Next = (void*)Next;
					Block->Size = _Size;

					State->UsedMem += (u64)Block + (u64)sizeof(BlockHeader) + _Size;
					SN_Log(LOGLEVEL_DEBUG, "STATE: Allocated MemBlock (0x%x) : Remaining (%u bytes)",
						   (void*)((u64)Block + (u64)sizeof(BlockHeader)), State->MemSize - State->UsedMem);
					return (void*)((u64)Block + (u64)sizeof(BlockHeader));
				}
			}

			Block = Block->Next;
		}
	}

	SN_Log(LOGLEVEL_FATAL, "STATE: Out of memory!");
	return 0;
}

void* State_calloc(u64 _Count, u64 _Size)
{
	if (!_Count || !_Size) return 0;

	void* Result = State_malloc(_Size * _Count);
	
	if (Result)
	{
		SDL_memset(Result, 0, _Size * _Count);
	}

	return Result;
}

void* State_realloc(void* _Ptr, u64 _Size)
{
	if (!State) return 0;
	if (!_Ptr && !_Size) return 0;
	void* Result = 0;

	// If ptr is 0, realloc behaves like malloc, returns new ptr
	if (!_Ptr)
	{
		return State_malloc(_Size);
	}

	// if size is 0, realloc behaves like free, returns 0 ptr
	if (!_Size)
	{
		State_free(_Ptr);
		return 0;
	}

	//TODO: maybe need more bounds checks here before reads/writes happen
	BlockHeader* Block = (BlockHeader*)((u64)_Ptr - (u64)sizeof(BlockHeader));
	BlockHeader* Next = (BlockHeader*)Block->Next;
	u64 OldSize = Block->Size;
	
	if (Block->Size < _Size) // requested increase in size
	{
		if (!Next || (u64)_Ptr + _Size < (u64)Next) // is there space to increase current block
		{
			if ((u64)_Ptr + _Size >= (u64)State->DataEnd)
			{
				SN_Log(LOGLEVEL_FATAL, "STATE: Out of memory!");
				return 0;
			}

			Block->Size = _Size;
			//TODO: do we want to set new bytes to 0 here?
			// set remaining bytes to 0
			//for (char* c = (void*)((u64)Result + OldSize); c < (char*)((u64)Result + _Size); c++) *c = 0;
		}

		else if((u64)_Ptr + _Size >= (u64)Next) // move to new alloc
		{
			Result = State_malloc(_Size);
			// copy old mem to new location
			SDL_memcpy(Result, _Ptr, OldSize);
			// set remaining bytes to 0
			for (char* c = (void*)((u64)Result + OldSize); c < (char*)((u64)Result + _Size); c++) *c = 0;
			State_free(_Ptr);
		}

		else // shouldn't get here, right?
		{
			SN_Log(LOGLEVEL_FATAL, "STATE: ReAlloc Error!");
			return 0;
		}
	}

	else // requested decrease in size (or same size)
	{
		Block->Size = _Size;
	}

	SN_Log(LOGLEVEL_DEBUG, "STATE: Reallocated MemBlock (0x%x, %u bytes) to (0x%x, %u bytes)",
		   _Ptr, OldSize, Result, _Size);

	return Result;
}

void State_free(void* _Ptr)
{
	if (!State || !_Ptr) return;

	if (_Ptr < State->Data || _Ptr >= State->DataEnd)
	{
		SN_Log(LOGLEVEL_ERROR, "STATE: State_free Invalid ptr (0x%x)", _Ptr);
		return;
	}

	BlockHeader* Block = (BlockHeader*)((u64)_Ptr - (u64)sizeof(BlockHeader));
	BlockHeader* Next = (BlockHeader*)Block->Next;
	BlockHeader* Prev = (BlockHeader*)Block->Prev;

	if (Prev) Prev->Next = (void*)Next;
	if (Next) Next->Prev = (void*)Prev;

	State->UsedMem -= (u64)sizeof(BlockHeader) + Block->Size;

	SN_Log(LOGLEVEL_DEBUG, "STATE: Freed memory block (0x%x) : Size (%u bytes) : Remaining (%u bytes)",
		   _Ptr, Block->Size, State->MemSize - State->UsedMem);
	Block->Size = 0;
}