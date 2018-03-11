#include "genann.h"
#include "debug.h"

int main() {
    cout << "== Squareworld Q-Learning ==" << endl;
    
    genann* ann = genann_init(2, 1, 3, 2);
    
    
    
    genann_free(ann);
    return 0;
}