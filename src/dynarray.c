#include "dynarray.h"
#include <stdlib.h>
#include <string.h>

void _da_init_impl(struct _da_impl* impl, size_t size, uintptr_t data_ptr) {
    void ** data = (void**) data_ptr;
    impl->n_elems = 0;
    impl->elem_size = size;
    impl->n_allocated = 5;

    *data = malloc(5 * size);
}

void _da_add_impl(struct _da_impl* impl, uintptr_t data_ptr, void *new_elem) {
    void ** data = (void**) data_ptr;
    if (impl->n_elems >= impl->n_allocated) {
        impl->n_allocated = 2 * (1 + impl->n_allocated);
        *data = realloc(*data, impl->n_allocated * impl->elem_size);
    }

    memcpy(*data + impl->n_elems * impl->elem_size, new_elem, impl->elem_size);
    impl->n_elems ++;
}
