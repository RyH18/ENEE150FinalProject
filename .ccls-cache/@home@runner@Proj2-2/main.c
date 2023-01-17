//Ryan Horvath ENEE 150 Project 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct per{
    char * FirstName;
    char * phoneNumber;
    struct per * left;
    struct per *right;
}person;

person * root=NULL;
person * rdetachedtree=NULL;
person * ldetachedtree=NULL;

char* str2ptr();
void allocheck(void * ptr);
void AddNode();
void printTree(person * node);
void freeAll(person * node);
person* namelookup(char * name, person * node,int replwithnull);
void deletenode(char * name,person * node);
void changenum(char * name, person * node);
person * detachleftsubtree(person*node);
person * detachrightsubtree(person*node);
void attachlefttree(person *subtree,person *maintree);
void attachrighttree(person*subtree,person*maintree);

//////////////////////////////////////////////
int main() {
    char todo[10], name[20];
    int check;

    //create initial node
    AddNode(root);
    //ask what they want to do
    do{
        printf("What would you like to do? (a-add node, p-print tree, d-delete node, c-change number, dl-detach left, dr-detach right, al-attach left, ar-attach right, e-exit)\n");
        scanf("%s",&todo);//scanning as string to reduce errors for typing in multiple characters
        getchar();//sucks up extra character that causes problems
        if(strcmp(todo,"a")==0){
            AddNode(root);
        }
        else if (strcmp(todo,"p")==0){
            printf("Printing Tree\n");
            printTree(root);
        }
        else if (strcmp(todo,"d")==0){
            //asking who to delete
            printf("Who do you want to delete\n");
            scanf("%s",&name);
            getchar();
            //looking for their location and deleting
            deletenode(name,root);
        }
        else if(strcmp(todo,"c")==0){
            printf("Who's number do you want to change\n");
            scanf("%s",&name);
            getchar();
            changenum(name,root);
        }
        else if(strcmp(todo,"dl")==0){
            if (ldetachedtree!=NULL){
                printf("There is already data saved\n");
            }
            else{
                ldetachedtree=detachleftsubtree(root);
            }
        }
        else if(strcmp(todo,"dr")==0){
            if (rdetachedtree!=NULL){
                printf("There is already data saved\n");
            }
            else{
                rdetachedtree=detachrightsubtree(root);
            }
        }
        else if(strcmp(todo,"al")==0){
            attachlefttree(ldetachedtree,root);
        }
        else if(strcmp(todo,"ar")==0){
            attachrighttree(rdetachedtree,root);
        }
    }while(strcmp(todo,"e")!=0);
    // Then, print the content of the tree
    freeAll(root);
    return 0;
}
///////////////////////////////////////////

//function to assign a string to a pointer
char* str2ptr(){
    int c, i=0;
    //initialize string pointer
    char * strptr=(char*)malloc(sizeof(char));
    allocheck(strptr);

    //add keyboard inputs into allocated memory
    while((c=getchar())!='\n'&& c!=EOF){
        strptr[i++]=c;
        strptr = (char *)realloc(strptr,i+1);
        allocheck(strptr);
    }
    strptr[i]='\0';
    return (strptr);
}

//checks for errors while allocating memory
void allocheck(void * ptr){
    if (ptr==NULL){
        printf("Error allocating memory");
        exit(1);
    }
}

void AddNode(){
    int compare;
    person *cur;

    //initialize new node
    person *new;
    new=malloc(sizeof (person));
    allocheck(new);
    printf("Enter first name\n");
    new->FirstName=str2ptr();
    printf("Enter phone number\n");
    new->phoneNumber=str2ptr();
    new->left=NULL;
    new->right=NULL;


    //check if root has been used yet and fill in if it is empty
    if (root==NULL){
        root=new;
        return;
    }

    //go through tree and look for open space
    cur=root;
    //each loop is the next node on the branch
    while(1){
        //seeing which is bigger between new name and name of current node
        compare=strcmp(new->FirstName,cur->FirstName);
        if(compare<0){
            //new earlier than current node
            if (cur->left==NULL){
                //insert node
                cur->left=new;
                return;
            }
            else{
                //check next node
                cur=cur->left;
            }
        }
        else if(compare>0){
            //new later than current node
            if (cur->right==NULL){
                //insert node
                cur->right=new;
                return;
            }
            else{
                //check next node
                cur=cur->right;
            }
        }
        else{
            //does not account for duplicate names
            return;
        }
    }
}

