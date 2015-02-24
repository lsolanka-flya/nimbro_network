// UDP receiver node
// Author: Max Schwarz <max.schwarz@uni-bonn.de>

#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <map>
#include <vector>
#include <string>

#include <ros/publisher.h>

#include <topic_tools/shape_shifter.h>

#include <list>

#include "udp_packet.h"

namespace nimbro_topic_transport
{

struct Message
{
	Message(uint16_t id)
	 : id(id)
	 , size(0)
	{}

	Message()
	{}

	uint32_t getLength() const
	{ return size; }

	uint8_t* getData()
	{ return payload.data(); }

	bool decompress(Message* dest);

	uint16_t id;
	UDPFirstPacket::Header header;
	std::vector<uint8_t> payload;
	size_t size;
	std::vector<bool> msgs;
};

struct TopicData
{
	ros::Publisher publisher;

	uint32_t md5[4];
	std::string md5_str;
	std::string msg_def;

	int last_message_counter;
};

class UDPReceiver
{
public:
	UDPReceiver();
	~UDPReceiver();

	void run();
private:
	typedef std::map<std::string, TopicData> TopicMap;
	typedef std::list<Message> MessageBuffer;

	int m_fd;
	MessageBuffer m_incompleteMessages;
	TopicMap m_topics;

	bool m_dropRepeatedMessages;

	ros::NodeHandle m_nh;
	ros::Publisher m_pub_heartbeat;
	ros::Time m_lastHeartbeatTime;

	Message m_decompressedMessage;
};

}

#endif