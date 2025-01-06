#include "frame.h"
#include <sstream>
#include <iomanip>

Frame::Frame() {}
Frame::Frame(WORD originNode, WORD destinationNode, BOOL ACK_BIT, DWORD data, WORD eof, BYTE priority = 1)
    : originNode(originNode), destinationNode(destinationNode), ACK_BIT(ACK_BIT), data(data), end_of_frame(eof), priority(priority) {}


WORD  Frame::getOriginNode()        { return this->originNode;      }
WORD  Frame::getDestinationNode()   { return this->destinationNode; }
DWORD Frame::getData()              { return this->data;            }
BOOL  Frame::getAck()               { return this->ACK_BIT;         }
BYTE  Frame::getPriority()          { return this->priority;        }

QString Frame::toString() {
    if(!this->getAck()) return "Invalid frame.";

    std::stringstream stream;
    stream << std::setfill ('0') << std::hex << this->getData();
    QString result ="";
    result.append(stream.str());

    QString data = "CURRENT FRAME:\n";
    data.append("\nOrigin node: ");      data.append(std::to_string(this->originNode));
    data.append("\nDestination node: "); data.append(std::to_string(this->destinationNode));
    data.append("\nFrame data: ");       data.append(result);
    data.append("\nACK BIT: ");          data.append(std::to_string(this->ACK_BIT));
    data.append("\nPRIORITY: ");         data.append(std::to_string(this->priority));
    return data;
}

QString Frame::toPipeStructure() {
    std::stringstream stream;
    stream << std::setfill ('0') << std::hex << this->getData();
    QString result ="";
    result.append(stream.str());

    QString data = "";
    data.append(std::to_string(this->originNode));
    data.append(" ");   data.append(std::to_string(this->destinationNode));
    data.append(" ");   data.append(result);
    data.append(" ");   data.append(std::to_string(this->ACK_BIT));
    data.append(" ");   data.append(std::to_string(this->priority));
    return data;
}
