// userland/pku-demo.c
#include <stdio.h>
#include <cpuid.h>

int main() {
    unsigned int eax, ebx, ecx, edx;

    // CPUID leaf 7, subleaf 0 → ECX bit 3 = PKU support
    if (__get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx)) {
        if (ecx & (1 << 3)) {
            printf("[+] PKU (Protection Keys for Userspace) is supported on this CPU!\n");
        } else {
            printf("[-] PKU is NOT supported on this CPU.\n");
        }
    } else {
        printf("[-] Failed to query CPUID.\n");
    }

    return 0;
}

