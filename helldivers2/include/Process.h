/*
    These originally are from MinHook.
*/

#pragma once
#include <Windows.h>
#include <TlHelp32.h>

typedef struct _FROZEN_THREADS
{
    LPDWORD pItems;         // Data heap
    UINT    capacity;       // Size of allocated data heap, items
    UINT    size;           // Actual number of data items
} FROZEN_THREADS, * PFROZEN_THREADS;

class Process
{
public:
    static HANDLE g_hHeap;

    static BOOL EnumerateThreads(PFROZEN_THREADS pThreads)
    {
        LPDWORD p;
        BOOL succeeded = FALSE;

        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnapshot != INVALID_HANDLE_VALUE)
        {
            THREADENTRY32 te;
            te.dwSize = sizeof(THREADENTRY32);
            if (Thread32First(hSnapshot, &te))
            {
                succeeded = TRUE;
                do
                {
                    if (te.dwSize >= (FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(DWORD))
                        && te.th32OwnerProcessID == GetCurrentProcessId()
                        && te.th32ThreadID != GetCurrentThreadId())
                    {
                        if (pThreads->pItems == NULL)
                        {
                            pThreads->capacity = 128;
                            pThreads->pItems
                                = (LPDWORD)HeapAlloc(g_hHeap, 0, pThreads->capacity * sizeof(DWORD));
                            if (pThreads->pItems == NULL)
                            {
                                succeeded = FALSE;
                                break;
                            }
                        }
                        else if (pThreads->size >= pThreads->capacity)
                        {
                            pThreads->capacity *= 2;
                            p = (LPDWORD)HeapReAlloc(g_hHeap, 0, pThreads->pItems, pThreads->capacity * sizeof(DWORD));
                            if (p == NULL)
                            {
                                succeeded = FALSE;
                                break;
                            }

                            pThreads->pItems = p;
                        }
                        pThreads->pItems[pThreads->size++] = te.th32ThreadID;
                    }

                    te.dwSize = sizeof(THREADENTRY32);
                } while (Thread32Next(hSnapshot, &te));

                if (succeeded && GetLastError() != ERROR_NO_MORE_FILES)
                    succeeded = FALSE;

                if (!succeeded && pThreads->pItems != NULL)
                {
                    HeapFree(g_hHeap, 0, pThreads->pItems);
                    pThreads->pItems = NULL;
                }
            }
            CloseHandle(hSnapshot);
        }

        return succeeded;
    }

    static void Suspend(PFROZEN_THREADS pThreads)
    {
        pThreads->pItems = NULL;
        pThreads->capacity = 0;
        pThreads->size = 0;
        if (!EnumerateThreads(pThreads))
        {
            return;
        }
        else if (pThreads->pItems != NULL)
        {
            UINT i;
            for (i = 0; i < pThreads->size; ++i)
            {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION | THREAD_SET_CONTEXT, FALSE, pThreads->pItems[i]);
                if (hThread != NULL)
                {
                    SuspendThread(hThread);
                    CloseHandle(hThread);
                }
            }
        }
    }

    static void Resume(PFROZEN_THREADS pThreads)
    {
        if (pThreads->pItems != NULL)
        {
            UINT i;
            for (i = 0; i < pThreads->size; ++i)
            {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION | THREAD_SET_CONTEXT, FALSE, pThreads->pItems[i]);
                if (hThread != NULL)
                {
                    ResumeThread(hThread);
                    CloseHandle(hThread);
                }
            }

            HeapFree(g_hHeap, 0, pThreads->pItems);
        }
    }
};