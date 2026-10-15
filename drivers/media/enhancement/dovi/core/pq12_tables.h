/* Decoder bindings to the canonical read-only PQ tables. */
#ifndef DOVI_PQ12_TABLES_H
#define DOVI_PQ12_TABLES_H

#include "data_layout.h"

#define dovi_pq12_positions dovi_retained_section_3.linear_to_pq_positions
#define dovi_pq12_slopes dovi_retained_section_3.linear_to_pq_slopes
#define dovi_pq12_intercepts dovi_retained_section_3.linear_to_pq_intercepts

#endif