void printTree(person * node){
    //using recursion
    if (node==NULL){
        //base case
        return;
    }
    //recursive case
    printTree(node->left);
    printf("%s ",node->FirstName);
    printf("%s\n",node->phoneNumber);
    printTree(node->right);
}

//uses recursion similar to printing but frees itself
void freeAll(person * node){
    if (node!=NULL){
        freeAll(node->left);
        free(node->FirstName);
        free(node->phoneNumber);
        freeAll(node->right);
        free(node);
    }
}

//uses fuction similar to addnode
person* namelookup(char * name, person * node,int replwithnull){
    int compare;
    person * prev;
    int leftorright;
    //searching from root to branch where each loop is the next node in the tree
    while(1){
        //seeing which is bigger between searched name and name of current node
        compare=strcmp(name,node->FirstName);
        if(compare<0){
            //name earlier in alphabet than current node
            if (node->left==NULL){
                //if we didn't find anyone by the leaf, return null
                printf("No one was found\n");
                return(NULL);
            }
            else{
                //check next node
                prev=node;
                leftorright=1;
                node=node->left;
            }
        }
        else if(compare>0){
            //name later in alphabet than current node
            if (node->right==NULL){
                //if we didn't find anyone by the leaf, return null
                printf("No one was found\n");
                return(NULL);
            }
            else{
                //check next node
                prev=node;
                leftorright=2;
                node=node->right;
            }
        }
        else if (compare==0){
            //if found
            //replwithnull determines wether or not to replace pointer to found node with null
            if (replwithnull==1){
                //uses previous node to replace pointer to found node with null. chooses side based on prevoius steps
                if (leftorright==1){
                    prev->left=NULL;
                }
                else if (leftorright==2){
                    prev->right=NULL;
                }
            }
            return(node);
        }
    }
}

void deletenode(char * name,person * node){
    person * nodeptr;
    //locate person in tree
    nodeptr=namelookup(name, node,1);
    if (node==nodeptr){
        printf("You cannot delete root node\n");
    }
        //check their pointer
    else if (nodeptr==NULL){
    }
    else{
        //free pointer
        freeAll(nodeptr);
        //return open space to null
    }
}

//changes number given a name
void changenum(char * name, person * node){
    person * nodeptr;
    //find node
    nodeptr=namelookup(name, node,0);
    if (nodeptr==NULL){
    }
    else{
        //update phone number
        printf("Enter phone number\n");
        nodeptr->phoneNumber=str2ptr();
    }
}

person * detachleftsubtree(person*node){
    char * name;
    person * detachednode;
    //check for subtree
    if (node->left==NULL){
        printf("Nothing in subtree\n");
        return(NULL);
    }
    else{
        //find pointer to left tree
        name= node->left->FirstName;
        //detach it
        detachednode=namelookup(name,node,1);
        //save it
        return(detachednode);
    }
}

person * detachrightsubtree(person*node){
    char * name;
    person * detachednode;
    //check for subtree
    if (node->right==NULL){
        printf("Nothing in subtree\n");
        return(NULL);
    }
    else{
        //find pointer to left tree
        name= node->right->FirstName;
        //detach it
        detachednode=namelookup(name,node,1);
        //save it
        return(detachednode);
    }
}

void attachlefttree(person*subtree,person*maintree){
    //check that there subtree isnt empty
    if (subtree==NULL){
        printf("Nothing in subtree\n");
    }
        //check for open spaces on main tree and then insert if open
    else if (maintree->left!=NULL){
        printf("There is already values in the left main tree\n");
    }
    else{
        maintree->left=subtree;
        ldetachedtree=NULL;
    }
}

void attachrighttree(person*subtree,person*maintree){
    //check that there subtree isnt empty
    if (subtree==NULL){
        printf("Nothing in subtree\n");
    }
        //check for open spaces on main tree and then insert if open
    else if (maintree->right!=NULL){
        printf("There is already values in the right main tree\n");
    }
    else{
        maintree->right=subtree;
        rdetachedtree=NULL;
    }
}