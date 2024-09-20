#ifndef TF_NODE_H
#define TF_NODE_H

#include "sma-controller.h"
#include "command-processor.h"
#include "settings.h"

class TFNode {
public:
    TFNode();
    void begin();
    void update();

private:
    SMAController smaController0;
    SMAController smaController1;
    CommandProcessor commandProcessor;
    Settings settings;
};

#endif // TF_NODE_H
