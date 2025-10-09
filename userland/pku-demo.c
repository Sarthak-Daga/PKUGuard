#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define domain constants
#define DOMAIN_KERNEL 1
#define DOMAIN_USER   2

// Define a memory region type
typedef struct {
    void *addr;
    size_t size;
    int domain;
} MemRegion;

// Simulate a PKU-protected write
int write_to_region(MemRegion *region, int current_domain, const char *data) {
    if (current_domain != region->domain) {
        printf("[!] Illegal write detected! Domain %d cannot modify domain %d memory.\n",
               current_domain, region->domain);
        return -1;
    }

    strncpy((char *)region->addr, data, region->size - 1);
    ((char *)region->addr)[region->size - 1] = '\0'; // ensure null-termination
    printf("[+] Write successful in domain %d.\n", current_domain);
    return 0;
}

int main() {
    char kernel_data[64] = "Initial kernel data.";
    char user_data[64] = "User buffer.";

    MemRegion kernel_region = {kernel_data, sizeof(kernel_data), DOMAIN_KERNEL};
    MemRegion user_region   = {user_data, sizeof(user_data), DOMAIN_USER};

    printf("=== PKU-Guard Simulation ===\n");

    printf("[*] Kernel region before: %s\n", kernel_data);
    printf("[*] User region before: %s\n", user_data);

    // Legal write from kernel
    write_to_region(&kernel_region, DOMAIN_KERNEL, "Updated by kernel.");

    // Legal write from user
    write_to_region(&user_region, DOMAIN_USER, "Updated by user.");

    // Illegal write from user to kernel memory
    write_to_region(&kernel_region, DOMAIN_USER, "User trying to corrupt kernel data!");

    printf("[*] Kernel region after: %s\n", kernel_data);
    printf("[*] User region after: %s\n", user_data);

    return 0;
}

