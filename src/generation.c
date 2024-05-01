
#include "generation.h"
#include <SDL2/SDL.h>

#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "misc-no-recursion"

typedef struct Node {
    bool leaf;
    struct Node* left;
    struct Node* right;
} Node;

static Node* nodeCreate(bool leaf) {
    Node* node = SDL_malloc(sizeof *node);
    node->leaf = leaf;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

static void nodeDestroy(Node* node) {
    SDL_free(node);
}

static void treePrintLeafs(const Node* node) {
    if (node->left == nullptr && node->left == nullptr)
        SDL_Log("%c", node->leaf ? 't' : 'f');
    else {
        if (node->left != nullptr)
            treePrintLeafs(node->left);
        if (node->right != nullptr)
            treePrintLeafs(node->right);
    }
}

//static
