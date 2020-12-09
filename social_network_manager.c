#include<stdio.h>
#include<stdlib.h>
#define N 8000
#define M 3

typedef struct _hash_elem{
	struct _hash_elem *next ,*prev;
	char *entity;
	struct _rel_a_da *invia, *riceve;
}entity_t;

typedef struct _rel_a_da{
	char *key;
	struct _rel_a_da *next,*prev;
	struct _node *istanze;
	int max;
}relazione_inhash_t;

typedef struct _rel_report{
	char *key;
	int max;
	struct _rel_report *next,*prev;
	struct _node *riceventi;
}relazione_t;

typedef struct _node{
	char color;
	struct _hash_elem *key;
	struct _node *left, *right, *parent;
}node_t;

node_t* nil;

//	FUNCTIONS FOR HASH TABLES
int hash_function(char*,int);
int hash_insert(entity_t*,int,char*);
entity_t* hash_search(entity_t*,int,int,char*);
int hash_delete(entity_t*,int,char*);
entity_t* rehash(entity_t*,int);
void hash_print(entity_t*,int);
//	END OF FUNCTIONS FOR HASH TABLES

//	FUNCTIONS FOR RED-BLACK TREES
node_t* rb_makeit_node(entity_t*);
node_t* rb_left_rotate(node_t*,node_t*);
node_t* rb_right_rotate(node_t*,node_t*);
node_t* rb_insert(node_t*,node_t*);
node_t* rb_insert_special(node_t*,node_t*,int*);
node_t* rb_insert_fixup(node_t*,node_t*);
node_t* rb_delete(node_t*,node_t*);
node_t* rb_delete_fixup(node_t*,node_t*);
node_t* rb_successore(node_t*);
node_t* rb_predecessore(node_t*);
node_t* rb_search(node_t*,entity_t*);
node_t* rb_search_and_destroy(node_t*,entity_t*);
node_t* rb_successor_fixup(node_t*,node_t*);
node_t* rb_successor_inverse_fixup(node_t*, node_t*);
node_t* rb_max(node_t*);
void rb_delete_tree(node_t*);
node_t* rb_heavy_search(node_t*,entity_t*);
//	END OF FUNCTIONS FOR RED-BLACK TREES



relazione_inhash_t* rel_inhash_double_list_insert(relazione_inhash_t*,char*,relazione_inhash_t**);
relazione_inhash_t* rel_inhash_double_list_delete(relazione_inhash_t*,relazione_inhash_t*);
relazione_inhash_t* rel_inhash_double_list_search(relazione_inhash_t*,char*);
relazione_t* rel_report_double_list_insert(relazione_t*,char*,relazione_t**);
relazione_t* rel_report_double_list_delete(relazione_t*,relazione_t*);
relazione_t* rel_report_double_list_search(relazione_t*,char*);

node_t* ric_insert_or_increment_and_sort(relazione_t*,entity_t*,relazione_inhash_t*);
node_t* ric_heavy_delete(relazione_t*,entity_t*,int*);
node_t* ric_delete_or_lower(relazione_t*,entity_t*,entity_t*,int);
node_t* ric_update(relazione_t*,entity_t*,int);

relazione_t* addrel(relazione_t*,entity_t*,int,char*,char*,char*);
relazione_t* delrel(relazione_t*,entity_t*,int,char*,char*,char*);
relazione_t* delent(relazione_t*,entity_t*,int,char*);
relazione_t* delent_helper(relazione_t*,entity_t*,entity_t*,int);
void rb_delent_walk(node_t*,relazione_t*,entity_t*,entity_t*,int);
void delent_riceventi(node_t*,entity_t*,char*);
void report(relazione_t*);
void report_assist(node_t*);

int dic_order(char*,char*);

