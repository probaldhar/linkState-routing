# linkState
link state algorithm &amp; forwarding table

### Usage -
	1. go to the folder using terminal/shell
	2. make
	3. ./node <routerLabel> <portNum> <totalNumRouters> <discoverFile> [-dynamic]

Here, *routerLabel* is a one word label describing the router itself, *portNum* is the port number on which the router receives and sends data, *totalNumRouters* is an integer for the total number of routers in the network, *discoveryFile* refers to the text file that contains neighbor information including the router’s own label, and the optional parameter *–dynamic* indicates if the router runs a dynamic network whose cost on a link may change.

### The File Format -

The neighbor discovery file must be formatted such that each neighbor of a router is described by a one word label, an IP address (or hostname), a port number, and a cost value as comma separate values on a single line. All neighbors of a router are listed line-by-line. The following describes the format where *\<routerLabel\>* is the label of the router, *\<IP_address/hostname\>* is the IP address or host name of the router, *\<portNumber\>* is the port number of the router, *\<cost\>* is the cost of the link.

*\<routerLabel\>,\<IP_address/hostname\>,\<portNumber\>,\<cost\>*  
*\<routerLabel\>,\<IP_address/hostname\>,\<portNumber\>,\<cost\>*  
*\<routerLabel\>,\<IP_address/hostname\>,\<portNumber\>,\<cost\>* 






