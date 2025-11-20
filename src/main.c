#include <stdio.h>
#include <stdlib.h>

#include "warehouse.h"

int main(void) {

    const int rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    const int columns = (MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2);

    int* warehouse = generate_layout(MAIN_AISLE_WIDTH, AISLE_WIDTH, SHELF_LENGTH, SHELF_AMOUNT, rows, columns);

    print_warehouse(warehouse, rows, columns);

    free(warehouse);

    return 0;
}