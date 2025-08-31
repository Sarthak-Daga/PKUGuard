#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/*
 * Minimal PKU demo:
 * - mmap() a single page
 * - allocate a protection key with pkey_alloc()
 * - tag the page with pkey via pkey_mprotect()
 * - change PKRU rights via pkey_set() (glibc wrapper)
 * - attempt a write and observe SIGSEGV if PKU is enforced
 *
 * NOTE: compile & run this inside the QEMU guest that exposes PKU (not on your AMD host).
 */

#ifndef PKEY_DISABLE_ACCESS
#define PKEY_DISABLE_ACCESS 0x1
#endif
#ifndef PKEY_DISABLE_WRITE
#define PKEY_DISABLE_WRITE 0x2
#endif

/* these wrappers are provided by modern glibc (guest). */
extern int pkey_alloc(unsigned int flags, unsigned int rights) __attribute__((weak));
extern int pkey_free(int pkey) __attribute__((weak));
extern int pkey_mprotect(void *addr, size_t len, int prot, int pkey) __attribute__((weak));
extern int pkey_set(int pkey, unsigned int rights) __attribute__((weak));

static void sigsegv(int signum, siginfo_t *si, void *ctx) {
    (void)signum; (void)ctx;
    fprintf(stderr, "SIGSEGV caught at address %p — PKU enforcement worked (or page protection hit)\n", si->si_addr);
    _exit(0);
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = sigsegv;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);

    size_t pg = sysconf(_SC_PAGESIZE);
    void *p = mmap(NULL, pg, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    printf("Allocated page at %p (pagesize=%zu)\n", p, pg);
    strcpy((char*)p, "hello");

    if (!pkey_alloc || !pkey_mprotect || !pkey_set) {
        fprintf(stderr, "Warning: glibc pkey wrappers not available in this build environment.\n"
                        "Compile & run this inside a modern distro (in the QEMU guest) with pkey support.\n");
        printf("Sample page contents: %s\n", (char*)p);
        munmap(p, pg);
        return 0;
    }

    int key = pkey_alloc(0, 0);
    if (key == -1) {
        perror("pkey_alloc");
        fprintf(stderr, "Kernel or glibc may not support pkeys here.\n");
        munmap(p, pg);
        return 1;
    }
    printf("Allocated PKEY = %d\n", key);

    if (pkey_mprotect(p, pg, PROT_READ | PROT_WRITE, key) == -1) {
        perror("pkey_mprotect");
        pkey_free(key);
        munmap(p, pg);
        return 1;
    }
    printf("Tagged page with pkey %d (mprotect succeeded)\n", key);

    /* disable write for this pkey (if supported) */
    if (pkey_set(key, PKEY_DISABLE_WRITE) == -1) {
        perror("pkey_set");
        printf("pkey_set failed — PKU rights not modified. On a guest with PKU try again.\n");
        pkey_free(key);
        munmap(p, pg);
        return 1;
    }
    printf("pkey_set: write disabled for pkey %d — attempting write (should fault)...\n", key);

    /* Attempt a write which should fault if PKRU enforces write-prohibition */
    ((char*)p)[0] = 'X';
    /* If we get here, write succeeded -> PKU not enforced (or rights not set) */
    printf("Write succeeded — PKU did not prevent write. Check kernel/guest configuration.\n");

    pkey_set(key, 0); /* re-enable */
    pkey_free(key);
    munmap(p, pg);
    return 0;
}

