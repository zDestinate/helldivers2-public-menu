#pragma once

#include <windows.h>
#include <iostream>
#include <string>
#include <Psapi.h>
#include <vector>

enum MemoryType
{
	MEMORY_COPY,
	MEMORY_ALLOCATE,
	MEMORY_MANUAL
};

class Memory
{
public:
	static uintptr_t FindPattern64(const char* moduleName, const char* pattern)
	{
		// Get the base address of the module
		HMODULE moduleHandle = GetModuleHandleA(moduleName);
		if (!moduleHandle) {
			return 0;
		}

		MODULEINFO moduleInfo;
		GetModuleInformation(GetCurrentProcess(), moduleHandle, &moduleInfo, sizeof(moduleInfo));
		uintptr_t moduleBase = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);

		// Convert the pattern to bytes
		std::vector<uint8_t> patternBytes;
		for (size_t i = 0; i < strlen(pattern); i += 3) {
			if (pattern[i] == '?') {
				patternBytes.push_back(0x00);  // Use a specific wildcard byte
				--i;
			}
			else {
				std::string byteString = { pattern[i], pattern[i + 1] };
				patternBytes.push_back(static_cast<uint8_t>(std::stoi(byteString, nullptr, 16)));
			}
		}

		// Search for the pattern in the module's memory
		for (uintptr_t i = 0; i < moduleInfo.SizeOfImage - patternBytes.size(); ++i) {
			bool found = true;
			for (size_t j = 0; j < patternBytes.size(); ++j) {
				if (patternBytes[j] != 0x00 && patternBytes[j] != *reinterpret_cast<uint8_t*>(moduleBase + i + j)) {
					found = false;
					break;
				}
			}

			if (found) {
				//std::cout << "Pattern found at offset: 0x" << std::hex << i << std::dec << std::endl;
				return (uintptr_t)moduleBase + i;
			}
		}

