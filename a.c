#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
	int len;
	struct node *child[256];
	int label;
};

int main(){
	static struct node root;

	for(;;){
		char buf[10000];
		fgets(buf, sizeof buf, stdin);

		if(buf[0] == '\n') break;
		int len = strlen(buf);
		buf[--len] = '\0';

		struct node *p = &root;
		for(int j = 0; j < len; ++j){
			if(!p->child[buf[j]]) p->child[buf[j]] = malloc(sizeof(struct node));
			p = p->child[buf[j]];
		}
		p->len = len;
	}

	static struct node *queue[10000];
	queue[0] = &root;
	for(int s = 0, e = 1; s != e; ++s == sizeof queue / sizeof queue[0] && (s = 0)){
		for(int i = 1; i < 256; ++i) if(queue[s]->child[i]){
			for(struct node *tmp = queue[s]->child[0];; tmp = tmp->child[0]){
				if(!tmp){
					queue[s]->child[i]->child[0] = &root;
					break;
				}
				if(tmp->child[i]){
					queue[s]->child[i]->child[0] = tmp->child[i];
					break;
				}
			}
			queue[e] = queue[s]->child[i];
			if(e + 1 == sizeof queue / sizeof queue[0]) e = 0;
			else ++e;
		}
	}

	static char in[100000000], out[100000000];
	fgets(in, sizeof in, stdin);

	struct node *p = &root;
	for(int i = 0, j = 0, cnt = 0; in[i] != '\n' || (out[j] = '\0'); ++i, ++j){
		for(;;p = p->child[0]){
			if(!p){
				p = &root;
				break;
			}
			if(p->child[in[i]]){
				p = p->child[in[i]];
				break;
			}
		}
		if(p->len){
			if(!p->label) p->label = ++cnt;
			char buf[100] = {};
			sprintf(buf, "( %d )", p->label);
			strcpy(out + j - p->len + 1, buf);
			j += strlen(buf) - p->len;
			p = &root;
		}else out[j] = in[i];
	}

	static struct node *stack[10000];
       	stack[0] = &root;
	for(int n = 1; n;){
		struct node *tmp = stack[--n];
		for(int i = 1; i < 256; ++i) if(tmp->child[i]) stack[n++] = tmp->child[i];
		if(tmp != &root) free(tmp);
	}

	puts(out);
}
