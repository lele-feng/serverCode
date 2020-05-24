#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <exception>

#include "json/json.h"

#include "rdkafkacpp.h"


struct parkRecord
{
	std::string msg;
	std::string key;
};

std::map<std::string, parkRecord> returnMsg;

template <class T>
std::string type2string(const T& data)
{
	std::ostringstream ss;
	ss << data;
	return ss.str();
}

void errorString (const std::string &name,
                         const std::string &errmsg,
                         const std::string &topic,
                         const std::string *key,
                         const std::string &value) {
  std::cout << "{ "
            << "\"name\": \"" << name << "\", "
            << "\"message\": \"" << errmsg << "\", "
            << "\"topic\": \"" << topic << "\", "
            << "\"key\": \"" << (key ? *key : "NULL") << "\", "
            << "\"value\": \"" << value << "\" "
            << "}" << std::endl;
}

class ExampleEventCb : public RdKafka::EventCb {
 public:
  void event_cb (RdKafka::Event &event) {
    switch (event.type())
    {
      case RdKafka::Event::EVENT_ERROR:
        std::cerr << ": ERROR (" << RdKafka::err2str(event.err()) << "): " <<
            event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_STATS:
        std::cerr << ": \"STATS\": " << event.str() << std::endl;
        break;

      case RdKafka::Event::EVENT_LOG:
        std::cout << ": LOG-" << event.severity() << "-"
                  << event.fac() << ": " << event.str() << std::endl;
        break;

      default:
        std::cout << ": EVENT " << event.type() <<
            " (" << RdKafka::err2str(event.err()) << "): " <<
            event.str() << std::endl;
        break;
    }
  }
};

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
 public:
  void dr_cb (RdKafka::Message &message) {
    //if (message.err()) 
	{
      errorString("dr cb", message.errstr(),
                  message.topic_name(),
                  message.key(),
                  std::string(static_cast<const char*>(message.payload()),
                              message.len()));
    
  	}
	}
};


int main(int argc, char **argv)
{
    std::string errstr;       /* librdkafka API error reporting buffer */
    char buf[512];          /* Message value temporary buffer */
    const char *brokers = "127.0.0.1:9092";    /* Argument: broker list */
    //const char *brokers = "192.168.120.86:9092";    /* Argument: broker list */
    //const char *topic= "parkrecord";      /* Argument: topic to produce to */

	int32_t partition = RdKafka::Topic::PARTITION_UA;
  	int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;

	RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

	/* Avoid slow shutdown on error */
 	 if (conf->set("message.timeout.ms", "60000", errstr)) {
    	std::cerr << ": " << errstr << std::endl;
    	exit(1);
  	}
  
	{
    	conf->set("client.id", std::string("rdkafka@test"), errstr);
 	}

  	conf->set("log.thread.name", "true", errstr);

  	/* correct producer offsets */
  	conf->set("produce.offset.report", "true", errstr);

  	/* auto commit is explicitly enabled with --enable-autocommit */
  	conf->set("enable.auto.commit", "false", errstr);

	conf->set("metadata.broker.list", brokers, errstr);
	conf->set("socket.timeout.ms", "10000", errstr);

  	conf->set("fetch.wait.max.ms", "500", errstr);
  	conf->set("fetch.min.bytes", "4096", errstr);

	ExampleEventCb ex_event_cb;
  	conf->set("event_cb", &ex_event_cb, errstr);

	ExampleDeliveryReportCb ex_dr_cb;
    /* Set delivery report callback */
    conf->set("dr_cb", &ex_dr_cb, errstr);

	RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
	if (!producer) {
      std::cerr << "Failed to create producer: " << errstr << std::endl;
      exit(1);
    }
	std::cout << "% Created producer " << producer->name() << std::endl;

	RdKafka::Topic *topic = RdKafka::Topic::create(producer, "parkrecord",
                                                   NULL, errstr);
	if (!topic) {
      std::cerr << ": Failed to create topic: " << errstr << std::endl;
      exit(1);
    }

	
	for(int i = 0; i < 50; ++i)
	{
		std::string msg = "value";
		msg += type2string(i);

		RdKafka::ErrorCode resp = producer->produce(topic, partition,
	                                  RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
	                                  const_cast<char *>(msg.c_str()),
	                                  msg.size(), NULL, NULL);
		std::cout<<"ret : "<< resp << std::endl;
		if (resp != RdKafka::ERR_NO_ERROR)
		{
			errorString("producer_send_error",
	                      RdKafka::err2str(resp), topic->name(), NULL, msg);
		}
		producer->poll(2 * 1000);
	}

    return 0;
}
