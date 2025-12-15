#include "warehouse.h"
#include <time.h>
#include "a_star.h"
#include <limits.h>

int manhat_dist(const int x1, const int y1, const int x2, const int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

double euclidean_dist(const int x1, const int y1, const int x2, const int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void* safe_malloc(size_t size) {
    // Attempt to allocate memory
    void* pointer = malloc(size);
    // Check return value for NULL
    if (pointer == NULL) {
        printf("Error: Could not allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    return pointer;
}

int file_write_items(const char* filename, int number_of_items) {
    // 10 Random colors and names
    const char* colors[] = {
        "Red", "Blue", "Green", "Yellow", "Black",
        "White", "Orange", "Purple", "Pink", "Gray"
    };

    const char* names[] = {
        "Box", "Ball", "Cube", "Bottle", "Bag",
        "Stone", "Book", "Lamp", "Pen", "Plate"
    };

    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "ERROR: Failed to open file\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < number_of_items; i++) {
        const char* color = colors[rand() % 10];
        const char* name  = names[rand() % 10];
        double weight = (rand() % 10000) / 100.0;   // 0.00–100.00
        fprintf(file, "%s %s %.2f\n", color, name, weight); // Correct format for read later.
    }
    fclose(file);
    return EXIT_SUCCESS;
}

item_t* read_items_from_file(char* file_name, int* items_read) {
    int n_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;

    file_write_items(file_name, n_shelves); // Writing the items right before reading.

    FILE* items_file = fopen(file_name, "r");
    if (items_file == NULL) {
        printf("Failed to open file.\n");
        printf("Remember to set working directory.\n");
        exit(EXIT_FAILURE);
    }

    item_t* items = safe_malloc(sizeof(item_t)*n_shelves);
    *items_read = file_read_items(items, n_shelves, items_file);
    fclose(items_file);

    return items;
}

int file_read_items(item_t* items, int n_items, FILE* file) {
    item_t item;
    int i;
    for (i = 0; i < n_items; i++) {
        int success = fscanf(file, " %s %s %lf", item.color, item.name, &item.weight);
        if(success != 3){
            printf("Failed to read enough fields for item %d, only read %d field(s).\n", i, success);
            break;
            //exit(EXIT_FAILURE);
        }
        items[i] = item;
    }
    if (i < n_items) {
        printf("Failed to generate %d items, only read %d items from file.\nSetting remainder of shelves to empty\n", n_items, i);
    }
    return i;
}

warehouse_t* create_warehouse() {

    warehouse_t* warehouse = (warehouse_t*)safe_malloc(sizeof(warehouse_t));

    warehouse->rows = SHELF_AMOUNT * (2 + AISLE_WIDTH) + AISLE_WIDTH;
    warehouse->columns = MAIN_AISLE_WIDTH * 3 + SHELF_LENGTH * 2;
    warehouse->number_of_shelves = SHELF_AMOUNT * SHELF_LENGTH * 2 * 2;
    warehouse->drop_zones = generate_drop_zones(AMOUNT_OF_DROP_ZONES);
    warehouse->items = read_items_from_file(ITEM_FILE, &warehouse->number_of_items);
    warehouse->map = generate_layout(warehouse);
    warehouse->shelves = populate_shelves(warehouse);
    warehouse->printing = true;

    set_drop_zone_cell(warehouse, warehouse->columns-1, warehouse->rows/2-1);
    set_drop_zone_cell(warehouse, warehouse->columns-1, warehouse->rows/2);

    if (BLOCK_CENTER_MAIN_AISLE) {
        block_center_aisle(warehouse);
    }

    return warehouse;
}

void set_obstacle(warehouse_t* warehouse, const int x, const int y) {
    cell_e* cell = get_cell(warehouse, x, y);
    *cell = shelf;
}

void block_center_aisle(warehouse_t* warehouse) {
    for (int i = 0; i < warehouse->rows-AISLE_WIDTH*2; i++) {
        set_obstacle(warehouse,warehouse->columns/2,1+i);
        set_obstacle(warehouse,warehouse->columns/2-1,1+i);
    }
}

void destroy_warehouse(warehouse_t* warehouse) {
    if (warehouse->items != NULL) free(warehouse->items);
    if (warehouse->map != NULL) free(warehouse->map);
    if (warehouse->shelves != NULL) free_shelves(warehouse->shelves, warehouse->number_of_shelves);
    if (warehouse->drop_zones != NULL) free(warehouse->drop_zones);
    if (warehouse != NULL) free(warehouse);
}

void destroy_picking_list(picking_list_t* picking_list) {
    if (picking_list->items != NULL) free(picking_list->items);
    if (picking_list != NULL) free(picking_list);
}

bool is_vertical_end_aisle(const warehouse_t* warehouse, int row) {
    if (row < AISLE_WIDTH
        || row >= warehouse->rows - AISLE_WIDTH){
        return true; // Top & bottom aisles
    }
    return false;
}

bool is_main_aisle(int column) {
    if (column >= MAIN_AISLE_WIDTH+SHELF_LENGTH
        && column < MAIN_AISLE_WIDTH + SHELF_LENGTH + MAIN_AISLE_WIDTH){
        return true; // Center main aisle
    }

    if (column < MAIN_AISLE_WIDTH
        || column >= MAIN_AISLE_WIDTH * 2 + SHELF_LENGTH * 2){
        return true; // Left & right side main aisles
    }
    return false;
}

cell_e* generate_layout(const warehouse_t* warehouse) {
    const int rows = warehouse->rows;
    const int columns = warehouse->columns;
    const int shelf_width = 2;

    cell_e* map = (cell_e*)safe_malloc(sizeof(cell_e)*columns*rows); // Allocate memory for the map

    int row_pattern = 0;
    for (int row = 0; row < rows; row++) {
        bool is_end_aisle = is_vertical_end_aisle(warehouse, row); // Bool per row: Is aisle is at the ends (top or bottom)

        if (!is_end_aisle) {
            row_pattern++; // Pattern inside the rows with shelves & aisles inbetween shelves
        }

        for (int col = 0; col < columns; col++) {
            int id = row * columns + col; // Array position

            if (is_end_aisle) {
                map[id] = empty; // Top & bottom aisles
                continue;
            }

            if (is_main_aisle(col)) {
                map[id] = empty; // Left, right & center main aisles
                continue;
            }

            if (row_pattern > AISLE_WIDTH + shelf_width) {
                row_pattern = 1; // Restart pattern
            }

            if (row_pattern <= shelf_width) {
                map[id] = shelf; // Pattern is within shelf width; must be shelf
            } else {
                map[id] = empty; // Outside of shelf width; must be aisle
            }
        }
    }
    return map;
}

shelf_t** populate_shelves(const warehouse_t* warehouse) {
    shelf_t** shelves = safe_malloc(sizeof(shelf_t*)*warehouse->number_of_shelves); // Allocate memory for shelves

    int shelf_count = 0;
    for (int row = 0; row < warehouse->rows; row++) {
        for (int col = 0; col < warehouse->columns; col++) {
            cell_e* cell = get_cell(warehouse, col, row); // Looping through rows & columns, we can get every cell as x,y

            if (*cell != shelf) // Continue if not a shelf
                continue;

            shelves[shelf_count] = generate_shelf(warehouse, shelf_count, STOCK_AMOUNT, col, row); // Generate shelf
            shelf_count++; // Keep track of array positions
        }
    }
    return shelves;
}

drop_zones* generate_drop_zones(int capacity) {
    drop_zones* zones = (drop_zones*)safe_malloc(sizeof(drop_zones));
    zones->amount = 0;
    zones->max_amount = capacity;
    zones->zones = (drop_zone_t**)safe_malloc(sizeof(drop_zone_t*) * capacity);

    return zones;
}

void set_drop_zone_cell(warehouse_t* warehouse, const int x, const int y) {
    cell_e* cell = get_cell(warehouse, x, y);

    if (warehouse->drop_zones->amount >= warehouse->drop_zones->max_amount) {
        printf("Maximum amount of drop zones already reached\n");
        return;
    }

    if (*cell != shelf && *cell != drop_zone) {
        *cell = drop_zone;
        drop_zone_t* drop_zone = (drop_zone_t*)safe_malloc(sizeof(drop_zone_t*));
        drop_zone->x = x;
        drop_zone->y = y;
        warehouse->drop_zones->zones[warehouse->drop_zones->amount] = drop_zone;
        warehouse->drop_zones->amount++;
        return;
    }
    printf("Cell is already occupied\n");
}

drop_zone_t* get_nearest_drop_zone(const warehouse_t* warehouse, int x, int y) {
    int nearest_distance = INT_MAX;
    drop_zone_t* nearest_zone = NULL;
    for (int i = 0; i < warehouse->drop_zones->amount; i++) {
        int dist = manhat_dist(x, y, warehouse->drop_zones->zones[i]->x, warehouse->drop_zones->zones[i]->y);
        if (dist < nearest_distance) {
            nearest_distance = dist;
            nearest_zone = warehouse->drop_zones->zones[i];
        }
    }
    if (nearest_zone != NULL)
        return nearest_zone;
    printf("Couldn't find any drop zone to return");
    return NULL;
}

void print_cell(cell_e cell) {
    switch (cell) {
        case empty:
            printf("| ");
            break;
        case shelf:
            printf("|X");
            break;
        case robot:
            printf("|R");
            break;
        case drop_zone:
            printf("|D");
            break;
    }

}

cell_e* get_cell(const warehouse_t* warehouse, int x, int y) {
    return &warehouse->map[y * warehouse->columns + x];
}

void print_warehouse(const warehouse_t* warehouse) {
    if (!warehouse->printing)
        return;

    int rows = warehouse->rows;
    int columns = warehouse->columns;

    // Print row of x-coords
    printf("\nY: X:");
    for (int x = 0; x < columns; x++) {
        printf("%d ", x % 10);
    }
    printf("\n");

    for (int y = 0; y < rows; y++) {
        printf("%d - ", y % 10);    // Prints y-coords
        for (int x = 0; x < columns; x++) {
            cell_e* cell = get_cell(warehouse, x, y);
            print_cell(*cell);
        }
        printf("|\n"); // Prints | at the end of each row and skips to new line
    }
}

struct shelf* generate_shelf(warehouse_t* warehouse, int shelf_count, int stock, int x, int y) {
    shelf_t* shelf = (shelf_t*)safe_malloc(sizeof(shelf_t));

    item_t empty_item = (item_t){0};
    char empty_name[20] = "empty";
    strcpy(empty_item.name, empty_name);

    if (shelf_count < warehouse->number_of_items) {
        shelf->item = warehouse->items[shelf_count];
    } else {
        shelf->item = empty_item;
    }

    shelf->stock = stock;
    shelf->x = x;
    shelf->y = y;
    return shelf;
}

shelf_t* search_nearest_item(int x, int y, char search_input_name[32], char search_input_color[32], const warehouse_t* warehouse) {
    int nearest_distance = INT_MAX;
    shelf_t* nearest_shelf = &(shelf_t){0};

    int n_shelves = warehouse->number_of_shelves;
    for (int i = 0; i < n_shelves; i++) {
        if (strcmp(warehouse->shelves[i]->item.color, search_input_color) == 0 && // Using string compare to find the shelf where the item is located.
            strcmp(warehouse->shelves[i]->item.name, search_input_name) == 0) {
                shelf_t* shelf = warehouse->shelves[i];
                int distance = (int)euclidean_dist(x, y, shelf->x, shelf->y);
                if (distance < nearest_distance) {
                    nearest_shelf = shelf;
                    nearest_distance = distance;
                }
        }
    }
    if (nearest_distance < INT_MAX) {
        return nearest_shelf;
    }

    printf("Failed to locate any shelf.");
    exit(EXIT_FAILURE);
}

shelf_t* manual_search_item(const warehouse_t* warehouse) {
    int n_shelves = warehouse->number_of_shelves;

    char search_input_color[20];
    char search_input_name[20];

    printf("\nWrite your search input>");
    scanf(" %6s %9s", search_input_color, search_input_name); //input from user, assigns the string values to search_input_color and search_input_name


    for (int i = 0; i < n_shelves; i++) {
        if (strcmp(warehouse->shelves[i]->item.color, search_input_color) == 0 && // String compare to find the correct shelf that holds the searched item.
            strcmp(warehouse->shelves[i]->item.name, search_input_name) == 0) {
            return warehouse->shelves[i];
            }
    }
    printf("Failed to locate any shelf, exiting.");
    exit(EXIT_FAILURE);
}

shelf_t* find_nearest_item(int x, int y, const warehouse_t* warehouse, picking_list_t* picking_list) {
    int nearest_distance = INT_MAX;
    shelf_t* nearest_shelf = &(shelf_t){0};

    for (int i = 0; i < picking_list->amount; i++) {
        if (picking_list->items[i].weight <= 0 && !strlen(picking_list->items[i].name) && !strlen(picking_list->items[i].color))
            continue;
        shelf_t* shelf = search_nearest_item(x, y, picking_list->items[i].name, picking_list->items[i].color, warehouse);
        int distance = (int)euclidean_dist(x, y, shelf->x, shelf->y);
        if (distance < nearest_distance) {
            nearest_shelf = shelf;
            nearest_distance = distance;
        }
    }

    if (nearest_distance < INT_MAX) {
        return nearest_shelf;
    }
    printf("Failed to locate any shelf, exiting.");
    exit(EXIT_FAILURE);
}

picking_list_t* generate_picking_list(const warehouse_t* warehouse, const int item_amount_input) {
    picking_list_t* picking_list = (picking_list_t*)safe_malloc(sizeof(picking_list_t));
    picking_list->amount = item_amount_input;
    item_t* items = safe_malloc(sizeof(item_t)*item_amount_input);
    picking_list->items = items;

    for (int i = 0; i < item_amount_input; i++) {
        int random_number = rand() % warehouse->number_of_items; // Randomness depending on the given seed.
        picking_list->items[i] = warehouse->items[random_number]; // Copying the whole struct
    }

    return picking_list;
}

void display_picking_list(picking_list_t* picking_list, int item_amount_input) {
    // Display the picking list in order
    printf("The picking list is: ");
    for (int i = 0; i < item_amount_input; i++) {
        printf("%s %s %.2lf ", picking_list->items[i].color, picking_list->items[i].name, picking_list->items[i].weight);
    }
    printf("\n \n");
}

void remove_item(picking_list_t* picking_list, item_t item) {
    for (int i = 0; i < picking_list->amount; i++) {
        if (picking_list->items[i].weight <= 0 && !strlen(picking_list->items[i].name) && !strlen(picking_list->items[i].color))
            continue;
        if (strcmp(picking_list->items[i].color,item.color) == 0 && strcmp(picking_list->items[i].name,item.name) == 0) {
            picking_list->items[i] = (item_t){0};
            return;
        }
    }
}

void free_shelves(shelf_t** shelves, const int n_shelves){
    for (int i = 0; i < n_shelves; i++) {
        free(shelves[i]);
    }
}