int main(int argc, char * argv[]){
	
	// VARIABLES
	int i,old_cont,cont,fin,n,dim;
	char *str = NULL, * comand = NULL, * arg1 = NULL, * arg2 = NULL , * arg3 = NULL;
	char str1[] = "addent",str2[] = "delent",str3[] = "addrel",str4[] = "delrel",str5[] = "report",str6[] = "end";
	size_t line_buf_size = 0;
	ssize_t line_size;
	entity_t *hash_table;
	relazione_t *head_rel_report = NULL;
	FILE * fp;
	
	if(!(nil = (node_t*)malloc(sizeof(node_t))))
		return 0;
	nil->key = NULL;
	nil->color = 'B';
	
	fin = 0;
	dim = N;
	n = 0;
	
	hash_table = (entity_t*)calloc(dim,sizeof(entity_t));
	
	//fp = fopen("100k.in.txt","r");
	//if(!fp)
	//	return 0;
	//ACQUISITION CYCLE
	do{
		
		line_size = getline(&str,&line_buf_size,stdin);
		
		for(cont=0,old_cont=0;(str[cont] != ' ') && (str[cont] != '\n');cont++)
			;
		if(!(comand = (char*)malloc(sizeof(char)*(cont+1))))
			return 0;
		for(i=0;(str[i] != ' ') && (str[i] != '\n');i++)
			comand[i] = str[i];
		comand[i] = '\0';
		
		//CASO ADDENT
		
		if(dic_order(comand,str1) == 2){
			cont += 2;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg1 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg1[i-old_cont] = str[i];
			arg1[i-old_cont] = '\0';
			n += hash_insert(hash_table,dim,arg1);
			free(arg1);
			//COMPLETA
		}
		
		// CASO DELENT
		
		else if(dic_order(comand,str2) == 2){
			cont += 2;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg1 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg1[i-old_cont] = str[i];
			arg1[i-old_cont] = '\0';
			head_rel_report = delent(head_rel_report, hash_table, dim, arg1);
			free(arg1);
			// COMPLETA
		}
		
		//CASO ADDREL
		
		else if(dic_order(comand,str3) == 2){
			cont += 2;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg1 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg1[i-old_cont] = str[i];
			arg1[i-old_cont] = '\0';
			cont += 3;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg2 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg2[i-old_cont] = str[i];
			arg2[i-old_cont] = '\0';
			cont += 3;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg3 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg3[i-old_cont] = str[i];
			arg3[i-old_cont] = '\0';
			head_rel_report = addrel(head_rel_report, hash_table, dim, arg1, arg2, arg3);
			free(arg1);
			free(arg2);
			free(arg3);
			//COMPLETA
		}
		
		//CASO DELREL
		
		else if(dic_order(comand,str4) == 2){
			cont += 2;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg1 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg1[i-old_cont] = str[i];
			arg1[i-old_cont] = '\0';
			cont += 3;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg2 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg2[i-old_cont] = str[i];
			arg2[i-old_cont] = '\0';
			cont += 3;
			old_cont = cont;
			for(;str[cont] != '"';cont++)
				;
			if(!(arg3 = (char*)malloc(sizeof(char)*(cont-old_cont+1))))
				return 0;
			for(i=old_cont;str[i] != '"';i++)
				arg3[i-old_cont] = str[i];
			arg3[i-old_cont] = '\0';
			head_rel_report = delrel(head_rel_report, hash_table, dim, arg1, arg2, arg3);
			free(arg1);
			free(arg2);
			free(arg3);
			//COMPLETA
		}
		
		
		else if(dic_order(comand,str5) == 2){
			if(head_rel_report == NULL)
				printf("none\n");
			else{
				report(head_rel_report);
				printf("\n");
			}
			//COMPLETO
		}
		
		else if(dic_order(comand,str6) == 2){
			fin = 1;
			//COMPLETO
		}
		
		if((float)n > (1*(float)dim)){
			//hash_table = rehash(hash_table,dim);
			//dim *= 2;
			return 0;
		}
		
		free(comand);
		
	}while(fin == 0);
	
	//fclose(fp);
	return 0;
}