		return 0;
	}

	static uintptr_t FindString(const char* text)
	{
		HANDLE hProcess = GetCurrentProcess();

		SYSTEM_INFO si;
		GetSystemInfo(&si);

		MEMORY_BASIC_INFORMATION mBI;
		std::vector<char> buffer;
		char* CurrentMemoryPage = (char*)si.lpMinimumApplicationAddress;
		size_t len = strlen(text);

		while (CurrentMemoryPage < si.lpMaximumApplicationAddress)
		{
			if (VirtualQueryEx(hProcess, CurrentMemoryPage, &mBI, sizeof(mBI)) == 0)
			{
				break;
			}

			if (mBI.State == MEM_COMMIT)
			{
				buffer.resize(mBI.RegionSize);
				SIZE_T bytesRead;

				if (ReadProcessMemory(hProcess, CurrentMemoryPage, &buffer[0], mBI.RegionSize, &bytesRead))
				{
					for (size_t i = 0; i < (bytesRead - len); ++i)
					{
						if (memcmp(text, &buffer[i], len) == 0)
						{
							return (uintptr_t)CurrentMemoryPage + i;
						}
					}
				}
			}

			CurrentMemoryPage = static_cast<char*>(mBI.BaseAddress) + mBI.RegionSize;
		}

		return 0;
	}
	
	static BYTE* Nop(LPVOID dst, unsigned int size, bool CreateRetBytes = false)
	{
		BYTE* old = new BYTE[size];
		memcpy(old, dst, size);

		DWORD oldprotect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldprotect, &oldprotect);

		if (CreateRetBytes)
		{
			return old;
		}

		delete[] old;
		return nullptr;
	}

	template<int src_size>
	static BYTE* Patch64(LPVOID dst, const BYTE(&src)[src_size], bool CreateRetBytes = false)
	{
		BYTE* old = new BYTE[src_size];
		memcpy(old, dst, src_size);

		DWORD oldprotect;
		VirtualProtect(dst, src_size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, src_size);
		VirtualProtect(dst, src_size, oldprotect, &oldprotect);

		if (CreateRetBytes)
		{
			return old;
		}

		delete[] old;
		return nullptr;
	}

	static BYTE* Patch64(LPVOID dst, BYTE* src, unsigned int src_size, bool CreateRetBytes = false)
	{
		BYTE* old = new BYTE[src_size];
		memcpy(old, dst, src_size);

		DWORD oldprotect;
		VirtualProtect(dst, src_size, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy(dst, src, src_size);
		VirtualProtect(dst, src_size, oldprotect, &oldprotect);

		if (CreateRetBytes)
		{
			return old;
		}

		delete[] old;
		return nullptr;
	}

	template<int src_size>
	static BYTE* PatchJmp64(uintptr_t targetAddress, LPVOID jmpAddress, const BYTE (&src)[src_size], bool CreateRetBytes = false, unsigned int return_offset = 0)
	{
		//The JMP instruction bytes for 64 bits
		BYTE jmpInstruction[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		//Set jmp address
		ULONG_PTR relativeOffset = reinterpret_cast<ULONG_PTR>(jmpAddress);
		memcpy(&jmpInstruction[6], &relativeOffset, sizeof(relativeOffset));

		BYTE* old = new BYTE[14];
		memcpy(old, (LPVOID)targetAddress, 14);
		

		//Patch to make it jmp to the jmp address
		Patch64((LPVOID)targetAddress, jmpInstruction);


		//Get current bytes to patch
		unsigned int nPatchByteSize = src_size + 14;
		BYTE* patchBYTES = new BYTE[nPatchByteSize];
		memcpy(patchBYTES, &src, src_size);

		//BYTE to patch with return address;
		BYTE retJmpInstruction[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		ULONG_PTR offsetAddress = targetAddress + 14 + return_offset;
		memcpy(&retJmpInstruction[6], &offsetAddress, sizeof(offsetAddress));

		//Copy jmp address
		memcpy(patchBYTES + src_size, &retJmpInstruction, sizeof(retJmpInstruction));
		
		//Patch the bytes from jmp memory
		Patch64(jmpAddress, patchBYTES, nPatchByteSize);

		if (CreateRetBytes)
		{
			return old;
		}
		
		delete[] old;
		return nullptr;
	}

	static LPVOID AllocateMemory64(uintptr_t targetAddress, size_t size)
	{
		return VirtualAlloc(nullptr, size + 14, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	}

	template <typename T, size_t N>
	static constexpr size_t ArrayLength(const T(&)[N])
	{
		return N;
	}

	static void CreateJmpFar(uintptr_t targetAddress, uintptr_t jmpAddress)
	{
		BYTE jmpAddressBytes[8] = { 0x00 };
		uintptr_t ulJmpAddress = jmpAddress;
		memcpy(jmpAddressBytes, &ulJmpAddress, sizeof(ulJmpAddress));

		BYTE jmpInstruction[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 
			jmpAddressBytes[0], jmpAddressBytes[1], jmpAddressBytes[2], jmpAddressBytes[3], jmpAddressBytes[4], jmpAddressBytes[5], jmpAddressBytes[6], jmpAddressBytes[7] };

		DWORD oldprotect;
		VirtualProtect((LPVOID)targetAddress, 14, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy((LPVOID)targetAddress, &jmpInstruction[0], 14);
		VirtualProtect((LPVOID)targetAddress, 14, oldprotect, &oldprotect);

		FlushInstructionCache(GetCurrentProcess(), (LPVOID)targetAddress, 14);
	}

	static LPVOID CreateTrampoline(uintptr_t targetAddress, MemoryType type, unsigned int return_offset = 0, int size = 0)
	{
		const BYTE jmpInstruction[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		int nTotalSize = 0;
		if (type == MemoryType::MEMORY_COPY)
		{
			nTotalSize = 14 + return_offset + 14;
		}
		else if (type == MemoryType::MEMORY_ALLOCATE)
		{
			nTotalSize = 2 + size + 14 + 14 + return_offset + 14;
		}
		else
		{
			nTotalSize = size + 14;
		}

		LPVOID AllocAddress = VirtualAlloc(nullptr, nTotalSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if(!AllocAddress) return nullptr;
		uintptr_t retAddress = targetAddress + 14 + return_offset;

		//Fill it up with empty NOP instruction
		for(int i = 0; i < nTotalSize; i++)
		{
			*(char*)((uintptr_t)AllocAddress + i) = 0x90;
		}

		if (type == MemoryType::MEMORY_COPY)
		{
			//Copy original bytes
			memcpy(AllocAddress, (LPVOID)targetAddress, 14 + return_offset);
		}
		else if (type == MemoryType::MEMORY_ALLOCATE)
		{
			*(char*)AllocAddress = 0xEB;
			*(char*)((uintptr_t)AllocAddress + 1) = (uint8_t)size + 14;

			//Return back (JMP far)
			memcpy((LPVOID)((uintptr_t)AllocAddress + 2 + size), jmpInstruction, 14);
			memcpy((LPVOID)((uintptr_t)AllocAddress + 2 + size + 6), &retAddress, 8);

			//Copy original bytes
			memcpy((LPVOID)((uintptr_t)AllocAddress + 2 + size + 14), (LPVOID)targetAddress, 14 + return_offset);
		}

		//Return back (JMP far)
		memcpy((LPVOID)((uintptr_t)AllocAddress + nTotalSize - 14), jmpInstruction, 14);
		memcpy((LPVOID)((uintptr_t)AllocAddress + nTotalSize - 8), &retAddress, 8);

		//Create jmp far from the original to the alloc memory
		if (type != MemoryType::MEMORY_MANUAL)
		{
			CreateJmpFar(targetAddress, (uintptr_t)AllocAddress);
		}

		return AllocAddress;
	}
};