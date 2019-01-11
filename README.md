# MADJudge

MADJudge is a server/client application which allows its users to compete with others in competitive programming. MADJudge contests are quick, just for fun and you can apply for one any time you want. No friends to play with? Don't worry! At any time you can simulate a competition in which you'll be the only participant and help yourself get better at those pesky algorithms and data structures.

MADJudge is a university project developed for the purpose of "Software Development" class at the Faculty of Mathematics at University of Belgrade.


### Features:

-Contest and exercise mode

-Simple insertion of new problems and their testcases

-Simple and intuitive user interface

-Live feed during the contest

-Cross-platform client


## Prerequisites

To build MADJudge you will need Qt >= 4.0.
To run the server application you will need to have a linux distribution.


## Installation & Running

In order to use the client application, you will need to setup the server properly. At the moment, server port and IP are hardcoded in both applications, but feel free to edit them. 

Server is run as a console application - root privileges are not needed if the user running the server can communicate across the network. Once the server is started, you should wait until it informs you that the manager initialization completed successfully. Once that's done, the server is ready to manage the contests.

To run the client, you'll need to include required qt libraries in its folder. Once it's run, you'll be prompted for a temporary name which you will use to login and which will be visible to other players. After logging in, you'll be able to choose whether you want to compete against other players or practice. The only real difference between these modes is the presence of other players in the former. When the contest starts, you'll be shown a main window. Using this window is pretty intuitive.


### Authors

**M**iloš Ivanović
**A**leksa Jovanović
**D**jordje Jovanović
**J**elena Marković
**U**
**D**
**G**
**E**
