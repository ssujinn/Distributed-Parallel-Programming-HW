import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.clients.admin.*;

class Producer {
    public static void Send(String ID, String chat, String msg) {
        Properties config = new Properties();
        config.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");
        config.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");

        config.put(ProducerConfig.CLIENT_ID_CONFIG, ID);
        config.put(ProducerConfig.LINGER_MS_CONFIG, 1);
//        config.put(ProducerConfig.COMPRESSION_TYPE_CONFIG, "lz4");

        KafkaProducer<String, String> producer = new KafkaProducer(config);
        ProducerRecord<String, String> record = new ProducerRecord(chat, ID+": "+msg);

        producer.send(record);
        producer.close();
    }

    public static void genTopic(String ID, String topicName) throws Exception {

        Properties config = new Properties();
        config.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");
        config.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");
        config.put(ProducerConfig.CLIENT_ID_CONFIG, ID);
        config.put(ProducerConfig.LINGER_MS_CONFIG, 1);

        AdminClient adminClient = AdminClient.create(config);
/*
        ListTopicsOptions listTopicsOptions = new ListTopicsOptions();
        listTopicsOptions.listInternal(true);

        System.out.println(adminClient.listTopics(listTopicsOptions).names().get());
*/

        NewTopic newTopic = new NewTopic(topicName, 1, (short)1); //new NewTopic(topicName, numPartitions, replicationFactor)

        List<NewTopic> newTopics = new ArrayList<NewTopic>();
        newTopics.add(newTopic);

        adminClient.createTopics(newTopics);
        adminClient.close();
    }
    }
