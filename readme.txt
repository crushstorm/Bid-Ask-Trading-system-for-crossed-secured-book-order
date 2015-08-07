Input format: 
For adding security:
Timestamp A BID/ASK SECURITY_NAME NUMBEROFSECURITY PRICE ORDERID
To modify order based on order id : 
Timestamp M ORDERID NUMBEROFSECURITY PRICE

Readme : 
Datastructure used : Several hashmaps, multiset datastructure. 
Runtime complexity O(logn) to update, retrieve, and check those crossed or secure condition for each line read. 

data structure : 
priceof -> price of equity based on orderid
locked -> check whether the equity is locked
orderidlookup -> lookup the object containing details of each orderid using orderid as key
stockMapping -> using either ask/buy and equity symbol as pair key, and multiset of pair of (orderid and price) as value. 

Approach : 
For every add order, we will add the price of the order, and orderid to multiset based on bid/ask type, and name of equity. Moreover, the details of the order will be stored as an object in hash map orderidlookup. 
After each line, we need to check whether there is any change in the normal, secure, and crossed condition.  
Moreover, in order to gain quick access to change the value in the multiset, I use another hash map to store the price as value, and orderid as key. Hence, we can find a particular pair of orderid, and price in multiset in O(log n). 
While we’re updating the order, we need to update the multiset in order to maintain order from small to large, and also update the orderidlookup and priceof hash map. 
If there is a change from normal to other conditions, I also need to change the locked hash map. 

As hash map has O(1) time complexity despite higher space complexity, and multiset has O(log n) for each insert,erase operation. The program should run pretty efficient based N number of lines. 

Live demo: http://ideone.com/5LxkTP
