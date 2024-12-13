# Networking

This directory contains the Node's networking components responsible for duties such as packet deconstruction/construction, command processing/execution, response construction, and message publishing to connected networks over interfaces (CAN, UART, SPI).


![](Node-Command-Processing.drawio.png)


# Rebuilding Protobuf source

After changes are made to `tf-node-messagaes.proto`, you must rebuild the C++ source using `ptotoc` and `EmbeddedProto`


1. Install `protoc` and add it to path
2. Run the commands below in the terminal with your own directories.


```powershell
cd C:\Users\priva\Dropbox\Github\TF-Node-Firmware\lib\EmbeddedProto
$PROTO_DIR = "C:\Users\priva\Dropbox\Github\TF-Node-Firmware\src\networking"
$BUILD_DIR = "C:\Users\priva\Dropbox\Github\TF-Node-Firmware\src\networking"
$PROTO_FILE = "tfnode-messages.proto"
protoc --plugin=protoc-gen-eams=protoc-gen-eams.bat -I $PROTO_DIR --eams_out=$BUILD_DIR $PROTO_DIR/$PROTO_FILE
```

EmbeddedProto docs: https://embeddedproto.com/documentation/