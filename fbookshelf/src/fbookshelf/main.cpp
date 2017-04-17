

#include <ZLibrary.h>

#include "FBookshelf.h"

int main(int argc, char **argv) {
    if (!ZLibrary::init(argc, argv)) {
        return 1;
    }
    ZLibrary::run(new Fbookshelf(argc == 1 ? std::string() : argv[1]));
    ZLibrary::shutdown();

    return 0;
}

