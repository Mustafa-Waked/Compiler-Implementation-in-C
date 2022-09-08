#include	"CodeGenerator.h"
#define MAX_BUFFER 300

int address = 5;
int flag = -1;
int label_count = 0;

typedef struct symbol_table {

  char name[MAX_BUFFER];
  char type[MAX_BUFFER];
  int addr;
  int size;
  struct symbol_table* next;

} Symbol_table;

Symbol_table* head;

int Search(char* var_name)
{
  Symbol_table* current = head;

  while(current != NULL)
  {
    if(strcmp(current->name,var_name) == 0)
    {
        return current->addr;
    }
    current = current->next;
  }
  return -1;
}

void Insert(Symbol_table** t, char* var_name,char* var_type)
{
    Symbol_table* v = malloc(sizeof(Symbol_table));
    if(v == NULL) return;

    strcpy(v->name,var_name);
    strcpy(v->type,var_type);
    v->addr = address;
    v->size = 1;
    v->next = NULL;

    if(*t == NULL)
    {
       *t = v;
    }
    else
    {
       Symbol_table* current = *t;
       while(current->next != NULL){
         current = current->next;
       }
       current->next = v;
    }
}

int  code_recur_aux(treenode *root)
{
	if_node  *ifn;
	for_node *forn;
	leafnode *leaf;
	int var_address,l1,l2;

    if (!root)
        return SUCCESS;

    switch (root->hdr.which){
		case LEAF_T:
			leaf = (leafnode *) root;
			switch (leaf->hdr.type) {
				case TN_LABEL:
					/* Maybe you will use it later */
					break;

				case TN_IDENT:
                    var_address = Search(leaf->data.sval->str);
				    if(var_address != -1)
                    {
                      printf("ldc %d\n",var_address);
                      flag = 0;
                    }
					/* variable case */
					/*
					*	In order to get the identifier name you have to use:
					*	leaf->data.sval->str
					*/
					break;

				case TN_COMMENT:
					/* Maybe you will use it later */
					break;

				case TN_ELLIPSIS:
					/* Maybe you will use it later */
					break;

				case TN_STRING:
					/* Maybe you will use it later */
					break;

				case TN_TYPE:
					/* Maybe you will use it later */
					break;

				case TN_INT:
				    printf("ldc %d\n",leaf->data.ival);
				    flag = 1;
					/* Constant case */
					/*
					*	In order to get the int value you have to use:
					*	leaf->data.ival
					*/
					break;

				case TN_REAL:
				    printf("ldc %f\n",leaf->data.dval);
				    flag = 1;
					/* Constant case */
					/*
					*	In order to get the real value you have to use:
					*	leaf->data.dval
					*/
					break;
			}
			break;

		case IF_T:
			ifn = (if_node *) root;
			switch (ifn->hdr.type) {

			case TN_IF:
				if (ifn->else_n == NULL)
                {
					/* if case (without else)*/
					l1 = label_count++;
					code_recur_aux(ifn->cond);
					printf("fjp Label%d\n",l1);
					code_recur_aux(ifn->then_n);
					printf("Label%d:\n",l1);
				}
				else {
					/* if - else case*/
					l1 = label_count++;
					l2 = label_count++;
					code_recur_aux(ifn->cond);
					printf("fjp Label%d\n",l1);
					code_recur_aux(ifn->then_n);
					printf("ujp Label%d\n",l2);
                    printf("Label%d:\n",l1);
					code_recur_aux(ifn->else_n);
					printf("Label%d:\n",l2);
				}
				break;

			case TN_COND_EXPR:
				/* (cond)?(exp):(exp); */
				l1 = label_count++;
                l2 = label_count++;
                code_recur_aux(ifn->cond);
                printf("fjp Label%d\n",l1);
                code_recur_aux(ifn->then_n);
                printf("ujp Label%d\n",l2);
                printf("Label%d:\n",l1);
                code_recur_aux(ifn->else_n);
                printf("Label%d:\n",l2);
				break;

			default:
				/* Maybe you will use it later */
				code_recur_aux(ifn->cond);
				code_recur_aux(ifn->then_n);
				code_recur_aux(ifn->else_n);
			}
			break;

		case FOR_T:
			forn = (for_node *) root;
			switch (forn->hdr.type) {

			case TN_FUNC_DEF:
				/* Function definition */
				/* e.g. int main(...) { ... } */
				/* Look at the output AST structure! */
				code_recur_aux(forn->init);
				code_recur_aux(forn->test);
				code_recur_aux(forn->incr);
				code_recur_aux(forn->stemnt);
				break;

			case TN_FOR:
				/* For case*/
				/* e.g. for(i=0;i<5;i++) { ... } */
				/* Look at the output AST structure! */
				l1 = label_count++;
                l2 = label_count++;
				code_recur_aux(forn->init);
				printf("Label%d:\n",l1);
				code_recur_aux(forn->test);
				printf("fjp Label%d\n",l2);
				code_recur_aux(forn->stemnt);
				code_recur_aux(forn->incr);
				printf("ujp Label%d\n",l1);
				printf("Label%d:\n",l2);
				break;

			default:
				/* Maybe you will use it later */
				code_recur_aux(forn->init);
				code_recur_aux(forn->test);
				code_recur_aux(forn->stemnt);
				code_recur_aux(forn->incr);
			}
			break;

		case NODE_T:
			switch (root->hdr.type) {
				case TN_PARBLOCK:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_PARBLOCK_EMPTY:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_TRANS_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_FUNC_DECL:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_FUNC_CALL:
					/* Function call */
					if (strcmp(((leafnode*)root->lnode)->data.sval->str, "printf") == 0) {
						/* printf case */
						/* The expression that you need to print is located in */
						/* the currentNode->right->right sub tree */
						/* Look at the output AST structure! */
						code_recur_aux(root->rnode);
						if(flag == 0)
                        {
                          printf("ind\n");
                          flag = -1;
                        }
						printf("print\n");
					}
					else {
						/* other function calls - for HW3 */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
					}
					break;

				case TN_BLOCK:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_ARRAY_DECL:
					/* array declaration - for HW2 */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_EXPR_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_NAME_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_ENUM_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_FIELD_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_PARAM_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_IDENT_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_TYPE_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_COMP_DECL:
					/* struct component declaration - for HW2 */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_DECL:
					/* structs declaration - for HW2 */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_DECL_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_DECLS:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_STEMNT_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_STEMNT:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_BIT_FIELD:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_PNTR:
					/* pointer - for HW2! */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_TYPE_NME:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_INIT_LIST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_INIT_BLK:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_OBJ_DEF:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_OBJ_REF:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_CAST:
					/* Maybe you will use it later */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_JUMP:
					if (root->hdr.tok == RETURN) {
						/* return jump - for HW2! */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
					}
					else if (root->hdr.tok == BREAK) {
						/* break jump - for HW2! */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
					}
					else if (root->hdr.tok == GOTO) {
						/* GOTO jump - for HW2! */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
					}
					break;

				case TN_SWITCH:
					/* Switch case - for HW2! */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_INDEX:
					/* call for array - for HW2! */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_DEREF:
					/* pointer derefrence - for HW2! */
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				case TN_SELECT:
					/* struct case - for HW2! */
					if (root->hdr.tok == ARROW){
						/* Struct select case "->" */
						/* e.g. struct_variable->x; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
					}
					else{
						/* Struct select case "." */
						/* e.g. struct_variable.x; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
					}
					break;

				case TN_ASSIGN:
					if(root->hdr.tok == EQ){
						/* Regular assignment "=" */
						/* e.g. x = 5; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
						if(flag == 0)
                        {
                            printf("ind\n");
                            flag = -1;
                        }
						printf("sto\n");
					}
					else if (root->hdr.tok == PLUS_EQ){
						/* Plus equal assignment "+=" */
						/* e.g. x += 5; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->lnode);
						printf("ind\n");
						code_recur_aux(root->rnode);
						printf("add\n");
						printf("sto\n");
					}
					else if (root->hdr.tok == MINUS_EQ){
						/* Minus equal assignment "-=" */
						/* e.g. x -= 5; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->lnode);
						printf("ind\n");
						code_recur_aux(root->rnode);
						printf("sub\n");
						printf("sto\n");
					}
					else if (root->hdr.tok == STAR_EQ){
						/* Multiply equal assignment "*=" */
						/* e.g. x *= 5; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->lnode);
						printf("ind\n");
						code_recur_aux(root->rnode);
						printf("mul\n");
						printf("sto\n");
					}
					else if (root->hdr.tok == DIV_EQ){
						/* Divide equal assignment "/=" */
						/* e.g. x /= 5; */
						code_recur_aux(root->lnode);
						code_recur_aux(root->lnode);
						printf("ind\n");
						code_recur_aux(root->rnode);
						printf("div\n");
						printf("sto\n");
					}
					break;

				case TN_EXPR:
					switch (root->hdr.tok) {
					  case CASE:
					      /* you should not get here */
						  code_recur_aux(root->lnode);
						  code_recur_aux(root->rnode);
						  break;

					  case INCR:
						  /* Increment token "++" */
						  code_recur_aux(root->lnode);
						  code_recur_aux(root->lnode);
						  printf("ind\n");
						  printf("ldc 1\n");
						  printf("add\n");
                          printf("sto\n");
						  break;

					  case DECR:
						  /* Decrement token "--" */
						  code_recur_aux(root->lnode);
						  code_recur_aux(root->lnode);
						  printf("ind\n");
						  printf("ldc 1\n");
						  printf("sub\n");
                          printf("sto\n");
						  break;

					  case PLUS:
					  	  /* Plus token "+" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("add\n");
						  break;

					  case MINUS:
					  	  /* Minus token "-" */
					  	  if(root->lnode == NULL)
                          {
                            code_recur_aux(root->rnode);
						    if(flag == 0)
                            {
                               printf("ind\n");
                               flag = -1;
                            }
                            printf("ldc -1\n");
                            printf("mul\n");
                          }
                          else
                          {
                            code_recur_aux(root->lnode);
						    if(flag == 0)
                            {
                               printf("ind\n");
                               flag = -1;
                            }
						    code_recur_aux(root->rnode);
						    if(flag == 0)
                            {
                               printf("ind\n");
                               flag = -1;
                            }
						    printf("sub\n");
                          }
						  break;

					  case DIV:
					  	  /* Divide token "/" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("div\n");
						  break;

					  case STAR:
					  	  /* multiply token "*" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("mul\n");
						  break;

					  case AND:
					  	  /* And token "&&" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("and\n");
						  break;

					  case OR:
					  	  /* Or token "||" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("or\n");
						  break;

					  case NOT:
					  	  /* Not token "!" */
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("not\n");
						  break;

					  case GRTR:
					  	  /* Greater token ">" */
						 code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("grt\n");
						  break;

					  case LESS:
					  	  /* Less token "<" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("les\n");
						  break;

					  case EQUAL:
					  	  /* Equal token "==" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("equ\n");
						  break;

					  case NOT_EQ:
					  	  /* Not equal token "!=" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("neq\n");
						  break;

					  case LESS_EQ:
					  	  /* Less or equal token "<=" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("leq\n");
						  break;

					  case GRTR_EQ:
					  	  /* Greater or equal token ">=" */
						  code_recur_aux(root->lnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  code_recur_aux(root->rnode);
						  if(flag == 0)
                          {
                             printf("ind\n");
                             flag = -1;
                          }
						  printf("geq\n");
						  break;

					  default:
						code_recur_aux(root->lnode);
						code_recur_aux(root->rnode);
						break;
					}
					break;

				case TN_WHILE:
					/* While case */
					l1 = label_count++;
                    l2 = label_count++;
                    printf("Label%d:\n",l1);
					code_recur_aux(root->lnode);
					printf("fjp Label%d\n",l2);
					code_recur_aux(root->rnode);
					printf("ujp Label%d\n",l1);
					printf("Label%d:\n",l2);
					break;

				case TN_DOWHILE:
					/* Do-While case */
					l1 = label_count++;
                    l2 = label_count++;
                    printf("Label%d:\n",l1);
					code_recur_aux(root->rnode);
					code_recur_aux(root->lnode);
					printf("fjp Label%d\n",l2);
					printf("ujp Label%d\n",l1);
					printf("Label%d:\n",l2);
					break;

				case TN_LABEL:
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
					break;

				default:
					code_recur_aux(root->lnode);
					code_recur_aux(root->rnode);
			}
			break;

		case NONE_T:
			printf("Error: Unknown node type!\n");
			exit(FAILURE);
    }

    return SUCCESS;
}

int  create_symbol_table(treenode *root, int level)
{
    if_node  *ifn;
    for_node *forn;
    leafnode *leaf;
    leafnode *leaf1;
    leafnode *leaf2;

    if (!root) return(0);

    switch (root->hdr.which){

    default:
    case NONE_T:
       return 0;

    case LEAF_T:
        leaf = (leafnode *) root;
        switch (leaf->hdr.type) {

        case TN_LABEL:
            break;

        case TN_IDENT:
            break;

        case TN_COMMENT:
            break;

        case TN_ELLIPSIS:
            break;

        case TN_STRING:
            break;

        case TN_TYPE:
            break;

        case TN_INT:
            break;

        case TN_REAL:
            break;

        default:
            break;
        }
        break;

    case IF_T:
        ifn = (if_node *) root;
        switch (ifn->hdr.type) {

        case TN_IF:
            create_symbol_table(ifn->cond, level);
            create_symbol_table(ifn->then_n, level+1);
            create_symbol_table(ifn->else_n, level+1);
            return(0);

        case TN_COND_EXPR:
            create_symbol_table(ifn->cond, level);
            create_symbol_table(ifn->then_n, level);
            create_symbol_table(ifn->else_n, level);
            break;

        default: break;
        }
        break;

    case FOR_T:
        forn = (for_node *) root;
        switch (forn->hdr.type) {

        case TN_FUNC_DEF:
            create_symbol_table(forn->init, level);
            create_symbol_table(forn->test, level);
            create_symbol_table(forn->incr, level);
            create_symbol_table(forn->stemnt, level);
            break;

        case TN_FOR:
            create_symbol_table(forn->init, level);
            create_symbol_table(forn->test, level);
            create_symbol_table(forn->incr, level);
            create_symbol_table(forn->stemnt, level+1);
            return 0;

        default:
            break;
        }
        break;

    case NODE_T:
        switch (root->hdr.type) {
			// start of Parc extension
		case TN_PARBLOCK:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
			break;

		case TN_PARBLOCK_EMPTY:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
			break;
			// end of Parc extension

        case TN_TRANS_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_FUNC_DECL:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_FUNC_CALL:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_BLOCK:
            create_symbol_table(root->lnode, level+1);
            create_symbol_table(root->rnode, level+1);
            return(0);

        case TN_ARRAY_DECL:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_EXPR_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_NAME_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_ENUM_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_FIELD_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_PARAM_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_IDENT_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_TYPE_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_DECL:
            leaf1 = (leafnode*)root->rnode;
            leaf2 = (leafnode*)root->lnode->lnode;

          if(Search(leaf1->data.sval->str) == -1)
          {
            if(leaf2->hdr.tok == INT)
            {
              Insert(&head,leaf1->data.sval->str,"int");
              address++;
            }

            if(leaf2->hdr.tok == FLOAT)
            {
              Insert(&head,leaf1->data.sval->str,"float");
              address++;
            }

            if(leaf2->hdr.tok == DOUBLE)
            {
              Insert(&head,leaf1->data.sval->str,"double");
              address++;
            }
          }
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_DECL_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_DECLS:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_STEMNT_LIST:
            create_symbol_table(root->lnode, level);
            if (root->rnode != NULL) {
                create_symbol_table(root->rnode, level);
            }
            break;

        case TN_STEMNT:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_COMP_DECL:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_BIT_FIELD:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_PNTR:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_TYPE_NME:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_INIT_LIST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_INIT_BLK:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_OBJ_DEF:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level+1);
            break;

        case TN_OBJ_REF:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_CAST:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_JUMP:
            if ((root->hdr.tok == RETURN) || (root->hdr.tok == GOTO))
            {
                create_symbol_table(root->lnode, level);
            }
            break;

        case TN_SWITCH:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level+1);
            break;

        case TN_INDEX:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_DEREF:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_SELECT:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_ASSIGN:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_EXPR:
            switch (root->hdr.tok) {
              case CASE:
                create_symbol_table(root->lnode, level);
                create_symbol_table(root->rnode, level);
                break;

              case SIZEOF:
                create_symbol_table(root->lnode, level);
                create_symbol_table(root->rnode, level);
                break;

              case INCR:
              case DECR:
                create_symbol_table(root->lnode, level);
                create_symbol_table(root->rnode, level);
                break;

              case B_AND:
                if (root->lnode == NULL) {
                  create_symbol_table(root->rnode, level);
                  break;
                }

              default:
                create_symbol_table(root->lnode, level);
                create_symbol_table(root->rnode, level);
                break;
            }
            break;

        case TN_WHILE:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level+1);
            return(0);

        case TN_DOWHILE:
            create_symbol_table(root->rnode, level+1);
            create_symbol_table(root->lnode, level);
            break;

        case TN_LABEL:
            create_symbol_table(root->lnode, level);
            create_symbol_table(root->rnode, level);
            break;

        case TN_EMPTY:
        default:
            return 0;
        }
       break;
    }

    return(1);
}

void FreeTable(Symbol_table* head)
{
   Symbol_table* temp;

   while (head != NULL)
    {
       temp = head;
       head = head->next;
       free(temp);
    }
}

int  code_recur(treenode *root)
{
    head = NULL;

    int p = create_symbol_table(root,0);

    int k = code_recur_aux(root);

    FreeTable(head);

    return 0;
}

/*
*	This function prints all the variables on your symbol table with their data
*	Input: treenode (AST)
*	Output: prints the Sumbol Table on the console
*/
void print_symbol_table(treenode *root)
{
  printf("---------------------------------------\n");
  printf("Showing the Symbol Table:\n\n");

  head = NULL;

  int p = create_symbol_table(root,0);

  Symbol_table* current = head;

  while(current != NULL)
  {
      printf("%s  %s  %d  %d\n\n",current->name,current->type,current->addr,current->size);
      current = current->next;
  }

  FreeTable(head);
}


