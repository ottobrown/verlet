#pragma once

#include <stdint.h>
#include <stdlib.h>

struct _da_impl {
    size_t elem_size;
    size_t n_elems;
    size_t n_allocated;
};

void _da_init_impl(struct _da_impl* impl, size_t size, uintptr_t data_ptr);
void _da_add_impl(struct _da_impl* impl, uintptr_t data_ptr, void* new_elem);

#define DynArray(T) struct { struct _da_impl impl; T* data; }
#define da_init(da, T) _da_init_impl(&da.impl, sizeof(T), (uintptr_t) &da.data);
#define da_add(da, new_elem) _da_add_impl(&da.impl, (uintptr_t) &da.data, &new_elem)
#define da_free(da) free(da.data)
#define da_len(da) da.impl.n_elems
