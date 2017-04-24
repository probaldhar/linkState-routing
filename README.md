# linkState
link state algorithm &amp; forwarding table

###### Usage -
	1. go to the folder using terminal/shell
	2. make
	3. ./node <routerLabel> <portNum> <totalNumRouters> <discoverFile> [-dynamic]

Here, routerLabel is a one word label describing the router itself, portNum is the port number on which the router receives and sends data, totalNumRouters is an integer for the total number of routers in the network, discoveryFile refers to the text file that contains neighbor information including the router’s own label, and the optional parameter –dynamic indicates if the router runs a dynamic network whose cost on a link may change.
