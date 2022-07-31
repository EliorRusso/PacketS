void SocketConnect(unsigned char *buffer);
void PrintingPackets(unsigned char *buffer);

struct ethdr { //
	unsigned char h_dest[ETH_ALEN];
	unsigned char h_source[ETH_ALEN];
	__be16 h_proto;
}__attribute__((packed));
struct iphr{
	#if __BYTE_ORDER == __LITTLE_ENDIAN
    	unsigned int ihl:4;
    	unsigned int version:4;
	#elif __BYTE_ORDER == __BIG_ENDIAN //order is crucial so when printed it wont print opposite values for version and ihl.
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
struct udpheader{
	unsigned int SourcePort:16;
	unsigned int DestinationPort:16;
	unsigned int Length:16;
	unsigned int Checksum:16;
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
}__attribute__((packed)); //compresses the information to save memory.