relazione_t* addrel(relazione_t* head,entity_t* hash_table,int dim,char* ist_da,char* ist_a,char* id_rel){
	
	entity_t *da,*a;
	relazione_inhash_t *rel_inhash_da,*rel_inhash_a;
	node_t *istanza_da,*istanza_a, *temp;
	relazione_t *rel_report;
	int key, fin = 0;
	
	key = hash_function(ist_da, dim);
	da = hash_search(hash_table, dim, key, ist_da);
	if(da == NULL)
		return head;
	key = hash_function(ist_a, dim);
	a = hash_search(hash_table, dim, key, ist_a);
	if(a == NULL)
		return head;
	
	
	rel_inhash_da = rel_inhash_double_list_search(da->invia, id_rel);
	rel_inhash_a = rel_inhash_double_list_search(a->riceve, id_rel);
	if(rel_inhash_a != NULL && rel_inhash_da != NULL){
		temp = rb_makeit_node(a);
		rel_inhash_da->istanze = rb_insert_special(rel_inhash_da->istanze,temp,&fin);
		if(fin == 1){
			free(temp);
			return head;
		}
		temp = rb_makeit_node(da);
		rel_inhash_a->istanze = rb_insert_special(rel_inhash_a->istanze,temp,&fin);
		rel_report = rel_report_double_list_search(head, id_rel);
		if(rel_report == NULL){
			head = rel_report_double_list_insert(head, id_rel,&rel_report);
		}
		rel_report->riceventi = ric_insert_or_increment_and_sort(rel_report, a, rel_inhash_a);
		rel_inhash_da->max += 1;
		rel_inhash_a->max += 1;
		return head;		
	}
	else if(rel_inhash_a == NULL && rel_inhash_da != NULL){
		a->riceve = rel_inhash_double_list_insert(a->riceve, id_rel,&rel_inhash_a);
	}
	else if(rel_inhash_a != NULL && rel_inhash_da == NULL){
		da->invia = rel_inhash_double_list_insert(da->invia, id_rel, &rel_inhash_da);
	}
	else if(rel_inhash_a == NULL && rel_inhash_da == NULL){
		a->riceve = rel_inhash_double_list_insert(a->riceve, id_rel, &rel_inhash_a);
		da->invia = rel_inhash_double_list_insert(da->invia, id_rel, &rel_inhash_da);
	}
	rel_report = rel_report_double_list_search(head, id_rel);
	if(rel_report == NULL){
		head = rel_report_double_list_insert(head, id_rel, &rel_report);
	}
	rel_report->riceventi = ric_insert_or_increment_and_sort(rel_report, a, rel_inhash_a);
	rel_inhash_da->istanze = rb_insert(rel_inhash_da->istanze, rb_makeit_node(a));
	rel_inhash_a->istanze = rb_insert(rel_inhash_a->istanze, rb_makeit_node(da));
	
	rel_inhash_da->max += 1;
	rel_inhash_a->max += 1;
	
	return head;
	
}
relazione_t* delrel(relazione_t* head,entity_t* hash_table,int dim,char* ist_da,char* ist_a,char* id_rel){
	entity_t *da,*a;
	relazione_inhash_t *rel_inhash_da,*rel_inhash_a;
	node_t *istanza_da,*istanza_a;
	relazione_t *rel_report;
	int key;

	key = hash_function(ist_da, dim);
	da = hash_search(hash_table, dim, key, ist_da);

	key = hash_function(ist_a, dim);
	a = hash_search(hash_table, dim, key, ist_a);
	if(da == NULL || a == NULL)
		return head;
	rel_inhash_da = rel_inhash_double_list_search(da->invia, id_rel);
	rel_inhash_a = rel_inhash_double_list_search(a->riceve, id_rel);
	if(rel_inhash_a != NULL && rel_inhash_da != NULL){
		istanza_da = rb_search(rel_inhash_da->istanze,a);
		istanza_a = rb_search(rel_inhash_a->istanze,da);
		if(istanza_a != NULL && istanza_da != NULL){
			rel_inhash_da->max -= 1;
			rel_inhash_a->max -= 1;
			rel_report = rel_report_double_list_search(head, id_rel);
			rel_inhash_da->istanze = rb_delete(rel_inhash_da->istanze, istanza_da);
			rel_inhash_a->istanze = rb_delete(rel_inhash_a->istanze, istanza_a);
			rel_report->riceventi = ric_delete_or_lower(rel_report, a,hash_table,dim);
			if(rel_inhash_da->istanze == nil)
				da->invia = rel_inhash_double_list_delete(da->invia, rel_inhash_da);
			if(rel_inhash_a->istanze == nil)
				a->riceve = rel_inhash_double_list_delete(a->riceve, rel_inhash_a);
			if(rel_report->max == 0){
				rb_delete_tree(rel_report->riceventi);
				head = rel_report_double_list_delete(head, rel_report);
			}
		}
	}
	return head;
}
void report(relazione_t* head){
	
	relazione_t *ciclo;
	node_t *maxer, *pred;
	int max;
	
	ciclo = head;
	
	for(; ciclo != NULL; ciclo = ciclo->next){
		fputs("\"",stdout);
		fputs(ciclo->key,stdout);
		fputs("\" ",stdout);
		//printf("\"%s\" ",ciclo->key);
		max = ciclo->max;
		report_assist(ciclo->riceventi);
		printf("%d; ",max);
	}
	return;
}
void report_assist(node_t * root){
	if(root == nil)
		return;
	report_assist(root->right);
	fputs("\"",stdout);
	fputs(root->key->entity,stdout);
	fputs("\" ",stdout);
	//printf("\"%s\" ",root->key->entity);
	report_assist(root->left);
	return;
}
relazione_t* delent(relazione_t* head,entity_t* hash_table,int dim,char* entita){
	entity_t *ent;
	relazione_t *temp, *jonny;
	relazione_inhash_t *temp2;
	int fin = 0,key;
	
	key = hash_function(entita, dim);
	ent = hash_search(hash_table, dim, key, entita);
	if(ent == NULL)
		return head;
	head = delent_helper(head, ent, hash_table,dim);
	
	temp2 = ent->invia;
	for(;temp2 != NULL && temp2->next != NULL;){
		rb_delete_tree(temp2->istanze);
		temp2 = temp2->next;
		free(temp2->prev->key);
		free(temp2->prev);
	}
	if(temp2 != NULL){
		rb_delete_tree(temp2->istanze);
		free(temp2->key);
		free(temp2);
	}
	ent->invia = NULL;
	
	temp = head;
	for(;temp != NULL && temp->next != NULL;){
		temp->riceventi = ric_heavy_delete(temp, ent, &fin);
		if(fin == 1){
			temp->riceventi = ric_update(temp,hash_table,dim);
			fin = 0;
		}
		if(temp->max == 0){
			rb_delete_tree(temp->riceventi);
			jonny = temp;
			temp = temp->next;
			head = rel_report_double_list_delete(head, jonny);
		}
		else
			temp = temp->next;
	}
	if(temp != NULL){
		temp->riceventi = ric_heavy_delete(temp, ent, &fin);
		if(fin == 1){
			temp->riceventi = ric_update(temp,hash_table,dim);
			fin = 0;
		}
		if(temp->max == 0){
			rb_delete_tree(temp->riceventi);
			head = rel_report_double_list_delete(head, temp);
		}
	}
	
	temp2 = ent->riceve;
	for(;temp2 != NULL && temp2->next != NULL;){
		delent_riceventi(temp2->istanze, ent, temp2->key);
		rb_delete_tree(temp2->istanze);
		temp2 = temp2->next;
		free(temp2->prev->key);
		free(temp2->prev);
	}
	if(temp2 != NULL){
		delent_riceventi(temp2->istanze, ent, temp2->key);
		rb_delete_tree(temp2->istanze);
		free(temp2->key);
		free(temp2);
	}
	
	
	//	END OF SOTTOFUNZIONE PER LA HEAVY DELETE
	
	hash_delete(hash_table,dim,ent->entity);
	
	return head;
}
relazione_t* delent_helper(relazione_t* head, entity_t* entita,entity_t* hash_table,int dim){
	relazione_t *temp_report,*temp;
	relazione_inhash_t *temp_inhash;
	
	temp_report = head;
	temp_inhash = entita->invia;
	if(temp_report == NULL || temp_inhash == NULL)
		return head;
	while(temp_inhash != NULL && temp_report != NULL){
		switch (dic_order(temp_inhash->key, temp_report->key)) {
			case 2:
				rb_delent_walk(temp_inhash->istanze,temp_report,entita,hash_table,dim);
				if(temp_report->max == 0){
					rb_delete_tree(temp_report->riceventi);
					temp = temp_report;
					temp_report = temp_report->next;
					head = rel_report_double_list_delete(head, temp);
				}
				else
					temp_report = temp_report->next;
				temp_inhash = temp_inhash->next;
				break;
			case 0:
				temp_report = temp_report->next;
				break;
			case 1:
				temp_inhash = temp_inhash->next;
				break;
		}
	}
	return head;
}
void rb_delent_walk(node_t* head_ist, relazione_t* root_report, entity_t* entita,entity_t* hash_table,int dim){
	
	relazione_inhash_t *rel_inhash;
	node_t *ist_temp;
	
	if(head_ist == nil)
		return;
	rb_delent_walk(head_ist->left, root_report, entita, hash_table, dim);
	rb_delent_walk(head_ist->right, root_report, entita, hash_table, dim);
	if(head_ist->key == entita)
		return;
	rel_inhash = rel_inhash_double_list_search(head_ist->key->riceve, root_report->key);
	ist_temp = rb_search(rel_inhash->istanze, entita);
	rel_inhash->istanze = rb_delete(rel_inhash->istanze, ist_temp);
	rel_inhash->max -= 1;
	root_report->riceventi = ric_delete_or_lower(root_report, head_ist->key,hash_table,dim);
	if(rel_inhash->istanze == nil){
		head_ist->key->riceve = rel_inhash_double_list_delete(head_ist->key->riceve, rel_inhash);
	}
	
	return ;
}
void delent_riceventi(node_t* head_ist,entity_t* ent_mia, char* id_rel){
	relazione_inhash_t *rel_inhash;
	node_t *ist_temp;
	
	if(head_ist == nil)
		return;
	
	delent_riceventi(head_ist->left,ent_mia, id_rel);
	delent_riceventi(head_ist->right,ent_mia, id_rel);
	
	if(head_ist->key == ent_mia)
		return;
	rel_inhash = rel_inhash_double_list_search(head_ist->key->invia, id_rel);
	ist_temp = rb_search(rel_inhash->istanze, ent_mia);
	rel_inhash->istanze = rb_delete(rel_inhash->istanze, ist_temp);
	rel_inhash->max -= 1;
	if(rel_inhash->istanze == nil){
		head_ist->key->invia = rel_inhash_double_list_delete(head_ist->key->invia, rel_inhash);
	}
	
	return ;
}
// FUNCTIONS FOR RECIEVERS LIST IMPLEMENTATION
node_t* ric_insert_or_increment_and_sort(relazione_t* relazione,entity_t* enti_ricevente, relazione_inhash_t* rel){
	int n;

	n = rel->max;
	if(relazione->max == n){
		rb_delete_tree(relazione->riceventi);
		relazione->riceventi = nil;
		relazione->riceventi = rb_insert(relazione->riceventi, rb_makeit_node(enti_ricevente));
		relazione->max +=1;
	}
	else if (relazione->max == n+1){
		relazione->riceventi = rb_insert(relazione->riceventi, rb_makeit_node(enti_ricevente));
	}
	
	return relazione->riceventi;
}
node_t* ric_delete_or_lower(relazione_t* relazione,entity_t* enti_ricevente,entity_t* hash_table,int dim){
	
	int n;
	relazione_inhash_t *rel;
	node_t *nodo;
	
	rel = rel_inhash_double_list_search(enti_ricevente->riceve, relazione->key);
	n = rel->max;
	if(relazione->max == n+1){
		nodo = rb_search(relazione->riceventi, enti_ricevente);
		relazione->riceventi = rb_delete(relazione->riceventi, nodo);
		if(relazione->riceventi == nil){
			relazione->max -= 1;
			if(relazione->max != 0){
				relazione->riceventi = ric_update(relazione,hash_table,dim);
			}
		}
	}
	return relazione->riceventi;
}
node_t* ric_heavy_delete(relazione_t* relazione,entity_t* enti_ricevente,int *fin){
	int n;
	relazione_inhash_t *rel;
	node_t* ricevente;
	
	rel = rel_inhash_double_list_search(enti_ricevente->riceve, relazione->key);
	if(rel != NULL){
		n = rel->max;
		if(relazione->max == n){
			ricevente = rb_search(relazione->riceventi, enti_ricevente);
			relazione->riceventi = rb_delete(relazione->riceventi, ricevente);
			rel->max = 0;
			if(relazione->riceventi == nil){
				relazione->max -= 1;
				if(relazione->max != 0){
					*fin = 1;
				}
			}
		}
	}
	return relazione->riceventi;
}
node_t* ric_update(relazione_t* relazione, entity_t* hash_table, int dim){
	int i;
	entity_t *temp;
	relazione_inhash_t *found;
	
	relazione->max = 0;
	relazione->riceventi = nil;
	
	for(i=0; i<dim; i++){
		if(hash_table[i].next != NULL){
			temp = hash_table[i].next;
			for(; temp != NULL; temp = temp->next){
				found = rel_inhash_double_list_search(temp->riceve, relazione->key);
				if(found != NULL){
					if(found->max > relazione->max){
						rb_delete_tree(relazione->riceventi);
						relazione->riceventi = nil;
						relazione->max = found->max;
						relazione->riceventi = rb_insert(relazione->riceventi, rb_makeit_node(temp));
					}
					else if(found->max == relazione->max){
						relazione->riceventi = rb_insert(relazione->riceventi, rb_makeit_node(temp));
					}
				}
			}
		}
	}
	return relazione->riceventi;
}
// END OF FUNCTIONS FOR RECIEVERS LIST IMPLEMENTATION

