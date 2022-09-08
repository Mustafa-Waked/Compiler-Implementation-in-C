#include "CodeGenerator.h"
#define MAX_BUFFER 300

/** Helping global variables **/

int array_flag = 0;

int label_count = 0;

int switch_label_count = 0;

int switch_flag = 0;

int case_count = 0;

int main_flag = 0;

int current_address = 5;

int last_label_count;

int identifier_declaration_flag = 0;

int component_declaration_flag = 0;

int dimension_flag = 0;

int dimension_value;

char temp_type[MAX_BUFFER];

char str_aux[MAX_BUFFER];

char str_aux_1[MAX_BUFFER];

int node_flag = 0;

char current_type[MAX_BUFFER];

int ind_value_flag = 0;

char struct_name[MAX_BUFFER];

int flag_1 = 1;

int temp_address_1;


typedef struct symbol_table_entry
{
    char entry_name[MAX_BUFFER];

    char entry_type[MAX_BUFFER];

    int entry_address;

    int entry_size;

    int array_struct_flag;

    int value_flag;

    char string_name[MAX_BUFFER];

    int  entry_amount;

    int entry_dimension;

    int entry_distance;

    int element_size;

    struct symbol_table_entry* next;

    struct symbol_table_entry* prev;

} Symbol_Table_Entry;


typedef struct symbol_table
{
    Symbol_Table_Entry* head;

    Symbol_Table_Entry* end;

	int table_size;

} Symbol_Table;


Symbol_Table* symbol_table_obj;


typedef struct struct_object
{
    char struct_name[MAX_BUFFER];

    int struct_size;

    int struct_address;

    Symbol_Table_Entry* head;

    Symbol_Table_Entry* end;

    struct struct_object* next;

    struct struct_object* prev;

} Struct_Object;


typedef struct struct_object_table
{
    Struct_Object* head;

    Struct_Object* end;

    int table_size;

} Struct_Object_Table;


Struct_Object_Table *struct_table_obj;


Struct_Object_Table* Create_Struct_Table(Struct_Object_Table* t)
{
    Struct_Object_Table* table = (Struct_Object_Table*)malloc(sizeof(Struct_Object_Table));

    table->head = NULL;

    table->end = NULL;

    table->table_size = 0;

    return table;
}

void Insert_Struct_Entry(Struct_Object_Table* table, Struct_Object* entry)
{
    Struct_Object* x = (Struct_Object*)malloc(sizeof(Struct_Object));

    if(table->table_size == 0)
    {
        table->end = entry;

        table->head = entry;
    }
    else
    {
        x = table->end;

        x->next = entry;

        entry -> prev = x;

        table->end = entry;
    }

    table->table_size++;
}

Struct_Object* Search_Struct_Entry(Struct_Object_Table* table, char* struct_name)
{
    Struct_Object* current = (Struct_Object*)malloc(sizeof(Struct_Object));

    current = table->head;

    while(current != NULL)
    {
      if(strcmp(current->struct_name,struct_name) == 0)
      {
         break;
      }

      current = current->next;
    }

    if(current == NULL)
    {
      return NULL;
    }

    else
    {
      return current;
    }
}

void Insert_Symbol_Table_Entry(Symbol_Table* table, Symbol_Table_Entry* entry)
{
    if(table->table_size == 0)
    {
        table->end = entry;

        table->head = entry;
    }

    else
    {
      Symbol_Table_Entry* x = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

      x = table->end;

      x->next = entry;

      entry->prev = x;

      table->end = entry;
    }

    table->table_size++;
}

Symbol_Table* Create_Symbol_Table(Symbol_Table* t)
{
    Symbol_Table* table = (Symbol_Table*)malloc(sizeof(Symbol_Table));

    table->head = NULL;

    table->end = NULL;

    table->table_size = 0;

    return table;
}


Symbol_Table_Entry* Create_Symbol_Table_Entry(char name[MAX_BUFFER], char type[MAX_BUFFER], int size, int array_struct_flag
                                    , int value_flag, int value, int element_size, int entry_amount)
{
    Symbol_Table_Entry* entry = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

    if(array_struct_flag == 0)
    {
        strcpy(entry->entry_name,name);

        strcpy(entry->entry_type,type);

        entry->entry_size = 1;

        entry->array_struct_flag = 0;

        return entry;
    }

    else if(array_struct_flag == 1)
    {

        if(value_flag == 0)
        {
            strcpy(entry->entry_name,name);

            strcpy(entry->entry_type,type);

            entry->entry_size = size*element_size;

            entry->array_struct_flag = 1;
        }

        else
        {
            entry->entry_size = 1;

            entry->array_struct_flag = 1;

            entry->entry_dimension = value;

            entry->value_flag = 1;
        }

        entry->element_size = element_size;

        entry->entry_amount = entry_amount;

        return entry;
    }

    else
    {
        if(value_flag == 0)
        {
            strcpy(entry->entry_name,name);

            strcpy(entry->entry_type,type);

            entry->entry_size = size;

            entry->array_struct_flag = 2;
        }

        else
        {
            entry->entry_size = size;

            entry->array_struct_flag = 2;
        }

        entry->element_size = element_size;

        entry->entry_amount = entry_amount;

        return entry;
    }

}

