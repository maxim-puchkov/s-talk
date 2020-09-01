mpuchkov@sfu.ca, 301314335



** SIMPLE TALK **
S-Talk allows connecting two *localhost* ports to each other and 
sending messages between them by using threads and mutexes.



** HOW TO CONNECT TWO TERMINALS ** 
To connect you can:
1) run the executable in command line:
	./s-talk <your_port> <remote_name> <remote_port>
2) OR run the executable in command line:
	./s-talk
   and enter
	s-talk <your_port> <remote_name> <remote_port>

Open two (2) terminals on one (1) machine and enter:
	(terminal #1)	./s-talk 60555 name2 60888
	(terminal #2)	./s-talk 60888 name1 60555
You can choose your own ports but they should match as above.

Then you should be connected and you can send messages from one
terminal to another.



** HOW TO DISCONNECT **
Enter '!' at any time. This will also close the other session.



** COLOUR **
You - White
Other - Green
Simple Talk - Magenta
Preferred Background - Black



** IMPLEMENTATION **
Works on five threads. Four as described and an additional thread
that 'accept()' connections.

Output and receiver threads share mutex_receieve and send and 
keyboard input threads share mutex_send so that they can't access
the lists at the same time.

Message can be no longer than 1024 characters.



** BUGS **
1) Do not send '!' as first message
2) Abort trap: 6 when other user enters '!'
3) Very rarely might not connect ports 
4) Connects two ports through another random port



2018-02-25
