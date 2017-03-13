#define  _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include "zmq.h"
#include <windows.h>


typedef struct node{
	int choise;
	double key;
	double frid;
	float balance;
	float sum;
	node* left;
	node* right;
}node;

struct node *root=0;


void insert(double key, struct node **leaf)
{
	if (*leaf == 0)
	{
		*leaf = (struct node*) malloc(sizeof(struct node));
		(*leaf)->key = key;
		(*leaf)->balance = 0;
		(*leaf)->left = 0;
		(*leaf)->right = 0;
	}
	else if (key < (*leaf)->key)
	{
		insert(key, &(*leaf)->left);
	}
	else if (key >(*leaf)->key)
	{
		insert(key, &(*leaf)->right);
	}
}
struct node *search(double z, struct node *leaf)
{
	if (leaf != 0)
	{
		if (z == leaf->key)
		{
			return leaf;
		}
		else if (z<leaf->key)
		{
			return search(z, leaf->left);
		}
		else
		{
			return search(z, leaf->right);
		}
	}
	else return 0;
}

void add_money(float money, double key)
{
	struct node *leaf = search(key, root);
	leaf->balance += money;
}

void min_money(float money, double key)
{
	struct node *leaf = search(key, root);
	if (leaf->balance < money)
	{
		printf("Are you broken?\n");
	}
	else{
	leaf->balance -= money;}
}
void send_client(double key)
{

	int d;
	 node *leaf = search(key, root);
	 node x;
	 x.key = leaf->key;
	 x.balance = leaf->balance;
	 x.left = 0;
	 x.right = 0;
	void* contex = zmq_ctx_new();
	void* serSocket = zmq_socket(contex, ZMQ_REQ);
	char  string[30];
	printf("enter last digit of ip address ");
	scanf("%d", &d);
	sprintf(string, "tcp://127.0.0.%d:4400\n",d);
	zmq_connect(serSocket, string); zmq_msg_t reply;
	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, serSocket, 0);
	size_t repSize = zmq_msg_size(&reply);
	//printf("Received: - %d %s\n", repSize, zmq_msg_data(&reply));
	zmq_msg_close(&reply);
	zmq_msg_t zmqMess;
	zmq_msg_init_size(&zmqMess, sizeof(node));
	memcpy(zmq_msg_data(&zmqMess), &x, sizeof(node));
	int send = zmq_msg_send(&zmqMess, serSocket, 0);
	printf("mess data %f",x.key);
	zmq_msg_close(&zmqMess);
	Sleep(5000);
}
void send_sum(float sum, double cur_id, double friend_id)
{
	 node *leaf = search(cur_id, root);
		if (leaf->balance >= sum)
		{
			min_money(sum, cur_id);
			add_money(sum, friend_id);
		}
		else
			printf("Are you broken?\n");
	}

void money(double key)
{
	struct node *leaf = search(key, root);
	printf("balance = %f\n", leaf->balance);
}

int main(int argc, char const *argv[])
{

	int k=0;
	float sum=0;
	int d;
    char  string[30];
	printf("enter last digit of ip address ");
	scanf("%d", &d);
	sprintf(string, "tcp://127.0.0.%d:4400\n", d);
	printf("%s", string);
	void* context = zmq_ctx_new();
	void* serverSocket = zmq_socket(context, ZMQ_REP);
	zmq_bind(serverSocket, string);
	printf("Starting...\n");
		zmq_msg_t message;
		zmq_msg_init(&message);
		zmq_msg_recv(&message, serverSocket, 0);
		node *m = (node *)zmq_msg_data(&message);
		printf("id: %f  balance: %f \n", m->key, m->balance);
		insert(m->key, &root);
		zmq_msg_close(&message);
		Sleep(1000);
		zmq_msg_t reply;
		zmq_msg_init_size(&reply, strlen("ok") + 1);
		memcpy(zmq_msg_data(&reply), "ok\0", 3);
		zmq_msg_send(&reply, serverSocket, 0);
		zmq_msg_close(&reply);
		while (1)
		{
		zmq_msg_t mess;
		zmq_msg_init(&mess);
		zmq_msg_recv(&mess, serverSocket, 0);
		node *md = (node *)zmq_msg_data(&mess);
			if (!search(md->key,root))
			{
				insert(md->key, &root);
				printf("new client added\n");
				printf("%lf", md->key);
			}
			zmq_msg_close(&mess);
		switch (md->choise)
		{
		case NULL:
			{
			break;
		}
		case 1:
		{
				  printf("id %f\n", md->key);
				  add_money(md->sum, md->key);
				  money(md->key);
				  printf("success\n");
				  break;
		}
		case 2:
		{		  
				  printf("id %f\n", md->key);
				  min_money(md->sum, md->key);
				  money(md->key);
				  printf("success\n");
				  break;
		}
		case 3:
		{  
				  printf("id %f\n", md->key);
				  send_sum(md->sum, md->key, md->frid);
				  money(md->key);
				  printf("id %f\n", md->frid);
				  money(md->frid);
				  printf("success\n");
				  break;
		}
		case 4:
		{
				  printf("%f", md->key);
				  send_client(md->key);
				  break;
		}
		case -1:
		{
				   zmq_close(serverSocket);
				   zmq_ctx_destroy(context);

				   return 0;
		}
		}
			zmq_msg_t reply;
			zmq_msg_init_size(&reply, strlen("ok") + 1);
			memcpy(zmq_msg_data(&reply), "ok\0", 3);
		    zmq_msg_send(&reply, serverSocket, 0);
			Sleep(1000);
	}
}