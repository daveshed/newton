#ifndef APPLICATION_H
#define APPLICATION_H

#include "buffer.h"
#include "interfaces.h"
#include "node.h"

namespace Newton {
/*
 * The Newton application.
 *
 * This class contains all the objects needed to run the app so that main,
 * simply needs to call this class appropriately.
 *
 * It uses a factory method to make the sensor which may either be a fake
 * sensor or a real one depending which sensor library it's linked against.
*/
class Application {
public:
    void setup(void);
    void loop(void);
    void handle_received(int n_bytes);
    void handle_requested(void);
private:
    Sensor* sensor_;
    Node* node_;
    FifoBuffer<uint8_t> tx_queue_;
    FifoBuffer<uint8_t> rx_queue_;
};

}; // namespace
#endif // APPLICATION_H