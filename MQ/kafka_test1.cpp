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
#include "rdkafka.h"

struct parkRecord
{
	std::string msg;
	std::string key;
	rd_kafka_resp_err_t err;
};

std::map<std::string, parkRecord> returnMsg;

template <class T>
std::string type2string(const T& data)
{
	std::ostringstream ss;
	ss << data;
	return ss.str();
}

void dr_msg_cb (rd_kafka_t *rk,
                       const rd_kafka_message_t *rkmessage, void *opaque) {
        std::cout<<(char*)(rkmessage->payload)<<std::endl;
        if (rkmessage->err)
                fprintf(stderr, "%% Message delivery failed: %s\n",
                        rd_kafka_err2str(rkmessage->err));
        else
                fprintf(stderr,
                        "%% Message delivered (%zd bytes, "
                        "partition %"PRId32")\n",
                        rkmessage->len, rkmessage->partition);

        /* The rkmessage is destroyed automatically by librdkafka */

		try
		{
			parkRecord record;
			record.err = rkmessage->err;
			record.msg = (char*)rkmessage->payload;

			std::cout<<"## "<< record.msg<<std::endl;
			
			Json::Reader reader;
			Json::Value json_object;
			const char* json_document = (char*)rkmessage->payload;
			reader.parse(json_document, json_object);
			record.key = json_object["name"].asString();

			std::cout<<"## record.key:"<< record.key<<std::endl;

			returnMsg.insert(std::map<std::string, parkRecord>::value_type(record.key, record));
			std::cout<<"insert success"<<std::endl;
		}
		catch(std::exception& e)
		{
			std::cout<<"parse msg error"<<std::endl;
		}
}

int main(int argc, char **argv)
{
    rd_kafka_t *rk;         /* Producer instance handle */
    rd_kafka_topic_t *rkt;  /* Topic object */
    rd_kafka_conf_t *conf;  /* Temporary configuration object */
    char errstr[512];       /* librdkafka API error reporting buffer */
    char buf[512];          /* Message value temporary buffer */
    const char *brokers = "127.0.0.1:9092";    /* Argument: broker list */
    const char *topic= "test3";      /* Argument: topic to produce to */

	
    conf = rd_kafka_conf_new();
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }
	#if 1
	if (rd_kafka_conf_set(conf, "security.protocol", "sasl_plaintext", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }
	if (rd_kafka_conf_set(conf, "sasl.mechanisms", "PLAIN", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }
	if (rd_kafka_conf_set(conf, "sasl.username", "admin", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }
	if (rd_kafka_conf_set(conf, "sasl.password", "test", errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }
	#endif

    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

	//用于全局配置和共享状态
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk)
    {
        fprintf(stderr,
                "%% Failed to create new producer: %s\n", errstr);
        return 1;
    }

    rkt = rd_kafka_topic_new(rk, topic, NULL);
    if (!rkt) 
	{
            fprintf(stderr, "%% Failed to create topic object: %s\n",
                    rd_kafka_err2str(rd_kafka_last_error()));
            rd_kafka_destroy(rk);
            return 1;
    }

	for(int i = 0; i < 10; ++i)
	{
		std::string key = "value";
		key += type2string(i);

		//Json::Value root;
    	//Json::FastWriter writer;
		//Json::Value msg;
		std::string json_str;
		json_str += "{\"name\":\"";
		json_str += key;
		json_str += "\"}";
		
		//msg["age"] = key;
   		//msg["gate_no"] = key;

		//root.append(msg);

		//std::string json_str = writer.write(root);
		//json_str = json_str.substr(1);
		//json_str = json_str.substr(0, json_str.length() - 2);

		int ret = rd_kafka_produce(
	                            /* Topic object */
	                            rkt,
	                            /* Use builtin partitioner to select partition*/
	                            RD_KAFKA_PARTITION_UA,
	                            /* Make a copy of the payload. */
	                            RD_KAFKA_MSG_F_COPY,
	                            /* Message payload (value) and length */
	                            (void *)json_str.c_str(), json_str.length(),
	                            /* Optional key and its length */
	                            (void*)key.c_str(), key.length(),
	                            /* Message opaque, provided in
	                             * delivery report callback as
	                             * msg_opaque. */
	                            NULL);
		std::cout<<"rd_kafka_produce ret:"<<ret<<std::endl;
		
	    if (ret == -1) 
	     {
	          /**Failed to *enqueue* message for producing. */
	          fprintf(stderr,
	                  "%% Failed to produce to topic %s: %s\n",
	                  rd_kafka_topic_name(rkt),
	                  rd_kafka_err2str(rd_kafka_last_error()));

	          /* Poll to handle delivery reports */
	          if (rd_kafka_last_error() == RD_KAFKA_RESP_ERR__QUEUE_FULL) 
			  {
	                /* If the internal queue is full, wait for
	                    * messages to be delivered and then retry.
	                    * The internal queue represents both
	                    * messages to be sent and messages that have
	                    * been sent or failed, awaiting their
	                    * delivery report callback to be called.
	                    *
	                    * The internal queue is limited by the
	                    * configuration property
	                    * queue.buffering.max.messages */
	                std::cout<<"ERR_QUEUE_FULL"<<std::endl;
	          }
	    } 
		else 
		{
	    	std::cout<<rd_kafka_topic_name(rkt)<<std::endl;
	    }

		//阻塞等待消息发送完成
	    rd_kafka_poll(rk, 0/*non-blocking*/);

	    fprintf(stderr, "%% Flushing final messages..\n");

		//等待完成producer请求完成
	    rd_kafka_flush(rk, 2*1000 /* wait for max 10 seconds */);

		std::map<std::string, parkRecord>::iterator it = returnMsg.find(key);
		if(it != returnMsg.end())
		{
			parkRecord record = it->second;
			std::cout<<"get msg, msg:" << record.msg << ", err:" << record.err <<std::endl;
			returnMsg.erase(key);
		}
	}

	std::cout<<"end size:"<<returnMsg.size()<<std::endl;
    /* Destroy topic object */
    rd_kafka_topic_destroy(rkt);

    /* Destroy the producer instance */
     rd_kafka_destroy(rk);

    return 0;
}
