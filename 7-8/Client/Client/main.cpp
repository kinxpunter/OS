#define  _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "zmq.h"
#include <Windows.h>
typedef struct node{
	int choise;
	double key;
	double frid;
	float balance;
	float sum;
	node* left;
	node* right;
};



int main(int argc, char const *argv[])
{
	srand(time(0));
	double id = (double)rand();
	int i;
	int d;
	char  string[30];
	printf("enter last digit of ip address ");
	scanf("%d", &d);
	sprintf(string, "tcp://127.0.0.%d:4400\n", d);
	void* context = zmq_ctx_new();
	printf("Publisher %d Starting...\n", id);
	float sum;
	void* publishSocket = zmq_socket(context, ZMQ_REQ);
	zmq_connect(publishSocket, string);
	int count = 0;
	int choise;
		node md;
		md.key = id;
		md.balance = 0;
		md.left = 0;
		md.right = 0;
			zmq_msg_t zmqMessage;
			zmq_msg_init_size(&zmqMessage, sizeof(node));
			memcpy(zmq_msg_data(&zmqMessage), &md, sizeof(node));
			printf("Sending: - %d\n", count);
			int send = zmq_msg_send(&zmqMessage, publishSocket, 0);
			zmq_msg_close(&zmqMessage);
			Sleep(1000);
			zmq_msg_t reply;
			zmq_msg_init(&reply);
			zmq_msg_recv(&reply, publishSocket, 0);
			size_t repSize = zmq_msg_size(&reply);
			printf("Received: - %d %s\n", repSize, zmq_msg_data(&reply));
			zmq_msg_close(&reply);
			while (1){
				zmq_msg_t reply;
				zmq_msg_init(&reply);
				zmq_msg_recv(&reply, publishSocket, 0);
			    size_t repSize = zmq_msg_size(&reply);
				//printf("Received: - %d %s\n", repSize, zmq_msg_data(&reply));
				zmq_msg_close(&reply);
				printf("enter action\n");
				scanf_s("%d", &choise);
				switch (choise)
				{
				case 1:
				{
						  printf("enter sum ");
						  scanf_s("%f", &sum);
						  md.choise = 1;
						  md.sum = sum;
						  break;
				}
				case 2:
				{
						  printf("enter sum ");
						  scanf_s("%f", &sum);
						  md.choise = 2;
						  md.sum = sum;
						  break;

				}
				case 3:
				{
						  double k;
						  printf("enter friend ");
						  scanf_s("%lf", &k);
						  printf("enter sum ");
						  scanf_s("%f", &sum);
						  md.choise = 3;
						  md.frid = k;
						  md.sum = sum;
						  break;
				}
				case 4:
				{
						  md.key = id;
						  md.choise = 4;
						  break;
				}
				case -1:
				{
						   md.choise = -1;
						   zmq_msg_t zmqMess;
						   zmq_msg_init_size(&zmqMess, sizeof(node));
						   memcpy(zmq_msg_data(&zmqMess), &md, sizeof(node));
						   int send = zmq_msg_send(&zmqMess, publishSocket, 0);
						   zmq_msg_close(&zmqMess);
						   zmq_close(publishSocket);
						   zmq_ctx_destroy(context);
						   return 0;
				}
				}
				zmq_msg_t zmqMess;
				zmq_msg_init_size(&zmqMess, sizeof(node));
				memcpy(zmq_msg_data(&zmqMess), &md, sizeof(node));
				int send = zmq_msg_send(&zmqMess, publishSocket, 0);
				zmq_msg_close(&zmqMess);
				Sleep(100);
			}
}



