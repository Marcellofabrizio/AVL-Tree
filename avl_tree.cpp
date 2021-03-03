#include <iostream>
using namespace std;

class Node  {
    public:
        int value;
        int height;
        Node* left;
        Node* right;
    
    private:
        void insertNode(int key, Node* node);
    
};

Node* getLowestNode(Node *node);

int max(int a, int b) {
    return (a > b) ? a : b;
}


int height(Node* Node) {
    if(Node == NULL) {
        return 0;
    }

    else {
        int LHeight = height(Node->left);
        int RHeight = height(Node->right);

        if(LHeight > RHeight) {
            return LHeight + 1;
        } else return RHeight + 1;
    }
}

int balance_factor(Node* node) {
    if(node == NULL) {
        return 0;
    }

   return height(node->left) - height(node->right);
}

Node* new_node(int value) {
    Node* new_node = new Node;
    new_node->value = value;
    new_node->right = NULL;
    new_node->left = NULL;
    new_node->height = 1;

    return new_node;
}


/* 
    Para balancear a árvore
        --> Realiza insersão normal do valor v na BSL
        --> Començando de v, busca o primeiro nodo
            desbalanceado
        --> Realiza as seguintes rotações
*/

Node *rotate_left(Node *x) {
    /*
    Sendo X o nodo pai, e Y o nodo filho a sua direita,
    se Y possui uma sub-árvore a esquerda, X será o pai 
    da sub-árvore a esquerda de Y

    SE o pai X for nulo, Y será a raiz da sub-árvore

    SE NÃO SE X for o filho a esqueda de P, Y se torna o
              o filho a esquerda de P

    SE NÃO Y vira filho a diretia de P

    Y se torna pai a de X 
    */

    Node *y = x->right;
    Node *aux = y->left;
    y->left = x;
    x->right = aux;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;   
}

Node *rotate_right(Node *x) {
    /*
    Sendo X o nodo pai, e Y o nodo filho a sua esquerda,
    se Y possui uma sub-árvore a direita, X será o pai 
    da sub-árvore a direita de Y

    SE o pai X for nulo, Y será a raiz da sub-árvore

    SE NÃO SE X for o filho a direita de P, Y se torna o
              o filho a direita de P

    SE NÃO Y vira filho a esquerda de P

    Y se torna pai a de X 
    */

    Node *y = x->left;
    Node *aux = y->right;
    y->right = x;
    x->left = aux;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node *rotate_right_left(Node *z) {
    /*
    Sendo Z, pai de X e X pai de Y,
                Z
               / \
              X  B
             / \
            A  Y
    Fazer a rotação a direita de X-Y
    Fazer a rotação a esquerda de Y-Z
    */

    // z->right = rotate_right(z->right);
    // z = rotate_left(z);
    // return z;
    

    z->right = rotate_right(z->right);
    return rotate_left(z);
}

Node *rotate_left_right(Node *z) {
     /*
    Sendo Z, pai de X e X pai de Y,
                Z
               / \
              X  B
             / \
            A  Y
    Fazer a rotação a esquerda de X-Y
    Fazer a rotação a direita de Y-Z
    */

    // z->left = rotate_left(z->left);
    // z = rotate_right(z);
    // return z;

    z->left = rotate_left(z->left);
    return rotate_right(z);
}

Node *insertNode(int key, Node* node) {

    if(node == NULL) {
        return(new_node(key));
    }

    if(node->value > key) {
        node->left = insertNode(key, node->left);
    }

    else if(node->value < key) {
        node->right = insertNode(key, node->right);
    }

    else {
        return node;
    }

    //atualiza altura e fator de balanceamento
    node->height = max(height(node->left), height(node->right)) + 1;

    int balance = balance_factor(node);

    //se altura do nodo for maior que um, significa
    //que a árvore está desbalanceada para a esquerda
    //então fazemos right rotation ou LR rotation
    if(balance > 1) {
        //se a nova chave for menor que a chave do filho 
        // a esquerda, rotacionamos para a direita
        if(key < node->left->value) {
            return rotate_right(node);
        }
        //se não, rotacionamos LR
        else if(key > node->left->value){
            return rotate_left_right(node);
        }
    }

    if(balance < -1) {
        if(key > node->right->value) {
            return rotate_left(node);
        }

        else if(key < node->right->value){
            return rotate_right_left(node);
        }
    }

    return node;
}

Node *deleteNode(Node *node, int key) {
    
    if(node == NULL) {
        return node;
    }

    if(node->value > key) {
        node->left = deleteNode(node->left, key);
    }

    if(node->value < key) {
        node->right = deleteNode(node->right, key);
    }

    else {

        //se nodo a remover for só uma folha,,
        //só remove o nodo

        //se tem um só filho, substiui o conteúdo
        //do nodo pelo do filho e remove ele

        if(node->left == NULL || node->right == NULL) {
            Node *aux = node->left ? node->left : node->right;
            if(aux == NULL) {
                aux = node;
                node = NULL;
            } 

            else 
                *node = *aux;
            
            free(aux);
        }

        //se tiver dois filhos:
        //---acha o filho com menor valor da árvore a direita
        //---substui o conteúdo
        //---remove o filho

        else {
            Node *nodeWithLowestValue = getLowestNode(node->right);
            node->value = nodeWithLowestValue->value;
            node->right = deleteNode(node->right, nodeWithLowestValue->value) ;
        }



    }
    
    //recalcula fatores de balanceamento
    node->height = max(height(node->left), height(node->right)) + 1;

    int balance = balance_factor(node);

    if(balance > 1) {
        if(key < node->left->value) {
            return rotate_right(node);
        }

        else if(key > node->left->value){
            return rotate_left_right(node);
        }
    }

    if(balance < -1) {
        if(key > node->right->value) {
            return rotate_left(node);
        }

        else if(key < node->right->value){
            return rotate_right_left(node);
        }
    }

    return node;
   
}

Node* getLowestNode(Node *node) {
    
    Node *current = node;

    while(current->left != NULL)
        current = current->left;
    
    return current;
}

void preorder_print(Node* node){
	if(node != NULL){
		cout << node->value << "\n";
        preorder_print(node->left);
        preorder_print(node->right);
	}
}

void printTree(Node *root, string indent, bool last) {
  if (root != nullptr) {
    cout << indent;
    if (last) {
      cout << "R----";
      indent += "   ";
    } else {
      cout << "L----";
      indent += "|  ";
    }
    cout << root->value << endl;
    printTree(root->left, indent, false);
    printTree(root->right, indent, true);
  }
}

int main() {
    Node *root = NULL;
    root = insertNode(10, root);
    root = insertNode(20, root);
    root = insertNode(30, root);
    root = insertNode(5, root);
    root = insertNode(3, root);
    root = insertNode(50, root);
    root = insertNode(40, root);
    root = insertNode(70, root);
    root = insertNode(60, root);
    root = insertNode(90, root);


    // printTree(root, "", true);
    preorder_print(root);
}