#include<stdio.h>	//For standard things
#include<stdlib.h>	//malloc
#include<string.h>	//memset
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<linux/types.h>
#include<net/ethernet.h>
#include <features.h>
#include <sys/types.h>
#include <netinet/in.h>
#define ETH_ALEN 6

int sock_raw;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;
struct sockaddr_in source,dest;
struct ethdr { //
	unsigned char h_dest[ETH_ALEN];
	unsigned char h_source[ETH_ALEN];
	__be16 h_proto;
}__attribute__((packed));
struct iphr{
	#if __BYTE_ORDER == __LITTLE_ENDIAN
    	unsigned int ihl:4;
    	unsigned int version:4;
	#elif __BYTE_ORDER == __BIG_ENDIAN
    	unsigned int version:4;
    	unsigned int ihl:4;
	#endif
	unsigned int TypeOfService:8;
	unsigned int TotalLength:16;//Total length of the datagram.
	unsigned int Identification:16;
	unsigned int Flags:3;
	unsigned int FragmentOffset:13;
	unsigned int Ttl:8;
	unsigned int Protocol:8;
	unsigned int HeaderChecksum:16;
	unsigned int SourceAddr;
	unsigned int DestAddr;
}__attribute__((packed));
struct tcpheader{
	short int SourcePort;
	short int DestinationPort;
	unsigned int SequenceNumber;
	unsigned int AckNumber;
	unsigned int DataOffset:4;
	unsigned int Reserved:6;
	unsigned int Urg:1;
	unsigned int Ack:1;
	unsigned int Psh:1;
	unsigned int Rst:1;
	unsigned int Syn:1;
	unsigned int Fin:1;
	short int Window;
	short int Checksum;
	short int UrgentPointer;
}__attribute__((packed));
int main(){
	int sock_raw;
	struct sockaddr_in source, dest;
	sock_raw=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sock_raw<0)
	{
		printf("error in socket\n");
		return -1;
	}
	unsigned char *buffer = (unsigned char *) malloc(65536); //to receive data
	memset(buffer,0,65536);
	struct sockaddr saddr;
	int saddr_len = sizeof (saddr);
		//Receive a network packet and copy in to buffer
		int buflen=recvfrom(sock_raw,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
		if(buflen<0)
		{
			printf("error in reading recvfrom function\n");
			return -1;
		}
		struct ethdr *eth = (struct ethhdr *) (buffer);
		printf("Ethernet Header \n");
		printf("Source As : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
		printf("Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
		printf("Protocol : %x \n",htons(eth->h_proto));
		printf("\n");
		unsigned short iphdrlen;
		struct iphr *iph = (struct iphr*)(buffer + sizeof(struct ethhdr));
		printf("IP header\n");
    	printf("\t\t\t |- Version : %d\n", iph->version);
    	printf("\t\t\t |- Inter Header Length : %d DWORDS or %d BYTES\n", (unsigned int)iph->ihl, (unsigned int)iph->ihl * 4);
		printf("\t\t\t |- Type Of Service : %d\n", (unsigned char)iph->TypeOfService);
    	printf("\t\t\t |- Total Length : %d Bytes\n", (unsigned short)ntohs(iph->TotalLength));
    	printf("\t\t\t |- Identification : %d\n", (unsigned short)iph->Identification);
    	printf("\t\t\t |- Time To Live : %d\n", (unsigned char)iph->Ttl);
    	printf("\t\t\t |- Protocol : %d\n", (unsigned char)iph->Protocol);
    	printf("\t\t\t |- Header Checksum : %d\n", (unsigned short)ntohs(iph->HeaderChecksum));
		source.sin_addr.s_addr = iph->SourceAddr;
    	dest.sin_addr.s_addr = iph->DestAddr;
    	printf("\t\t\t |- Source IP : %s\n", inet_ntoa(source.sin_addr));
    	printf("\t\t\t |- Destination IP : %s\n", inet_ntoa(dest.sin_addr));
		struct tcpheader *tcph = (struct tcpheader *)buffer;
    	printf("\ntcp header\n");
    	printf("\t\t\t |- Source Port\t : %d\n", (unsigned short)ntohs(tcph->SourcePort));
    	printf("\t\t\t |- Destination Port\t : %d\n", (unsigned short)ntohs(tcph->DestinationPort));
    	printf("\t\t\t |- Sequence Number\t : %d\n", (unsigned int)ntohs(tcph->SequenceNumber));
    	printf("\t\t\t |- Acknowledge Number\t : %d\n", (unsigned int)ntohs(tcph->AckNumber));
    	printf("\t\t\t |-Header Length : %d DWORDS or %d BYTES\n", (unsigned int)tcph->DataOffset, (unsigned int)tcph->DataOffset * 4);
    	printf("\n------------------------------------------------------flags------------------------------------------------------\n");
    	printf("\t\t\t\t |-Urgent flag : %d\n", (unsigned int)tcph->Urg);
    	printf("\t\t\t\t |-Acknowledgement flag : %d\n", (unsigned int)tcph->Ack);
    	printf("\t\t\t\t |-push flag : %d\n", (unsigned int)tcph->Psh);
    	printf("\t\t\t\t |-Reset flag : %d\n", (unsigned int)tcph->Rst);
    	printf("\t\t\t\t |-Synchronise flag : %d\n", (unsigned int)tcph->Syn);
    	printf("\t\t\t\t |-Finish flag : %d\n", (unsigned int)tcph->Fin);
    	printf("\t\t\t |-Window size  :%d\n", (unsigned short)ntohs(tcph->Window));
    	printf("\t\t\t |- checksum  :%d\n", (unsigned short)ntohs(tcph->Checksum));
    	printf("\t\t\t |- Urgent pointer  :%d\n", (unsigned short)ntohs(tcph->UrgentPointer));


    	printf("\n******************************************************END OF PACKET******************************************************\n");

}