Symbol_Table_Entry* Search_Symbol_Table_Entry(Symbol_Table* table, char name[MAX_BUFFER], int array_struct_flag)
{
    Symbol_Table_Entry* current = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

    current = table->head;

    while(current != NULL)
    {
        if((strcmp(current->entry_name,name)== 0)&& (current->array_struct_flag == array_struct_flag))
        {
          break;
        }

        current = current->next;
    }

    if(current == NULL)
    {
       return NULL;
    }

    else
    {
      return current;
    }
}

void Remove_Symbol_Table_Last_Entry(Symbol_Table* table)
{
    Symbol_Table_Entry* target;

    while(table->table_size != 0)
    {
        if(table->table_size > 1)
        {
          target = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

          target = table->head;

          table->head = target->next;

          target->next = NULL;

          table->head->prev = NULL;

          free(target);

          table->table_size --;

        }

        if(table->table_size == 1)
        {
            free(table->head);

            table->table_size--;
        }
    }

    free(table);
}


int code_recur(treenode *root)
{
    /**  Local Variables  **/

    if_node  *ifn;

	for_node *forn;

    int shift_amount;

    int first_label;

    int second_label;

    int i;

    int d = 0;

    int count;

    int var_size;

	leafnode *leaf;

	treenode* temp_root_1;

	treenode* temp_root_2;

	Symbol_Table_Entry* temp_var_1;

	Symbol_Table_Entry* temp_var_2;

	Struct_Object* temp_struct_1;

	Struct_Object* temp_struct_2;

	int  amount_flag;

	int array_select_count = 0;

	int dreference_flag = 0;

    if(symbol_table_obj == NULL)
    {
      symbol_table_obj = Create_Symbol_Table(symbol_table_obj);
    }

    if(struct_table_obj == NULL)
    {
       struct_table_obj = Create_Struct_Table(struct_table_obj);
    }

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
					/* variable case */
					/*
					*	In order to get the identifier name you have to use:
					*	leaf->data.sval->str
					*/
                    if(node_flag == 1)
                    {
                        strcpy(str_aux,leaf->data.sval->str);

                        if(flag_1 != 0)
                        {
                            temp_var_1 = Search_Symbol_Table_Entry(symbol_table_obj,str_aux,2);

                            printf("ldc %d\n",temp_var_1->entry_address);

                            strcpy(temp_type,temp_var_1->entry_type);

                            flag_1 = 0;
                        }

                    }

                    else
                    {
                        if(identifier_declaration_flag == 0)
                        {
                            if(Search_Symbol_Table_Entry(symbol_table_obj,leaf->data.sval->str,0) != 0)
                            {
                               printf("ldc %d\n",Search_Symbol_Table_Entry(symbol_table_obj,leaf->data.sval->str,0)->entry_address);
                            }

                            if(Search_Symbol_Table_Entry(symbol_table_obj,leaf->data.sval->str,2) != 0)
                            {
                               printf("ldc %d\n",Search_Symbol_Table_Entry(symbol_table_obj,leaf->data.sval->str,2)->entry_address);

                               strcpy(temp_type,Search_Symbol_Table_Entry(symbol_table_obj,leaf->data.sval->str,2)->entry_type);
                            }

                            main_flag = 1;
                        }
                     }

                    if(dreference_flag == 1)
                    {
                        strcpy(str_aux,leaf->data.sval->str);

                        dreference_flag = 0;
                    }

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
					/* Constant case */
					/*
					*	In order to get the int value you have to use:
					*	leaf->data.ival
					*/

					if(dimension_flag == 1)
                    {
                        dimension_flag = 0;

                        dimension_value = leaf->data.ival;
					}

					else
                    {
                        printf("ldc %d\n",leaf->data.ival);
					}

					break;

				case TN_REAL:
					/* Constant case */
					/*
					*	In order to get the real value you have to use:
					*	leaf->data.dval
					*/

					printf("ldc %f\n",leaf->data.dval);

					break;
			}
			break;

		case IF_T:
			ifn = (if_node *) root;
			switch (ifn->hdr.type){

			case TN_IF:

				if (ifn->else_n == NULL)
                {
                  second_label = ++label_count;

                  code_recur(ifn->cond);

                  node_flag = 0;

                  if(main_flag != 0)
                  {
                     main_flag = 0;

                     printf("ind\n");
                  }

                  printf("fjp label_%d\n",second_label);

                  code_recur(ifn->then_n);

                  node_flag = 0;

                  printf("label_%d:\n",second_label);
				}

				else
                {
					first_label = ++label_count;

					second_label = ++label_count;

					code_recur(ifn->cond);

					node_flag = 0;

					if(main_flag != 0)
                    {
                        main_flag = 0;

                        printf("ind\n");
					}

                    printf("fjp label_%d\n",first_label);

					code_recur(ifn->then_n);

                    printf("ujp label_%d\n",second_label);

                    printf("label_%d:\n",first_label);

					code_recur(ifn->else_n);

					printf("label_%d:\n",second_label);
				}

				break;

			case TN_COND_EXPR:
				/* (cond)?(exp):(exp); */

				first_label = ++label_count;

                second_label = ++label_count;

				code_recur(ifn->cond);

				node_flag = 0;

				if(main_flag != 0)
                {
                   main_flag = 0;

                   printf("ind\n");
                }

                printf("fjp label_%d\n",first_label);

                code_recur(ifn->then_n);

                printf("ujp label_%d\n",second_label);

                printf("label_%d:\n",first_label);

				code_recur(ifn->else_n);

				printf("label_%d:\n",second_label);

				break;

			default:
				/* Maybe you will use it later */
				code_recur(ifn->cond);
				code_recur(ifn->then_n);
				code_recur(ifn->else_n);
			}
			break;

		case FOR_T:
			forn = (for_node *) root;
			switch (forn->hdr.type) {

			case TN_FUNC_DEF:
				/* Function definition */
				/* e.g. int main(...) { ... } */
				/* Look at the output AST structure! */
				code_recur(forn->init);
				code_recur(forn->test->rnode);
				code_recur(forn->incr);
				code_recur(forn->stemnt);
				break;

			case TN_FOR:
				/* For case*/
				/* e.g. for(i=0;i<5;i++) { ... } */
				/* Look at the output AST structure! */

				first_label = ++label_count;

				second_label = ++label_count;

				last_label_count = second_label;

				code_recur(forn->init);

				printf("label_%d:\n",first_label);

				code_recur(forn->test);

				node_flag = 0;

				if(main_flag != 0)
                {
                  main_flag=0;
                  printf("ind\n");
                }

				printf("fjp label_%d\n",second_label);

				code_recur(forn->stemnt);

				code_recur(forn->incr);

				printf("ujp label_%d\n",first_label);

				printf("label_%d:\n",second_label);

				break;

			default:
				/* Maybe you will use it later */
				code_recur(forn->init);
				code_recur(forn->test);
				code_recur(forn->stemnt);
				code_recur(forn->incr);
			}
			break;

		case NODE_T:
			switch (root->hdr.type) {
				case TN_PARBLOCK:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_PARBLOCK_EMPTY:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_TRANS_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_FUNC_DECL:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_FUNC_CALL:
					/* Function call */
					if (strcmp(((leafnode*)root->lnode)->data.sval->str, "printf") == 0)
                    {
						/* printf case */
						/* The expression that you need to print is located in */

						/* the currentNode->right->right sub tree */
						/* Look at the output AST structure! */

						code_recur(root->rnode->rnode);

						node_flag = 0;

						if(main_flag != 0)
                        {
                            printf("ind\n");

                            main_flag = 0;
						}

						printf("print\n");
					}

					else {
						/* other function calls - for HW3 */
						code_recur(root->lnode);
						code_recur(root->rnode);
					}

					break;

				case TN_BLOCK:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_ARRAY_DECL:
					/* array declaration - for HW2*/

					array_flag = 1;

					var_size = 1;

					amount_flag = 0;

					temp_root_1 = root;

					while(root->rnode != NULL)
                    {
                      root = root->lnode;
					}

					leaf = (leafnode*)root;

                    strcpy(str_aux,leaf->data.sval->str);

                    root = temp_root_1;

					while(root->rnode != NULL)
                    {
                      dimension_flag = 1;

                      code_recur(root->rnode);

                      var_size *= dimension_value;

                      amount_flag++;

                      dimension_flag = 0;

                      root = root->lnode;
					}

					root = temp_root_1;

					if(Search_Symbol_Table_Entry(symbol_table_obj,temp_type,1)!= NULL)
                    {
                        var_size *= Search_Symbol_Table_Entry(symbol_table_obj,temp_type,1)->entry_size;

                        temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,1,0,0,
                                       Search_Symbol_Table_Entry(symbol_table_obj,temp_type,1)->element_size,amount_flag);
					}

                    else if(Search_Struct_Entry(struct_table_obj,temp_type)!= NULL)
                    {
                        var_size *= Search_Struct_Entry(struct_table_obj,temp_type)->struct_size;

                        temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,1,0,0,
                                                               Search_Struct_Entry(struct_table_obj,temp_type)->struct_size,amount_flag);
                    }

                    else
                    {
                        temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,1,0,0,1,amount_flag);
                    }

                    if(component_declaration_flag == 0)
                    {
                        temp_var_1->entry_address = current_address;

                        current_address += var_size;

                        Insert_Symbol_Table_Entry(symbol_table_obj,temp_var_1);
                    }

                    else
                    {
                        strcpy(temp_var_1->string_name,str_aux_1);

                        if(struct_table_obj->end->struct_size == 0)
                        {
                          struct_table_obj->end->head = temp_var_1;

                          struct_table_obj->end->end = struct_table_obj->end->head;

                          struct_table_obj->end->end->entry_distance = 0;
                        }

                        else
                        {
                            temp_var_2 = struct_table_obj->end->end;

                            struct_table_obj->end->end->next = temp_var_1;

                            temp_var_1->prev = struct_table_obj->end->end;

                            struct_table_obj->end->end = temp_var_1;

                            struct_table_obj->end->end->entry_distance = (temp_var_2->entry_distance + temp_var_2->entry_size);

                        }

                    }

					while(root->rnode != NULL)
                    {
                        dimension_flag = 1;

                        code_recur(root->rnode);

                        temp_var_1 = Create_Symbol_Table_Entry(NULL,NULL,1,1,1,dimension_value,1,1);

                        if(component_declaration_flag == 0)
                        {
                            Insert_Symbol_Table_Entry(symbol_table_obj,temp_var_1);
                        }

                        else
                        {
                            struct_table_obj->end->end->next = temp_var_1;

                            temp_var_1->prev = struct_table_obj->end->end;

                            struct_table_obj->end->end = temp_var_1;

                            struct_table_obj->end->end->entry_size = var_size;

                            struct_table_obj->end->struct_size += var_size;
                        }

                        dimension_flag = 0;

                        root = root->lnode;
					}

                    root = temp_root_1;

					break;

				case TN_EXPR_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_NAME_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_ENUM_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_FIELD_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);;
					break;

				case TN_PARAM_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_IDENT_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_TYPE_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_COMP_DECL:
					/* struct component declaration - for HW2 */

					identifier_declaration_flag = 1;

                    component_declaration_flag = 1;

					temp_var_1 = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

                    if(root->lnode->lnode->hdr.type != TN_OBJ_REF)
                    {
                        leaf = (leafnode*)root->lnode->lnode;

                        strcpy(temp_type,toksym(leaf->hdr.tok,1));

                        temp_root_2 = (treenode *)root->rnode->lnode;

                        if(temp_root_2->hdr.type != TN_PNTR)
                        {
                            code_recur(root->rnode);

                            if(array_flag == 0)
                            {
                                leaf = (leafnode*)root->rnode;

                                strcpy(str_aux,leaf->data.sval->str);

                                leaf = (leafnode*)root->lnode->lnode;

                                strcpy(temp_type,toksym(leaf->hdr.tok,1));

                                temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,1,0,0,0,1,1);

                                if(struct_table_obj->end->struct_size == 0)
                                {
                                    struct_table_obj->end->head = temp_var_1;

                                    struct_table_obj->end->head->entry_distance = 0;

                                    struct_table_obj->end->end = struct_table_obj->end->head;

                                    struct_table_obj->end->struct_size++;
                                }

                                else
                                {
                                    temp_var_2 = struct_table_obj->end->end;

                                    count = struct_table_obj->end->end->entry_size;

                                    struct_table_obj->end->end->next = temp_var_1;

                                    temp_var_1->prev = struct_table_obj->end->end;

                                    struct_table_obj->end->end = temp_var_1;

                                    struct_table_obj->end->end->entry_distance = (temp_var_2->entry_distance + temp_var_2->entry_size);

                                    struct_table_obj->end->struct_size++;
                                }
                            }

                            else
                            {
                                array_flag = 0;
                            }

                        }

                        else
                        {
                          leaf = (leafnode*)root->rnode->rnode;

                          strcpy(str_aux,leaf->data.sval->str);

                          temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,1,0,0,0,1,1);

                          if(struct_table_obj->end->struct_size == 0)
                          {
                             struct_table_obj->end->head = temp_var_1;

                             struct_table_obj->end->head->entry_distance = 0;

                             struct_table_obj->end->end = struct_table_obj->end->head;

                             struct_table_obj->end->struct_size++;
                          }

                          else
                          {
                             temp_var_2 = struct_table_obj->end->end;

                             count = struct_table_obj->end->end->entry_size;

                             struct_table_obj->end->end->next = temp_var_1;

                             temp_var_1->prev = struct_table_obj->end->end;

                             struct_table_obj->end->end = temp_var_1;

                             struct_table_obj->end->end->entry_distance = (temp_var_2->entry_distance + temp_var_2->entry_size);

                             struct_table_obj->end->struct_size++;
                           }
                        }
                    }

                    else
                    {
                        leaf = (leafnode*)(root->lnode->lnode->lnode);

                        strcpy(temp_type,leaf->data.sval->str);

                        temp_root_2 = (treenode*)root->rnode->lnode;

                        if(temp_root_2->hdr.type != TN_PNTR)
                        {
                           leaf = (leafnode*)root->rnode;
                        }

                        else
                        {
                           leaf = (leafnode*)root->rnode->rnode;
                        }

                        strcpy(str_aux,leaf->data.sval->str);

                        var_size = 1;

                        if(root->rnode->lnode->hdr.type != TN_PNTR)
                        {
                           var_size = Search_Struct_Entry(struct_table_obj,temp_type)->struct_size;
                        }

                        temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,2,0,0,1,1);

                        if(struct_table_obj->end->struct_size == 0)
                        {
                            struct_table_obj->end->head = temp_var_1;

                            struct_table_obj->end->head->entry_distance = 0;

                            struct_table_obj->end->end = struct_table_obj->end->head;

                            struct_table_obj->end->struct_size += var_size;
                        }

                        else
                        {
                            temp_var_2 = struct_table_obj->end->end;

                            count = struct_table_obj->end->end->entry_size;

                            struct_table_obj->end->end->next = temp_var_1;

                            temp_var_1->prev = struct_table_obj->end->end;

                            struct_table_obj->end->end = temp_var_1;

                            struct_table_obj->end->struct_size += var_size;

                            struct_table_obj->end->end->entry_distance = (temp_var_2->entry_distance + temp_var_2->entry_size);
                        }
                    }

                    temp_var_1 = NULL;

                    free(temp_var_1);

                    component_declaration_flag = 0;

                    identifier_declaration_flag = 0;

					break;

				case TN_DECL:

					identifier_declaration_flag = 1;

					temp_var_1 = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

                    if(root->lnode->lnode->hdr.type != TN_OBJ_DEF)
                    {
                        leaf = (leafnode*)root->lnode->lnode;

                        strcpy(temp_type,toksym(leaf->hdr.tok,1));

                        if(root->lnode->lnode->hdr.type == TN_OBJ_REF)
                        {
                            leaf = (leafnode*)root->lnode->lnode->lnode;

                            strcpy(temp_type,leaf->data.sval->str);
                        }

                        temp_root_2 = (treenode*)root->rnode->lnode;

                        if(temp_root_2->hdr.type != TN_PNTR)
                        {
                          code_recur(root->rnode);

                          if(array_flag == 0)
                          {
                             leaf = (leafnode*)root->rnode;

                             strcpy(str_aux,leaf->data.sval->str);

                             var_size = 1;

                             if(root->lnode->lnode->hdr.type == TN_OBJ_REF)
                             {
                                 var_size = Search_Struct_Entry(struct_table_obj,temp_type)->struct_size;

                                 temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,2,0,0,1,1);
                             }

                             else
                             {
                                 temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,1,0,0,0,1,1);
                             }

                             temp_var_1->entry_address = current_address;

                             current_address += var_size;

                             Insert_Symbol_Table_Entry(symbol_table_obj,temp_var_1);

                            }

                            else
                            {
                                array_flag = 0;
                            }

                        }

                        else
                        {
                            leaf = (leafnode*)root->rnode->rnode;

                            strcpy(str_aux,leaf->data.sval->str);

                            var_size = 1;

                            if(root->lnode->lnode->hdr.type == TN_OBJ_REF)
                            {
                              temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,2,0,0,1,1);
                            }

                            else
                            {
                              temp_var_1 = Create_Symbol_Table_Entry(str_aux,temp_type,var_size,0,0,0,1,1);
                            }

                            temp_var_1->entry_address = current_address;

                            current_address += var_size;

                            Insert_Symbol_Table_Entry(symbol_table_obj,temp_var_1);
                        }

                    }

                    else
                    {
                      code_recur(root->lnode);
                    }

					temp_var_1 = NULL;

                    free(temp_var_1);

					identifier_declaration_flag = 0;

					break;

				case TN_DECL_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_DECLS:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_STEMNT_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_STEMNT:
					/* Maybe you will use it later */

					ind_value_flag = 0;

					node_flag = 0;

					code_recur(root->lnode);
					code_recur(root->rnode);

					break;

				case TN_BIT_FIELD:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_PNTR:
					/* pointer - for HW2! */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_TYPE_NME:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_INIT_LIST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_INIT_BLK:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_OBJ_DEF:
					/* Maybe you will use it later */

					temp_struct_1 = (Struct_Object*)malloc(sizeof(Struct_Object));

					leaf = (leafnode*)root->lnode;

                    strcpy(temp_struct_1->struct_name,leaf->data.sval->str);

                    strcpy(str_aux_1,temp_struct_1->struct_name);

                    temp_struct_1->struct_size = 0;

                    Insert_Struct_Entry(struct_table_obj,temp_struct_1);

					code_recur(root->rnode);

					temp_struct_1 = NULL;

					free(temp_struct_1);

					break;

				case TN_OBJ_REF:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_CAST:
					/* Maybe you will use it later */
					code_recur(root->lnode);
					code_recur(root->rnode);
					break;

				case TN_JUMP:
					if (root->hdr.tok == RETURN) {
						/* return jump - for HW2! */
						code_recur(root->lnode);
						code_recur(root->rnode);
						Remove_Symbol_Table_Last_Entry(symbol_table_obj);
					}
					else if (root->hdr.tok == BREAK) {
						/* break jump - for HW2! */
						code_recur(root->lnode);
						code_recur(root->rnode);

						if(switch_flag == 1)
                        {
                          printf("ujp end_switch_case%d\n",switch_label_count);
                          printf("end_case%d_%d:\n",switch_label_count,case_count);
						}

						else
                        {
                          printf("ujp label_%d\n",last_label_count);
						}
					}
					else if (root->hdr.tok == GOTO) {
						/* GOTO jump - for HW2! */
						code_recur(root->lnode);
						code_recur(root->rnode);
					}
					break;

				case TN_SWITCH:
					/* Switch case - for HW2! */

					switch_flag = 1;

					++switch_label_count;

					code_recur(root->lnode);

					node_flag = 0;

					if(main_flag != 0)
                    {
                        printf("ind\n");

                        main_flag = 0;
                    }

					code_recur(root->rnode);

					case_count = 0;

					printf("end_switch_case%d:\n",switch_label_count);

					switch_flag = 0;

					break;

				case TN_INDEX:
					/* call for array - for HW2! */

                    main_flag = 1;

                    if(node_flag == 1)
                    {
                        ind_value_flag = 1;
                    }

					temp_var_1 = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

					temp_var_2 = (Symbol_Table_Entry*)malloc(sizeof(Symbol_Table_Entry));

					temp_root_1 = root;

					if(root->lnode->hdr.type != TN_DEREF)
                    {
					  if(root->lnode->hdr.type != TN_SELECT)
					  {
                        while(root->rnode != NULL)
                        {
                            root = root->lnode;

                            if(root->lnode->hdr.type == TN_SELECT)
                            {
                                array_select_count++;

                                code_recur(root->lnode);

                                if(node_flag == 1)
                                {
                                    node_flag = 0;

                                    temp_var_1 = Search_Struct_Entry(struct_table_obj,struct_name)->head;

                                    while(temp_var_1 != NULL)
                                    {
                                        if(strcmp(temp_var_1->entry_name,str_aux)== 0)
                                        {
                                          break;
                                        }

                                        temp_var_1 = temp_var_1->next;
                                    }
                                }
                            }

                            if(array_select_count != 0)
                            {
                                break;
                            }
                        }

                        if(array_select_count == 0)
                        {
                            array_select_count = 0;

                            leaf=(leafnode*)root;

                            strcpy(str_aux,leaf->data.sval->str);

                            temp_var_1 = Search_Symbol_Table_Entry(symbol_table_obj,str_aux,1);

                            temp_address_1 = temp_var_1->entry_address;

                            printf("ldc %d\n",temp_address_1);
                        }

                        else
                        {
                           array_select_count = 0;
                        }

                    }

                    else
                    {
                        code_recur(root->lnode);

                        if(node_flag == 1)
                        {
                          node_flag = 0;

                          temp_var_1 = Search_Struct_Entry(struct_table_obj,struct_name)->head;

                          while(temp_var_1 != NULL)
                          {

                            if(strcmp(temp_var_1->entry_name,str_aux) == 0)
                            {
                              break;
                            }

                            temp_var_1 = temp_var_1->next;
                          }

                        }

                        else
                        {
                          printf("ldc %d\n",temp_address_1);
                        }
					}

                    root = temp_root_1;

                    var_size = temp_var_1->entry_amount;

                    shift_amount = temp_var_1->element_size;

                    temp_var_2 = temp_var_1;

                    while(var_size > 1)
                    {
                        temp_var_2 = temp_var_2->next;

                        shift_amount *= temp_var_2->entry_dimension;

                        var_size--;
                    }

                    temp_root_2 = temp_root_1;

                    var_size = temp_var_1->entry_amount;

                    if(var_size == 1)
                    {
                      temp_var_2 = temp_var_2->next;
                    }

					while(var_size >= 1)
                    {
					    root = temp_root_1;

                        count = var_size;

                        while(count > 1)
                        {
                            root = root->lnode;

                            count--;
                        }

                        main_flag = 0;

                        code_recur(root->rnode);

                        if(main_flag == 1)
                        {
                          printf("ind\n");
                        }

                        main_flag = 1;

                        printf("ixa %d\n",shift_amount);

                        var_size--;

                        if(var_size > 0)
                        {
                           shift_amount /= temp_var_2->entry_dimension;
                        }

                        if(var_size > 1)
                        {
                           temp_var_2 = temp_var_2->prev;
                        }
					}
                  }

                  else
                  {
                        code_recur(root->lnode);
                        code_recur(root->rnode);

                        if(Search_Struct_Entry(struct_table_obj,temp_type) != NULL)
                        {
                           printf("ixa %d\n",Search_Struct_Entry(struct_table_obj,temp_type)->struct_size);
                        }

                        else
                        {
                           printf("ixa 1\n");
                        }
					}

					root = temp_root_2;

					break;

				case TN_DEREF:
					/* pointer derefrence - for HW2! */

					dreference_flag = 1;

					code_recur(root->lnode);
					code_recur(root->rnode);

					printf("ind\n");

					dreference_flag = 0;

					break;

				case TN_SELECT:
					/* struct case - for HW2! */
					if (root->hdr.tok == ARROW){
						/* Struct select case "->" */
						/* e.g. struct_variable->x; */

                        node_flag = 1;

                        flag_1 = 1;

                        code_recur(root->lnode);

                        flag_1 = 0;

                        if(ind_value_flag != 0)
                        {
                            if(flag_1 != 0)
                            {
                                flag_1 = 0;

                                strcpy(temp_type,str_aux_1);

                                ind_value_flag = 0;
                            }
                        }

                        printf("ind\n");

						code_recur(root->rnode);

						temp_var_1 = Search_Struct_Entry(struct_table_obj,temp_type)->head;

						while(temp_var_1 != NULL)
                        {
                            if(strcmp(str_aux,temp_var_1->entry_name) == 0)
                            {
                               break;
                            }

                            temp_var_1 = temp_var_1->next;
						}

                        printf("inc %d\n",temp_var_1->entry_distance);

                        strcpy(temp_type,temp_var_1->entry_type);

                        main_flag = 1;

                        strcpy(struct_name,temp_var_1->string_name);
					}
					else{
						/* Struct select case "." */
						/* e.g. struct_variable.x; */

                        node_flag = 1;

                        flag_1 = 1;

                        code_recur(root->lnode);

                        if(ind_value_flag != 0)
                        {
                            if(flag_1 != 0)
                            {
                                flag_1 = 0;

                                strcpy(temp_type,str_aux_1);

                                ind_value_flag = 0;
                            }
                        }

						code_recur(root->rnode);

						temp_var_1 = Search_Struct_Entry(struct_table_obj,temp_type)->head;

						while(temp_var_1 != NULL)
                        {
                            if(strcmp(str_aux,temp_var_1->entry_name) == 0)
                            {
                              break;
                            }

                            temp_var_1 = temp_var_1->next;
						}

                        printf("inc %d\n",temp_var_1->entry_distance);

                        strcpy(temp_type,temp_var_1->entry_type);

                        main_flag = 1;

                        strcpy(struct_name,temp_var_1->string_name);
					}

					break;

				case TN_ASSIGN:
					if(root->hdr.tok == EQ){
						/* Regular assignment "=" */
						/* e.g. x = 5; */

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						code_recur(root->rnode);

						node_flag = 0;

						if(main_flag != 0)
                        {
                            printf("ind\n");

                            main_flag = 0;
						}

                        printf("sto\n");
					}
					else if (root->hdr.tok == PLUS_EQ){
						/* Plus equal assignment "+=" */
						/* e.g. x += 5; */

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						printf("ind\n");

						code_recur(root->rnode);

						node_flag = 0;

						if(main_flag != 0)
                        {
                            printf("ind\n");

                            main_flag = 0;
						}

						printf("add\n");

						printf("sto\n");

					}
					else if (root->hdr.tok == MINUS_EQ){
						/* Minus equal assignment "-=" */
						/* e.g. x -= 5; */

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						printf("ind\n");

						code_recur(root->rnode);

						node_flag = 0;

						if(main_flag != 0)
                        {
                            printf("ind\n");

                            main_flag = 0;
						}

						printf("sub\n");

						printf("sto\n");
					}
					else if (root->hdr.tok == STAR_EQ){
						/* Multiply equal assignment "*=" */
						/* e.g. x *= 5; */

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						printf("ind\n");

						code_recur(root->rnode);

						node_flag = 0;

						if(main_flag != 0)
                        {
                            printf("ind\n");

                            main_flag = 0;
						}

						printf("mul\n");

						printf("sto\n");
					}
					else if (root->hdr.tok == DIV_EQ){
						/* Divide equal assignment "/=" */
						/* e.g. x /= 5; */

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						code_recur(root->lnode);

						node_flag = 0;

						main_flag = 0;

						printf("ind\n");

						code_recur(root->rnode);

						node_flag = 0;

						if(main_flag != 0)
                        {
                            printf("ind\n");

                            main_flag=0;
						}

						printf("div\n");

						printf("sto\n");
					}

					break;

				case TN_EXPR:
					switch (root->hdr.tok) {
					  case CASE:
					      /* you should not get here */
						  code_recur(root->lnode);
						  code_recur(root->rnode);
						  break;

					  case INCR:
						  /* Increment token "++" */
                          code_recur(root->lnode);

                          node_flag = 0;

                          if(main_flag != 0)
                          {
                            main_flag = 0;
                          }

                          if(root->lnode!= NULL)
                          {
                              code_recur(root->lnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");

                              printf("ldc 1\n");

                              printf("add\n");

                              printf("sto\n");

                              code_recur(root->lnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");

                              printf("dec 1\n");
                          }

                          code_recur(root->rnode);

                          node_flag = 0;

                          if(main_flag != 0)
                          {
                            main_flag = 0;
                          }

                          if(root->rnode != NULL)
                          {
                              code_recur(root->rnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");

                              printf("ldc 1\n");

                              printf("add\n");

                              printf("sto\n");

                              code_recur(root->rnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");
                          }

						  break;

					  case DECR:
						  /* Decrement token "--" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            main_flag = 0;
                          }

                          if(root->lnode != NULL)
                          {
                              code_recur(root->lnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");

                              printf("dec 1\n");

                              printf("sto\n");

                              code_recur(root->lnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");

                              printf("inc 1\n");
                          }

                          code_recur(root->rnode);

                          node_flag = 0;

                          if(main_flag != 0)
                          {
                            main_flag = 0;
                          }

                          if(root->rnode != NULL)
                          {
                              code_recur(root->rnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");

                              printf("dec 1\n");

                              printf("sto\n");

                              code_recur(root->rnode);

                              node_flag = 0;

                              if(main_flag != 0)
                              {
                                main_flag = 0;
                              }

                              printf("ind\n");
                          }

						  break;

					  case PLUS:
					  	  /* Plus token "+" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
                          }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
                          }

						  printf("add\n");

						  break;

					  case MINUS:
					  	  /* Minus token "-" */

					  	  if((root->lnode != NULL))
                          {
						    code_recur(root->lnode);

						    node_flag = 0;

						    if(main_flag != 0)
                            {
                              printf("ind\n");

                              main_flag = 0;
						    }

						    code_recur(root->rnode);

						    node_flag = 0;

						    if(main_flag != 0)
                            {
                              printf("ind\n");

                              main_flag = 0;
						    }

						    printf("sub\n");
					  	  }

					  	  else
                          {
                            code_recur(root->lnode);

                            node_flag = 0;

                            if(main_flag != 0)
                            {
                                printf("ind\n");

                                main_flag = 0;
                            }

                            code_recur(root->rnode);

                            node_flag = 0;

                            if(main_flag != 0)
                            {
                                printf("ind\n");

                                main_flag = 0;
                            }

                            printf("neg\n");
					  	  }
						  break;

					  case DIV:
					  	  /* Divide token "/" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("div\n");

						  break;

					  case STAR:
					  	  /* multiply token "*" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("mul\n");

						  break;

					  case AND:
					  	  /* And token "&&" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("and\n");

						  break;

					  case OR:
					  	  /* Or token "||" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("or\n");

						  break;

					  case NOT:
					  	  /* Not token "!" */
						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("not\n");

						  break;

					  case GRTR:
					  	  /* Greater token ">" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("grt\n");

						  break;

					  case LESS:
					  	  /* Less token "<" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("les\n");

						  break;

					  case EQUAL:
					  	  /* Equal token "==" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("equ\n");

						  break;

					  case NOT_EQ:
					  	  /* Not equal token "!=" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("neq\n");

						  break;

					  case LESS_EQ:
					  	  /* Less or equal token "<=" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("leq\n");

						  break;

					  case GRTR_EQ:
					  	  /* Greater or equal token ">=" */
						  code_recur(root->lnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  code_recur(root->rnode);

						  node_flag = 0;

						  if(main_flag != 0)
                          {
                            printf("ind\n");

                            main_flag = 0;
						  }

						  printf("geq\n");

						  break;

					  default:
						code_recur(root->lnode);
						code_recur(root->rnode);
						break;
					}
					break;

				case TN_WHILE:
					/* While case */

					first_label = ++label_count;

					second_label = ++label_count;

					last_label_count = second_label;

					printf("label_%d:\n",first_label);

					code_recur(root->lnode);

					node_flag = 0;

					if(main_flag != 0)
                    {
                        main_flag = 0;

                        printf("ind\n");
					}

					printf("fjp label_%d\n",second_label);

					code_recur(root->rnode);

					printf("ujp label_%d\n",first_label);

					printf("label_%d:\n",second_label);

					break;

				case TN_DOWHILE:
					/* Do-While case */

					first_label = ++label_count;

					second_label = ++label_count;

					last_label_count = second_label;

					printf("label_%d:\n",first_label);

					code_recur(root->rnode);

					code_recur(root->lnode);

					node_flag = 0;

					if(main_flag != 0)
                    {
                        main_flag = 0;

                        printf("ind\n");
					}

					printf("fjp label_%d\n",second_label);

					printf("ujp label_%d\n",first_label);

					printf("label_%d:\n",second_label);

					break;

				case TN_LABEL:

				    printf("dpl\n");

					code_recur(root->lnode);

					node_flag = 0;

					if(main_flag != 0)
                    {
                        main_flag = 0;

                        printf("ind\n");
					}

					printf("equ\n");

					printf("fjp end_case%d_%d\n",switch_label_count,++case_count);

					code_recur(root->rnode);

					break;

				default:
					code_recur(root->lnode);
					code_recur(root->rnode);
			}
			break;

		case NONE_T:
			printf("Error: Unknown node type!\n");
			exit(FAILURE);
    }

    return SUCCESS;
}


void print_symbol_table(treenode *root)
{
	printf("---------------------------------------\n");
	printf("Showing the Symbol Table:\n");
}
