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
			if(!p->child[(unsigned char)buf[j]]) p->child[(unsigned char)buf[j]] = malloc(sizeof(struct node));
			p = p->child[(unsigned char)buf[j]];
		}
		p->len = len;
	}

	static struct node *queue[10000];
	queue[0] = &root;
	for(int s = 0, e = 1; s != e;){
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
			++e;
			if(e == sizeof queue / sizeof queue[0]) e = 0;
		}
		++s;
		if(s == sizeof queue / sizeof queue[0]) s = 0;
	}

	static char out[100000000];
	struct node *p = &root;
	for(int i = 0, cnt = 0;; ++i){
		char ch = getchar();
		if(ch == EOF) break;
		for(;;p = p->child[0]){
			if(!p){
				p = &root;
				break;
			}
			if(p->child[(unsigned char)ch]){
				p = p->child[(unsigned char)ch];
				break;
			}
		}
		if(p->len){
			char buf[100] = {};
			if(!p->label){
				p->label = ++cnt;
				sprintf(buf, "( %d )", p->label);
			}else{
				sprintf(buf, "(%d)", p->label);
			}
			strcpy(out + i - p->len + 1, buf);
			i += strlen(buf) - p->len;
			p = &root;
		}else out[i] = ch;
	}
	puts(out);

	static struct node *stack[10000];
       	stack[0] = &root;
	for(int n = 1; n;){
		struct node *tmp = stack[--n];
		for(int i = 1; i < 256; ++i) if(tmp->child[i]) stack[n++] = tmp->child[i];
		if(tmp != &root) free(tmp);
	}
}
