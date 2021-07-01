import java.time.Duration;
import java.util.Collections;
import java.util.Properties;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;

public class Consumer {
    public static void Recv(String ID, String chat, boolean reset) {
        Properties config = new Properties();
        config.put(ConsumerConfig.GROUP_ID_CONFIG, ID);
        config.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        config.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
        config.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringDeserializer");
        config.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");
        config.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");

        KafkaConsumer<String, String> consumer = new KafkaConsumer(config);
        consumer.subscribe(Collections.singletonList(chat));
        String msg;
        try {
            if (reset == true) {
                consumer.poll(0);
                consumer.seekToBeginning(consumer.assignment());
            }
                ConsumerRecords<String, String> records = consumer.poll(Duration.ofMillis(10000));

                for (ConsumerRecord<String, String> record : records) {
                    msg = record.value();
                    System.out.println(msg);
                }
        } catch(Exception e) {
            // exception
        } finally {
            consumer.close();
        }

    }
}
