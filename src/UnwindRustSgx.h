//===--------------------- UnwindRustSgx.h ----------------------------------===//
//
////                     The LLVM Compiler Infrastructure
////
//// This file is dual licensed under the MIT and the University of Illinois Open
//// Source Licenses. See LICENSE.TXT for details.
////
////
////===----------------------------------------------------------------------===//

#if !defined(UNWIND_RUST_SGX_H)
#define UNWIND_RUST_SGX_H

#ifdef RUST_SGX

#define _GNU_SOURCE
#include <link.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

// We have to use RWLock from rust repo, it is defined in:
// src/libstd/sys/sgx/rwlock.rs.
// rwlock.rs has compile time check to ensure sizeof(RWLock) = 128.
typedef struct {
    unsigned char opaque[128];
} RWLock;

// The below is obtained by printing initialized bytes
// for RWLock in rust repo: src/libstd/sys/sgx/unwind.rs.
#define RWLOCK_INIT { \
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, \
}

// These are the functions exposed by SGX-Rust.
// The rust changes are available at:
#ifdef __cplusplus
extern "C"  {
#endif
    int __rust_rwlock_rdlock(RWLock *rwlock);
    int __rust_rwlock_wrlock(RWLock *rwlock);
    int __rust_rwlock_unlock(RWLock *rwlock);
    unsigned char *__rust_alloc(size_t, size_t);
    void __rust_dealloc(unsigned char *, size_t, size_t);
    void __rust_print_err(uint8_t *m, int s);
    __attribute__((noreturn)) void __rust_abort(void);
    unsigned char *__rust_encl_address(size_t);
#ifdef __cplusplus
}
#endif

#define abort __rust_abort

#undef pthread_rwlock_t
#undef pthread_rwlock_rdlock
#undef pthread_rwlock_wrlock
#undef pthread_rwlock_unlock

#define pthread_rwlock_t RWLock
#define PTHREAD_RWLOCK_INITIALIZER RWLOCK_INIT


#define pthread_rwlock_rdlock __rust_rwlock_rdlock
#define pthread_rwlock_wrlock __rust_rwlock_wrlock
#define pthread_rwlock_unlock __rust_rwlock_unlock


#define malloc libuw_malloc
#define free libuw_free

#ifdef dl_iterate_phdr
#undef dl_iterate_phdr
#endif
#define dl_iterate_phdr libuw_dl_iterate_phdr

#ifdef __cplusplus
extern "C" {
#endif

void *libuw_malloc(size_t size);

void libuw_free(void *p);

int
libuw_dl_iterate_phdr (int (*callback) (struct dl_phdr_info *,
                  size_t, void *), void *data);

#ifdef __cplusplus
}
#endif

#endif
#endif