int dic_order(char* str1,char* str2){
	int i;
	for(i=0;str1[i] != '\0' && str2[i] != '\0';i++){
		if(str1[i] < str2[i])
			return 1;
		else if(str1[i] > str2[i])
			return 0;
	}
	if(str1[i] == '\0' && str2[i] != '\0')
		return 1;
	if(str2[i] == '\0' && str1[i] != '\0')
		return 0;
	return 2;
}
int hash_function(char* str,int dim){
	
	int key,i,old_p = M;
	long int sum,p;
	for(sum = 0, p = 1, i = 0; str[i] != '\0'; i++){
		sum += (str[i] * p)+(i*p);
		p *= old_p;
	}
	key = sum % dim;
	return key;
}
int hash_insert(entity_t* hash_table, int dim, char* str){
	
	entity_t *new;
	int i,key;
	
	if(str == NULL)
		return 0;

	key = hash_function(str,dim);
	
	if(hash_search(hash_table,dim,key,str) == NULL){
		
		new = (entity_t*)malloc(sizeof(entity_t));
		for(i=0; str[i] != '\0'; i++)
			;
		new->entity = (char*)malloc(sizeof(char)*(i+1));
		for(i=0; str[i] != '\0';i++)
			new->entity[i] = str[i];
		new->entity[i] = '\0';
		new->prev = NULL;
		new->invia = new->riceve = NULL;
		
		if(hash_table[key].next == NULL){
			hash_table[key].next = new;
			new->next = NULL;
		}
		else{
			new->next = hash_table[key].next;
			new->next->prev = new;
			hash_table[key].next = new;
		}
		return 1;
	}
	return 0;
}
entity_t* hash_search(entity_t* hash_table,int dim,int key, char* str){
	
	entity_t *temp;
	
	if(hash_table[key].next == NULL)
		return NULL;
	temp = hash_table[key].next;
	while(temp->next != NULL){
		if(dic_order(str, temp->entity) == 2)
			return temp;
		temp = temp->next;
	}
	if(dic_order(str, temp->entity) == 2)
		return temp;
	return NULL;
}
int hash_delete(entity_t* hash_table, int dim,char *str){
	
	int key;
	entity_t *temp;
	
	key = hash_function(str, dim);
	if(hash_table[key].next == NULL)
		return 0;
	temp = hash_table[key].next;
	if(dic_order(str, temp->entity) == 2){
		hash_table[key].next = temp->next;
		if(temp->next != NULL)
			temp->next->prev = NULL;
		free(temp->entity);
		free(temp);
		return 1;
	}
	temp = temp->next;
	while(temp->next != NULL){
		if(dic_order(str, temp->entity) == 2){
			temp->next->prev = temp->prev;
			temp->prev->next = temp->next;
			free(temp->entity);
			free(temp);
			return 1;
		}
		else
			temp = temp->next;
	}
	if(dic_order(str, temp->entity) == 2){
		temp->prev->next = temp->next;
		free(temp->entity);
		free(temp);
		return 1;
	}
	return 0;
}
entity_t* rehash(entity_t* old_hash_table,int old_dim){
	int dim,i;
	entity_t *hash_table, *temp;
	dim = old_dim * 2;
	
	hash_table = (entity_t*)calloc(dim,sizeof(entity_t));
	
	for(i=0; i<old_dim; i++){
		if(old_hash_table[i].next != NULL){
			temp = old_hash_table[i].next;
			while(temp->next != NULL){
				hash_insert(hash_table, dim, temp->entity);
				temp = temp->next;
				free(temp->prev->entity);
				free(temp->prev);
			}
			hash_insert(hash_table, dim, temp->entity);
			free(temp->entity);
			free(temp);
		}
	}
	free(old_hash_table);
	return hash_table;
}
void hash_print(entity_t* hash_table, int dim){
	
	int i;
	entity_t* temp;
	for(i=0; i<dim; i++){
		if(hash_table[i].next != NULL){
			printf("riga %d contenuto: ",i);
			temp = hash_table[i].next;
			while(temp->next != NULL){
				printf("%s ",temp->entity);
				temp = temp->next;
			}
			printf("%s ",temp->entity);
			printf("\n");
		}
	}
	printf("\n\n");
	return;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
relazione_inhash_t* rel_inhash_double_list_insert(relazione_inhash_t* head,char* key,relazione_inhash_t** inserita){
	int fin,i;
	relazione_inhash_t *temp,*new;
	temp = head;
	for(fin = 0;fin == 0 && temp != NULL && temp->next != NULL;){
		switch (dic_order(temp->key, key)) {
			case 2:
				printf("\n\n ERRORE NELLA INSERT DELLA REL INHASH ELEMENTO GIA PRESETE \n\n");
				break;
			case 0:
				fin = 1;
				break;
			case 1:
				temp = temp->next;
				break;
		}
	}
	if(fin == 0 && temp != NULL){
		switch (dic_order(temp->key, key)) {
			case 2:
				printf("\n\n ERRORE NELLA INSERT DELLA REL INHASH ELEMENTO GIA PRESETE \n\n");
				break;
			case 0:
				fin = 1;
				break;
			case 1:
				break;
		}
	}
	
	if(!(new = (relazione_inhash_t*)malloc(sizeof(relazione_inhash_t))))
		return head;
	for(i=0;key[i] != '\0';i++)
		;
	if(!(new->key = (char*)malloc(sizeof(char)*(i+1))))
		return head;
	for(i=0;key[i] != '\0';i++)
		new->key[i] = key[i];
	new->key[i] = '\0';
	new->istanze = nil;
	new->prev = new->next = NULL;
	new->max = 0;
	*inserita = new;
	
	if(head == NULL)
		return new;
	if(fin == 0){
		temp->next = new;
		new->prev = temp;
	}
	else if(fin == 1){
		if(temp->prev != NULL)
			temp->prev->next = new;
		new->prev = temp->prev;
		new->next = temp;
		temp->prev = new;
	}
	if(new->prev == NULL)
		return new;
	return head;
}
relazione_inhash_t* rel_inhash_double_list_delete(relazione_inhash_t* head,relazione_inhash_t* todel){
	relazione_inhash_t *temp;
	if(todel == head){
		if(head->next != NULL)
			head->next->prev = NULL;
		temp = head->next;
		free(todel->key);
		free(todel);
		return temp;
	}
	if(todel->next != NULL)
		todel->next->prev = todel->prev;
	if(todel->prev != NULL)
		todel->prev->next = todel->next;
	free(todel->key);
	free(todel);
	return head;
}
relazione_inhash_t* rel_inhash_double_list_search(relazione_inhash_t* head,char* key){
	relazione_inhash_t* temp;
	temp = head;
	for(;temp != NULL;temp = temp->next){
		if(dic_order(temp->key, key) == 2)
			return temp;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
relazione_t* rel_report_double_list_insert(relazione_t* head,char* key,relazione_t **inserita){
	int fin,i;
	relazione_t *temp,*new;
	temp = head;
	for(fin = 0;fin == 0 && temp != NULL && temp->next != NULL;){
		switch (dic_order(temp->key, key)) {
			case 2:
				printf("\n\n ERRORE NELLA INSERT DELLA REL INHASH ELEMENTO GIA PRESETE \n\n");
				break;
			case 0:
				fin = 1;
				break;
			case 1:
				temp = temp->next;
				break;
		}
	}
	if(fin == 0 && temp != NULL){
		switch (dic_order(temp->key, key)) {
			case 2:
				printf("\n\n ERRORE NELLA INSERT DELLA REL INHASH ELEMENTO GIA PRESETE \n\n");
				break;
			case 0:
				fin = 1;
				break;
			case 1:
				break;
		}
	}
	
	if(!(new = (relazione_t*)malloc(sizeof(relazione_t))))
		return head;
	for(i=0;key[i] != '\0';i++)
		;
	if(!(new->key = (char*)malloc(sizeof(char)*(i+1))))
		return head;
	for(i=0;key[i] != '\0';i++)
		new->key[i] = key[i];
	new->key[i] = '\0';
	new->riceventi = nil;
	new->prev = new->next = NULL;
	new->max = 0;
	*inserita = new;

	if(head == NULL)
		return new;
	if(fin == 0){
		temp->next = new;
		new->prev = temp;
	}
	else if(fin == 1){
		if(temp->prev != NULL)
			temp->prev->next = new;
		new->prev = temp->prev;
		new->next = temp;
		temp->prev = new;
	}
	if(new->prev == NULL)
		return new;
	return head;
}
relazione_t* rel_report_double_list_delete(relazione_t* head,relazione_t* todel){
	relazione_t *temp;
	if(todel == head){
		if(head->next != NULL)
			head->next->prev = NULL;
		temp = head->next;
		free(todel->key);
		free(todel);
		return temp;
	}
	if(todel->next != NULL)
		todel->next->prev = todel->prev;
	if(todel->prev != NULL)
		todel->prev->next = todel->next;
	free(todel->key);
	free(todel);
	return head;
}
relazione_t* rel_report_double_list_search(relazione_t* head,char* key){
	relazione_t* temp;
	temp = head;
	for(;temp != NULL;temp = temp->next){
		if(dic_order(temp->key, key) == 2)
			return temp;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*
node_t* rb_successor_inverse_fixup(node_t* root, node_t* nodo){
	node_t *temp;
	entity_t *temp_ent;
	int  temp_max;
	
	temp = rb_predecessore(nodo);
	for(;(temp != nil && temp->max > nodo->max)||(temp != nil && temp->max == nodo->max && dic_order(temp->key->entity, nodo->key->entity) == 1); temp = rb_predecessore(nodo)){
		
		temp_ent = temp->key;
		temp_max = temp->max;
		temp->key = nodo->key;
		temp->max = nodo->max;
		nodo->key = temp_ent;
		nodo->max = temp_max;
		nodo = temp;
	}
	return root;
}
node_t* rb_successor_fixup(node_t* root, node_t* nodo){
	node_t *temp;
	entity_t *temp_ent;
	int  temp_max;
	
	temp = rb_successore(nodo);
	for(;(temp != nil && temp->max < nodo->max)||(temp != nil && temp->max == nodo->max && dic_order(temp->key->entity, nodo->key->entity) == 0); temp = rb_successore(nodo)){
		
		temp_ent = temp->key;
		temp_max = temp->max;
		temp->key = nodo->key;
		temp->max = nodo->max;
		nodo->key = temp_ent;
		nodo->max = temp_max;
		nodo = temp;
	}
	return root;
}*/
node_t* rb_search_and_destroy(node_t* root,entity_t *key){
	
	
	node_t * temp = nil;
	
	temp = rb_search(root, key);
	if(temp != NULL)
		root = rb_delete(root, temp);
	return root;
}
node_t* rb_insert(node_t* root,node_t* z){
	node_t *y,*x;
	y = nil;
	x = root;
	while(x != nil){
		y = x;
		
		if(dic_order(z->key->entity,x->key->entity) == 0)
			x = x->left;
		else if(dic_order(z->key->entity,x->key->entity) == 2){
			return root;
		}
		else
			x = x->right;
	}
	z->parent = y;
	if(y == nil)
		root = z;
	else if(dic_order(z->key->entity, y->key->entity) == 0)
		y->left = z;
	else
		y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = 'R';
	root = rb_insert_fixup(root,z);
	return root;
}
node_t* rb_insert_special(node_t* root,node_t* z,int* fin){
	node_t *y,*x;
	y = nil;
	x = root;
	while(x != nil){
		y = x;
		
		if(dic_order(z->key->entity,x->key->entity) == 0)
			x = x->left;
		else if(dic_order(z->key->entity,x->key->entity) == 2){
			*fin = 1;
			return root;
		}
		else
			x = x->right;
	}
	z->parent = y;
	if(y == nil)
		root = z;
	else if(dic_order(z->key->entity, y->key->entity) == 0)
		y->left = z;
	else
		y->right = z;
	z->left = nil;
	z->right = nil;
	z->color = 'R';
	root = rb_insert_fixup(root,z);
	return root;
}
node_t* rb_insert_fixup(node_t* root,node_t* z){
	node_t *y,*x;
	if(z == root)
		root->color = 'B';
	else{
		x = z->parent;
		if(x->color == 'R'){
			if(x == x->parent->left){
				y = x->parent->right;
				if(y->color == 'R'){
					x->color = 'B';
					y->color = 'B';
					x->parent->color = 'R';
					root = rb_insert_fixup(root, x->parent);
				}
				else{
					if(z == x->right){
						z = x;
						root = rb_left_rotate(root, z);
						x = z->parent;
					}
					x->color = 'B';
					x->parent->color = 'R';
					root = rb_right_rotate(root, x->parent);
				}
			}
			else{
				y = x->parent->left;
				if(y->color == 'R'){
					x->color = 'B';
					y->color = 'B';
					x->parent->color = 'R';
					root = rb_insert_fixup(root, x->parent);
				}
				else{
					if(z == x->left){
						z = x;
						root = rb_right_rotate(root, z);
						x = z->parent;
					}
					x->color = 'B';
					x->parent->color = 'R';
					root = rb_left_rotate(root, x->parent);
				}
			}
		}
	}
	return root;
}
node_t* rb_delete(node_t *root,node_t* z){
	
	node_t *y,*x;
	if(root != nil){
		if((z->left == nil)||(z->right == nil))
			y = z;
		else
			y = rb_successore(z);
		if(y->left != nil)
			x = y->left;
		else
			x = y->right;
		x->parent = y->parent;
		if(y->parent == nil)
			root = x;
		else if(y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
		if(y != z){
			z->key = y->key;
		}
		if(y->color == 'B'){
			root = rb_delete_fixup(root, x);
		}
		if(y != nil){
			free(y);
		}
	}
	return root;
}
node_t* rb_delete_fixup(node_t* root,node_t* x){
	node_t* w;
	while(x != root && x->color == 'B'){
		if(x == x->parent->left){
			w = x->parent->right;
			if(w->color == 'R'){
				w->color = 'B';
				x->parent->color = 'R';
				root = rb_left_rotate(root,x->parent);
				w = x->parent->right;
			}
			
			if(w->left->color == 'B' && w->right->color == 'B'){
				w->color = 'R';
				x = x->parent;
			}
			else{
				if(w->right->color == 'B'){
					w->left->color = 'B';
					w->color = 'R';
					root = rb_right_rotate(root,w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = 'B';
				w->right->color = 'B';
				root = rb_left_rotate(root,x->parent);
				x = root;
			}
		}
		else{
			w = x->parent->left;
			if(w->color == 'R'){
				w->color = 'B';
				x->parent->color = 'R';
				root = rb_right_rotate(root,x->parent);
				w = x->parent->left;
			}
			
			if(w->left->color == 'B' && w->right->color == 'B'){
				w->color = 'R';
				x = x->parent;
			}
			else{
				if(w->left->color == 'B'){
					w->right->color = 'B';
					w->color = 'R';
					root = rb_left_rotate(root,w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = 'B';
				w->left->color = 'B';
				root = rb_right_rotate(root,x->parent);
				x = root;
			}
		}
	}
	x->color = 'B';
	return root;
}
node_t* rb_search(node_t * root,entity_t * entita){
	if(root == nil)
		return NULL;
	if(dic_order(root->key->entity,entita->entity) == 2)
		return root;
	else if(dic_order(root->key->entity,entita->entity) == 0)
		return rb_search(root->right, entita);
	return rb_search(root->left, entita);
}
node_t* rb_makeit_node(entity_t* entita){
	node_t * node;
	if(!(node = (node_t*)malloc(sizeof(node_t))))
		return NULL;
	node->key = entita;
	node->parent = nil;
	
	return node;
}
node_t* rb_left_rotate(node_t * root,node_t * x){
	node_t * y;
	y = x->right;
	x->right = y->left;
	
	if(y->left != nil)
		y->left->parent = x;
	y->parent = x->parent;
	if(x->parent == nil)
		root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
	return root;
}
node_t* rb_right_rotate(node_t * root,node_t * x){
	node_t * y;
	y = x->left;
	x->left = y->right;
	
	if(y->right != nil)
		y->right->parent = x;
	y->parent = x->parent;
	if(x->parent == nil)
		root = y;
	else if(x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
	return root;
}
node_t* rb_successore(node_t * root){
	node_t * temp;
	temp = root->right;
	if(temp != nil){
		while(temp->left != nil)
			temp = temp->left;
		return temp;
	}
	else{
		temp = root->parent;
		while(temp != nil && root == temp->right){
			root = temp;
			temp = temp->parent;
		}
		return temp;
	}
	return temp;
}
node_t* rb_predecessore(node_t * root){
	node_t * temp;
	temp = root->left;
	if(temp != nil){
		while(temp->right != nil)
			temp = temp->right;
		return temp;
	}
	else{
		temp = root->parent;
		while(temp != nil && root == temp->left){
			root = temp;
			temp = temp->parent;
		}
		return temp;
	}
	return temp;
}
node_t* rb_max(node_t* root){
	if(root == nil)
		return nil;
	while(root->right != nil){
		root = root->right;
	}
	return root;
}
void rb_delete_tree(node_t* node){
	if(node == nil)
		return;
	rb_delete_tree(node->left);
	rb_delete_tree(node->right);
	free(node);
	return;
}
