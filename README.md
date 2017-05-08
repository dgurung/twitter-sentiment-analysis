#### Description   
Read stream of tweets from Twitter's public streaming API using OAuth2.0 authentication. Store the streamed tweets into queue (ringbuffer). Analyse each tweets using Sentiment140 REST API.     

#### Dependencies   
+ libcurl   
+ liboauth   
+ boost    

#### Build   
```cpp
cmake .. -DBOOST_ROOT=/usr/local
make 
```

#### Technical Details:   
+ OAuth is designed using template method(GoF)      
+ ConnectionHandler handles curl session.     
+ TwitterHandler handles twitter connection.     
+ AnalysisHandler handles Sentiment140 REST API services. ( not uploaded in repository yet)     
+ Use CRTP idiom to instantiate TwitterHandler and AnalysisHandler from ConnectionHandler class template.     
+ . Use Ringbuffer for queuing up tweet stream       
+ Ringbuffer uses```boost::lockless::spsc_queue``` along with ```boost::interprocess::shared_memory```.     
