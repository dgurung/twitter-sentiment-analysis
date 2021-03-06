#### Description    
Read stream of tweets from Twitter's public streaming API using OAuth2.0 authentication. Store the streamed tweets into queue (ringbuffer). Analyse each tweets using Sentiment140 REST API.  

#### Dependencies   
Install these libraries and dev files.
+ libcurl
+ liboauth
+ boost

**Install step for OAuth and cURL dev header files in ubuntu**
```
sudo apt-get install \
        liboauth-dev \
        curl libocurl4-openssl-dev
```

#### Requirement to access twitter public stream API   
Go to [twitter](https://dev.twitter.com/) and register your app. Also get access token key and secret.
+ Consumer key and secret.
+ Access token key and secret.

#### Build   
```
mkdir build && cd build
cmake .. -DBOOST_ROOT=/usr/local
make
./main
```

#### Architecture     
+ A producer handler subscribes to twitter public stream and gets the data.     
+ Data is buffered in a ring-buffer. Ring buffer is designed in single producer and single consmer scheme.   
+ A consumer consumes data from ring-buffer and delivers the data to Sentiment140.com     
+ Response from Sentiment140 visualized.  
Example of such architecture is here: [Building a Near Real-Time Discovery Platform with AWS](https://aws.amazon.com/blogs/big-data/building-a-near-real-time-discovery-platform-with-aws/)


#### Technical Details      
+ OAuth is designed using template method(GoF)
+ ConnectionHandler handles curl session.
+ TwitterHandler handles twitter connection.
+ AnalysisHandler handles Sentiment140 REST API services.
+ Use [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) idiom to instantiate TwitterHandler and AnalysisHandler from ConnectionHandler class template.   
+ Use Ringbuffer for queuing up tweet stream.   
+ Ringbuffer uses```boost::lockless::spsc_queue``` along with ```boost::interprocess::shared_memory```.   
+ Use [RAII](http://en.cppreference.com/w/cpp/language/raii) to design class. This idiom binds life cycle of all resources to the lifetime of an object.  
+ Two async threads for twitter stream handling and sentiment140 REST API handling.  


#### TODO       
+ fix bulk post request to sentiment140.com API  
