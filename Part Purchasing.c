#include <stdio.h>
#include <stdlib.h>

typedef struct Part {
    int count; // The number of this part available
    int extra_part; // The additional part that comes when purchasing
    int price; // The price of purchasing the part
} Part;

typedef struct Component {
    int known; // Is the component known to the repair crew
    int part_req; // The part required to fix
    int num_comp_rev; // Number of components revealed when fixed
    int * comp_rev; // Array of revealed components when fixed
} Component;

int main() {
    // Get number of parts
    int numParts;
    scanf("%d", &numParts);

    // Create an array for number of parts
    Part * parts = (Part *)malloc(numParts * sizeof(Part));

    // Handle input of parts
    for(int i = 0; i < numParts; i++) {
        scanf("%d %d", &parts[i].price, &parts[i].extra_part);
        parts[i].count = 0;
    }

    // Get number of components
    int numComponents;
    scanf("%d", &numComponents);

    // Create an array for number of components
    Component * components = (Component *)malloc(numComponents * sizeof(Component));

    // Handle input for components
    for(int i = 0; i < numComponents; i++) {
        scanf("%d %d", &components[i].part_req, &components[i].num_comp_rev);

        // Get number of components revealed
        components[i].comp_rev = (int *)malloc(numComponents * sizeof(int));

        // Store the components revealed
        for(int j = 0; j < components[i].num_comp_rev; j++)
            scanf("%d", &components[i].comp_rev[j]);
    }




    free(components->comp_rev);
    free(components);
    free(parts);

    return 0;
}